/**
 * @file Tree.hpp
 * @author Emil Ovcina (emovc18@student.sdu.dk)
 * @brief Header file for a Scapegoat tree implementation.
 * @date 2022-05-16
 */

#ifndef TREE_H
#define TREE_H

#include <math.h>
#include <utility>
#include <numeric>

namespace DM852 {
    template<typename Key, typename Value, typename Comp = std::less<Key>>
    struct Tree {
        using value_type = std::pair<const Key, Value>;
    private:
        /**
         * @brief Nested Node class
         */
        struct Node {
            Node* parent;
            Node* left;
            Node* right;
            std::pair<const Key, Value>* pair;

            /**
             * @brief Finds the successor of the node.
             *          Runtime: O(h) where h is the height of the tree.
             *            But because it is self balancing and loosely
             *            alpha-height-balanced: h < h_alpha(n) + 1
             *            the running time is O(h_alpha(n))
             * @return Node* to the successor.
             */
            Node* next() {
                if(right) {
                    Node* res = right;
                    while(res->left) {
                        res = res->left;
                    }
                    return res;
                }
                if(parent && comp(pair->first, parent->pair->first))
                    return parent;  //if leaf and parent is bigger, the next should be the parent
                if(parent && parent->parent && comp(parent->pair->first, parent->parent->pair->first)) // if the next node is the parents parent
                    return parent->parent;
                return nullptr;
            }

            /**
             * @brief Overloaded next() function.
             * @return const Node* to the successor.
             */
            const Node* next() const {
                if(right) {
                    Node* res = right;
                    while(res->left) {
                        res = res->left;
                    }
                    return res;
                }
                if(parent && comp(pair->first, parent->pair->first))
                    return parent;  //if leaf and parent is bigger, the next should be the parent
                if(parent && parent->parent && comp(parent->pair->first, parent->parent->pair->first)) // if the next node is the parents parent
                    return parent->parent;
                return nullptr;
            }

            /**
             * @brief Finds the predecessor of the node.
             *          Runtime: O(h) where h is the height of the tree.
             *            But because it is self balancing and loosely 
             *            alpha-height-balanced: h < h_alpha(n) + 1
             *            the running time is O(h_alpha(n))
             * @return Node* to the predecessor.
             */
            Node* prev() {
                if(left) {
                    Node* res = left;
                    while(res->right) {
                        res = res->right;
                    }
                    return res;
                }
                if(parent && comp(parent->pair->first, pair->first)) {
                    return parent;  //if leaf and parent is bigger, the prev should be the parent
                }
                if(parent && parent->parent && comp(parent->parent->pair->first, parent->pair->first)) // if the prev node is the parents parent
                    return parent->parent;
                return nullptr;
            }

            /**
             * @brief Overloaded prev() function.
             * @return const Node* to the predecessor.
             */
            const Node* prev() const {
                if(left) {
                    Node* res = left;
                    while(res->right) {
                        res = res->right;
                    }
                    return res;
                }
                if(parent && comp(parent->pair->first, pair->first)) {
                    return parent;  //if leaf and parent is bigger, the prev should be the parent
                }
                if(parent && parent->parent && comp(parent->parent->pair->first, parent->pair->first)) // if the prev node is the parents parent
                    return parent->parent;
                return nullptr;
            }

            /**
             * @brief Construct a new Node object, and creates a pair object of the key and value.
             * @param key key of the node
             * @param value value of the node
             */
            Node(const Key& key, const Value& value) {
                pair = new std::pair<const Key, Value>(key, value);
                parent = left = right = nullptr;
            }

            /**
             * @brief Move Construct a new Node object, and creates a pair object of the key and value.
             * @param key rvalue key of the node
             * @param value rvalue value of the node
             */
            Node(Key&& key, Value&& value) {
                pair = new std::pair<const Key, Value>(std::move(key), std::move(value));
                parent = left = right = nullptr;
            }

            /**
             * @brief Destroy the Node object
             */
            ~Node() {
                delete left;
                delete right;
                delete pair;
            }
        };
    public:
        struct iterator {
            friend class Tree;
            friend class const_iterator;
            using value_type = std::pair<const Key, Value>;
            using reference = value_type&;

            /**
             * @brief Default constructer.
             */
            iterator() = default;

            /**
             * @brief Constructer.
             * @param tree reference to the tree which the node belongs to.
             * @param ptr pointer to the node where the iterator starts.
             */
            iterator(Tree& tree, Node* node) : tree(tree), ptr(node) {}

            /**
             * @brief Copy Construct iterator.
             * @param other iterator reference.
             */
            iterator(const iterator& other) : tree(other.tree), ptr(other.ptr) {}

            /**
             * @brief Copy assignment for iterator.
             * @param other iterator reference.
             */
            iterator& operator=(const iterator& other) {
                tree = other.tree;
                ptr = other.ptr;
                return *this;
            }

            /**
             * @brief Desctruct the iterator.
             */
            ~iterator() = default;

            /**
             * @brief dereference operator overloaded.
             * @return reference to the key value pair stored in the node
             */
            reference operator*() const {
                return *(ptr->pair);
            }

            /**
             * @brief Pre-increment operator. Moves the pointer to the nodes next node, by calling next().
             * @return iterator ref with the next node.
             */
            iterator& operator++() {
                ptr = ptr->next();
                return *this;
            }

            /**
             * @brief Post-increment operator. Moves the pointer to the nodes next node, by calling next(),
             *                                  but returns a copy of the iterator before it is moved.
             * @return iterator ref.
             */
            iterator operator++(int) {
                auto tmp = *this;
                ptr = ptr->next();
                return tmp;
            }

            /**
             * @brief Pre-decrement operator. Moves the pointer to the nodes prev node, by calling prev().
             * @return iterator ref with the prev node.
             */
            iterator& operator--() {
                if(ptr == nullptr)
                    ptr = tree.last;
                else
                    ptr = ptr->prev();
                return *this;
            }

            /**
             * @brief Post-decrement operator. Moves the pointer to the nodes prev node, by calling prev(),
             *                                  but returns a copy of the iterator before it is moved.
             * @return iterator ref.
             */
            iterator operator--(int) {
                auto tmp = *this;
                if(ptr == nullptr)
                    ptr = tree.last;
                else
                    ptr = ptr->prev();
                return tmp;
            }

            /**
             * @brief Equality operator. Checks if the pointers point to the same object.
             * @param rhs iterator ref to compare with.
             * @return true if this is equal to rhs
             */
            bool operator==(const iterator& rhs) {
                return ptr == rhs.ptr;
            }

            /**
             * @brief Inequality operator. Checks if the pointers point to the same object.
             *          This is probably redundant because of compiler optimization, but is left in for completeness sake.
             * @param rhs iterator ref to compare with.
             * @return true if this is not equal to rhs
             */
            bool operator!=(const iterator& rhs) {
                return ptr != rhs.ptr;
            }

            /**
             * @brief Member access operator.
             * @return The key value pair in node.
             */
            value_type* operator->() {
                return ptr->pair;
            }

            private:
                Tree& tree;
                Node* ptr;
        };
        struct const_iterator {
            friend class Tree;
            using value_type = std::pair<const Key, Value>;
            using reference = value_type&;

            /**
             * @brief Default constructer.
             */
            const_iterator() = default;

            /**
             * @brief Constructer.
             * @param tree reference to the tree which the node belongs to.
             * @param ptr pointer to the node where the const_iterator starts.
             */
            const_iterator(const Tree& tree, Node* node) : tree(tree), ptr(node) {}

            /**
             * @brief Copy Construct const_iterator.
             * @param other const_iterator reference.
             */
            const_iterator(const const_iterator& other) : tree(other.tree), ptr(other.ptr) {}

            /**
             * @brief Copy Construct const_iterator. Used to convert iterator to const_iterator.
             * @param other iterator reference.
             */
            const_iterator(const iterator& other) : tree(other.tree), ptr(other.ptr) {}

            /**
             * @brief Copy assignment for const_iterator.
             * @param other const_iterator reference.
             */
            const_iterator& operator=(const const_iterator& other) {
                tree = other.tree;
                ptr = other.ptr;
                return *this;
            }

            /**
             * @brief Copy assignment for const_iterator. Used to convert iterator to const_iterator.
             * @param other iterator reference.
             */
            const_iterator& operator=(const iterator& other) {
                tree = other.tree;
                ptr = other.ptr;
                return *this;
            }

            /**
             * @brief Desctruct the const_iterator.
             */
            ~const_iterator() = default;

            /**
             * @brief dereference operator overloaded.
             * @return reference to the key value pair stored in the node
             */
            reference operator*() const {
                return *(ptr->pair);
            }

            /**
             * @brief Pre-increment operator. Moves the pointer to the nodes next node, by using next().
             * @return const_iterator ref with the next node.
             */
            const_iterator& operator++() {
                ptr = ptr->next();
                return *this;
            }

            /**
             * @brief Post-increment operator. Moves the pointer to the nodes next node, by using next(),
             *                                  but returns a copy of the iterator before it is moved.
             * @return const_iterator ref.
             */
            const_iterator operator++(int) {
                auto tmp = *this;
                ptr = ptr->next();
                return tmp;
            }

            /**
             * @brief Pre-decrement operator. Moves the pointer to the nodes prev node, by using prev().
             * @return const_iterator ref with the prev node.
             */
            const_iterator& operator--() {
                if(ptr == nullptr)
                    ptr = tree.last;
                else
                    ptr = ptr->prev();
                return *this;
            }

            /**
             * @brief Post-decrement operator. Moves the pointer to the nodes prev node, by using prev(),
             *                                  but returns a copy of the iterator before it is moved.
             * @return const_iterator ref.
             */
            const_iterator operator--(int) {
                auto tmp = *this;
                if(ptr == nullptr)
                    ptr = tree.last;
                else
                    ptr = ptr->prev();
                return tmp;
            }

            /**
             * @brief Equality operator. Checks if the pointers point to the same object.
             * @param rhs const_iterator ref to compare with.
             * @return true if this is equal to rhs
             */
            bool operator==(const const_iterator& rhs) {
                return ptr == rhs.ptr;
            }

            /**
             * @brief Inequality operator. Checks if the pointers point to the same object.
             *          This is probably redundant because of compiler optimization, but is left in for completeness sake.
             * @param rhs const_iterator  ref to compare with.
             * @return true if this is not equal to rhs
             */
            bool operator!=(const const_iterator& rhs) {
                return ptr != rhs.ptr;
            }

            /**
             * @brief Member access operator.
             * @return The key value pair in node.
             */
            const value_type* operator->() const {
                return ptr->pair;
            }

            private:
                const Tree& tree;
                Node* ptr;
        };

        /**
         * @brief Default Construct a new Tree object
         */
        Tree() {
            root = first = last = nullptr;
            tree_size = max_size = 0;
            alpha = 0.57;
            comp = Comp();
        }

        /**
         * @brief Construct a new Tree object with comparitor as parameter
         * @param Comp object
         */
        Tree(Comp compare) {
            root = first = last = nullptr;
            tree_size = max_size = 0;
            alpha = 0.57;
            comp = compare;
        }

        /**
         * @brief Copy Construct a new Tree object
         *          Runs through the tree depth first and copies every node.
         *          This is used instead of "insert every element" because this way saves running time
         *          Also has to set the first and last pointers after copying.
         *
         *          Running time: O(n)
         * @param other tree to copy.
         */
        Tree(const Tree& other) : tree_size(other.tree_size), max_size(other.max_size), alpha(other.alpha) {
            root = nullptr;
            comp = other.comp;
            if(other.root)
                root = copy_helper(other.root);
            auto n = root;
            while(n->left)
                n = n->left;
            first = n;
            n = root;
            while(n->right)
                n = n->right;
            last = n;
        }

        /**
         * @brief Copy Assign - overloading '='
         *         Runs through the tree depth first and copies every node.
         *         This is used instead of "insert every element" because this way saves running time
         *          Also has to set the first and last pointers after copying.
         *
         *          Running time: O(n)
         * @param other tree to copy.
         * @return Tree& copy of tree.
         */
        Tree& operator=(const Tree& other) {
            delete root;
            root = nullptr;
            tree_size = other.tree_size;
            max_size = other.max_size;
            alpha = other.alpha;
            comp = other.comp;
            if(other.root)
                root = copy_helper(other.root);

            auto n = root;
            while(n->left)
                n = n->left;
            first = n;
            n = root;
            while(n->right)
                n = n->right;
            last = n;
            return *this;
        }

        /**
         * @brief Move Constructer. This tree takes ownership of other trees nodes.
         * @param other tree object.
         */
        Tree(Tree&& other) {
            root = other.root;
            first = other.first;
            last = other.last;
            tree_size = other.tree_size;
            max_size = other.max_size;
            alpha = other.alpha;

            other.root = other.first = other.last = nullptr;
            other.tree_size = other.max_size = 0;
        }

        /**
         * @brief Move Assignment operator.
         *          This tree takes ownership of other trees nodes.
         * @param other tree object.
         */
        Tree& operator=(Tree&& other) {
            if(this == &other) return *this;
            delete root;
            root = other.root;
            first = other.first;
            last = other.last;
            tree_size = other.tree_size;
            max_size = other.max_size;
            alpha = other.alpha;

            other.root = other.first = other.last = nullptr;
            other.tree_size = other.max_size = 0;
            return *this;
        }

        /**
         * @brief Destroy the Tree object
         */
        ~Tree() {
            delete root;
        }

        /**
         * @brief Equality comparison - overloading '=='
         *         Goes through the tree inorder and compares every node
         *         with the corresponding node in the other tree,
         *         both their parents and key value pair.
         *
         *          Runtime: O(n)
         * @param other tree to compare with.
         * @return true if the trees are equal.
         */
        bool operator==(const Tree& other) {
            if(tree_size != other.tree_size) return false;
            auto tmp = begin();
            for(const_iterator n = other.begin(); n != other.end(); n++) {
                //XOR on the parents.
                if((tmp.ptr->parent || n.ptr->parent) && (!tmp.ptr->parent || !n.ptr->parent))
                    return false;
                if(tmp.ptr->parent) // all node's parents must be the same
                    if(tmp.ptr->parent->pair->first != n.ptr->parent->pair->first || tmp.ptr->parent->pair->second != n.ptr->parent->pair->second)
                        return false;
                // Nodes must be the same.
                if(tmp.ptr->pair->first != n.ptr->pair->first || tmp.ptr->pair->second != n.ptr->pair->second)
                    return false;
                ++tmp;
            }
            return true;
        }

        /**
         * @brief Return size of tree.
         * @return int
         */
        int size() const {
            return tree_size;
        }

        /**
         * @brief Checks if size of tree is 0.
         * @return true if size = 0
         */
        bool empty() const {
            return tree_size == 0;
        }

        /**
         * @brief Inserts a new node into the tree.
         *     Since it is self balancing, after the node has been inserted
         *     it will check if the node is "too deep" meaning it is not alpha weight balanced:
         *     alpha-weight-balanced if: size(left(x)) ≤ alpha * size(x) && size(right(x)) ≤ alpha * size(x).
         *     If a node is too deep it will find the scapegoat node using parent pointers, and rebuild the tree at that node.
         *     This guarentees the loosely alpha-weight-balanced property for all nodes.
         *
         *     Runtime: O_A(log n) - amortized.
         * @param key of the node
         * @param value value of the node
         * @return std::pair<iterator, bool> - iterator at the position of the newly inserted node, bool is true if the insertion was a sucess.
         */
        std::pair<iterator, bool> insert(const Key& key, const Value& value) {
            Node* node = new Node(key, value);
            Node* tmp = nullptr;
            Node* n = root;
            bool left_most = true;
            bool right_most = true;
            int depth = 0; // used later
            while(n) {
                tmp = n;
                depth++;
                if (comp(node->pair->first, n->pair->first)) {
                    n = n->left;
                    right_most = false;
                }
                else if (!comp(node->pair->first, n->pair->first) && !comp(n->pair->first, node->pair->first)) { //node with key already exists
                    bool res_bool = false;
                    if(node->pair->second != n->pair->second) { //update value of node
                        n->pair->second = value;
                        res_bool = true;
                    }
                    delete node;
                    auto it = iterator(*this, n);
                    return std::make_pair(it, res_bool);
                }
                else {
                    n = n->right;
                    left_most = false;
                }
            }
            node->parent = tmp;
            if(!tmp)
                root = node;
            else if(comp(node->pair->first, tmp->pair->first))
                tmp->left = node;
            else
                tmp->right = node;
            tree_size++;
            max_size = std::max(max_size, tree_size);

            if(left_most)
                first = node;
            if(right_most)
                last = node;
            auto iter = iterator(*this, node);
            /* Self balancing part: */
            //Check if inserted node is too deep:
            if(depth > h_alpha() && tree_size > 2) {
                //Find scapegoat node:
                Node* tmp = node;
                while(tmp->parent != nullptr) {
                    Node* scn = tmp->parent;
                    int n_size = node_size(scn);
                    // Check if node is scapegoat candidate:
                    if(!(node_size(scn->left) <= alpha * n_size && node_size(scn->right) <= alpha * n_size)) {
                        Node* w = new Node(Key(), Value());
                        if(scn->parent == nullptr) { //root is scapegoat node, rebuild whole tree
                            root = flatten_wrapper(root, w);
                            root = build(n_size, root)->left;
                            max_size = tree_size;
                            w->left = nullptr;
                            delete w;
                            return std::make_pair(iter, true);
                        }
                        Node* scn_parent = scn->parent; // Remember the parent of the scapegoat node
                        Node* z = flatten_wrapper(scn, w);
                        scn = build(n_size, z)->left;
                        scn->parent = scn_parent;
                        if(comp(scn->pair->first, scn_parent->pair->first)) // left child
                            scn_parent->left = scn;
                        else                       // right right
                            scn_parent->right = scn;
                        max_size = tree_size;
                        w->left = nullptr;
                        delete w;
                        return std::make_pair(iter, true);
                    }
                    tmp = tmp->parent;
                }
            }
            return std::make_pair(iter, true);
        }

        /**
         * @brief Overloaded insert, but inserts by moving instead.
         * @param key rvalue key of the node
         * @param value rvalue value of the node
         * @return std::pair<iterator, bool> - iterator at the position of the newly inserted node, bool is true if the insertion was a sucess.
         */
        std::pair<iterator, bool> insert(Key&& key, Value&& value) {
            Node* node = new Node(key, value);
            Node* tmp = nullptr;
            Node* n = root;
            bool left_most = true;
            bool right_most = true;
            int depth = 0; // used later
            while(n) {
                tmp = n;
                depth++;
                if (comp(node->pair->first, n->pair->first)) {
                    n = n->left;
                    right_most = false;
                }
                else if (!comp(node->pair->first, n->pair->first) && !comp(n->pair->first, node->pair->first)) { //node with key already exists
                    bool res_bool = false;
                    if(node->pair->second != n->pair->second) { //update value of node
                        n->pair->second = value;
                        res_bool = true;
                    }
                    delete node;
                    auto it = iterator(*this, n);
                    return std::make_pair(it, res_bool);
                }
                else {
                    n = n->right;
                    left_most = false;
                }
            }
            node->parent = tmp;
            if(!tmp)
                root = node;
            else if(comp(node->pair->first, tmp->pair->first))
                tmp->left = node;
            else
                tmp->right = node;
            tree_size++;
            max_size = std::max(max_size, tree_size);

            if(left_most)
                first = node;
            if(right_most)
                last = node;
            auto iter = iterator(*this, node);
            /* Self balancing part: */
            //Check if inserted node is too deep:
            if(depth > h_alpha() && tree_size > 2) {
                //Find scapegoat node:
                Node* tmp = node;
                while(tmp->parent != nullptr) {
                    Node* scn = tmp->parent;
                    int n_size = node_size(scn);
                    // Check if node is scapegoat candidate:
                    if(!(node_size(scn->left) <= alpha * n_size && node_size(scn->right) <= alpha * n_size)) {
                        Node* w = new Node(Key(), Value());
                        if(scn->parent == nullptr) { //root is scapegoat node, rebuild whole tree
                            root = flatten_wrapper(root, w);
                            root = build(n_size, root)->left;
                            max_size = tree_size;
                            w->left = nullptr;
                            delete w;
                            return std::make_pair(iter, true);
                        }
                        Node* scn_parent = scn->parent; // Remember the parent of the scapegoat node
                        Node* z = flatten_wrapper(scn, w);
                        scn = build(n_size, z)->left;
                        scn->parent = scn_parent;
                        if(comp(scn->pair->first, scn_parent->pair->first)) // left child
                            scn_parent->left = scn;
                        else                       // right right
                            scn_parent->right = scn;
                        max_size = tree_size;
                        w->left = nullptr;
                        delete w;
                        return std::make_pair(iter, true);
                    }
                    tmp = tmp->parent;
                }
            }
            return std::make_pair(iter, true);
        }

        /**
         * @brief Finds a node given a key.
         *        Runtime: O(log n)
         *
         * @param key to find
         * @return iterator at the position to the found node. past the end iterator if not found.
         */
        iterator find(const Key& key) {
            auto tmp = root;
            while(tmp && tmp->pair->first != key) {
                if(comp(key, tmp->pair->first))
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            }
            if(tmp == nullptr)
                return end();
            return iterator(*this, tmp);
        }

        /**
         * @brief overloaded find() function.
         * @param key to find
         * @return const_iterator at the position to the found node. past the end iterator if not found.
         */
        const_iterator find(const Key& key) const {
            auto tmp = root;
            while(tmp && tmp->pair->first != key) {
                if(comp(key, tmp->pair->first))
                    tmp = tmp->left;
                else
                    tmp = tmp->right;
            }
            if(tmp == nullptr)
                return end();
            return const_iterator(*this, tmp);
        }

        /**
         * @brief Deletes all nodes and sets root to nullptr.
         *
         *          Runtime: O(n)
         */
        void clear() {
            delete root;
            first = last = root = nullptr;
            tree_size = 0;
            max_size = 0;
        }

        /**
         * @brief Removes a node given the key.
         *        Follows the pseudo-code in Introduction to Algorithms from Cormen et al. chapter 12 p.298
         *        case 1: z has no left child -> replace z with right child
         *                  if right child is null z is leaf
         *        case 2: z only has a left child -> replace z with left child.
         *        case 3: find z's successor and replace z with that.
         *
         *        Runtime: O_A(log n) - amortized
         * @param key of node to remove.
         */
        void erase(const Key& key) {
            auto pos = find(key);
            erase(pos);
        }

        /**
         * @brief Same implementation as erase(int key)
         *        Doesn't need to find the node first
         *        Runtime: O_A(log n) - amortized
         * @param const_iterator position of the node to remove.
         */
        void erase(const_iterator pos) {
            auto* node = const_cast<Node*>(pos.ptr);
            if(first == node) // deleting the first node
                first = node->next();
            if(last == node) // deleting the last node
                last = node->prev();
            if(node->left == nullptr) {
                subtree_shift(node, node->right);
            } else if (node->right == nullptr){
                subtree_shift(node, node->left);
            } else {
                auto y = node->next();
                if(y->parent != node) {
                    subtree_shift(y, y->right);
                    y->right = node->right;
                    y->right->parent = y;
                }
                subtree_shift(node, y);
                y->left = node->left;
                y->left->parent = y;
            }
            tree_size--;
            node->left = node->right = nullptr;
            delete node;

            //rebuild tree at root if too unbalanced.
            if(tree_size < alpha * max_size) {
                Node* w = new Node(Key(), Value());
                root = flatten_wrapper(root, w);
                root = build(tree_size, root)->left;
                max_size = tree_size;
                w->left = nullptr;
                delete w;
            }
        }

         /**
         * @brief The leftmost node.
         * @return value_type& value of the leftmost node.
         *          Past the end iterator if tree is empty.
         */
        value_type& front() {
            return *(begin());
        }

        /**
         * @brief The leftmost node.
         * @return const value_type& value of the leftmost node.
         *          Past the end const_iterator if tree is empty.
         */
        const value_type& front() const {
            return *(begin());
        }

        /**
         * @brief The rightmost node.
         * @return value_type& value of the rightmost node.
         *          Past the end iterator if tree is empty.
         */
        value_type& back() {
            if(empty()) end();
            return *(--end());
        }

        /**
         * @brief The rightmost node.
         * @return const value_type& value of the rightmost node.
         *          Past the end const_iterator if tree is empty.
         */
        const value_type& back() const {
            if(empty()) end();
            return *(--end());
        }

        /**
         * @return iterator at the position of the leftmost node.
         *          Past the end iterator if tree is empty.
         */
        iterator begin() {
            if(empty()) return end();
            return iterator(*this, first);
        }

        /**
         * @return const_iterator at the position of the leftmost node.
         *          Past the end const_iterator if tree is empty.
         */
        const_iterator begin() const {
            if(empty()) return end();
            return const_iterator(*this, first);
        }

        /**
         * @return past the end iterator.
         */
        iterator end() {
            return iterator(*this, nullptr);
        }

        /**
         * @return past the end const_iterator.
         */
        const_iterator end() const {
            return const_iterator(*this, nullptr);
        }

        private:
            Node* root;
            Node* first;
            Node* last;
            static Comp comp;
            int tree_size, max_size;
            float alpha;

            /**
             * @brief calculates floor(log_{1/alpha}(n)), n = size of the tree.
             * @return int
             */
            int h_alpha() {
                // this is equal to log(tree_size) with base 1/alpha:
                return floor(log(tree_size) / log(1/alpha));
            }

            /**
             * @brief Recursive function to calculate the size of a node
             *          by calculating the size of its subtrees.
             *
             *          Runtime: O(n)
             * @param node to find the size on.
             * @return int size of node.
             */
            int node_size(Node* node) {
                if(node == nullptr) return 0;
                return (node_size(node->left) + node_size(node->right)) + 1;
            }

            /**
             * @brief Builds a n-sized tree given linked list of nodes
             *        Uses divide and conquer approach to build the tree.
             *          Runtime: O(n)
             *          Described in "chapter 19 scapegoat trees" by Igal Galperin and Ronald L. Rivest.
             *          https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.309.9376
             *
             * @param n size of tree to build
             * @param x first element of the list
             * @return Node* pointer to a noe whose left child is the root of the built tree.
             */
            Node* build(float n, Node* x) {
                if (n == 0.0) {
                    x->left = nullptr;
                    return x;
                }
                Node* r = build(ceil((n-1)/2), x);
                Node* s = build(floor((n-1)/2), r->right);
                r->right = s->left;
                // Update parent pointers:
                if(r->right != nullptr)
                    r->right->parent = r;
                if(r->left != nullptr)
                    r->left->parent = r;
                s->left = r;
                return s;
            }

            /**
             * @brief Recursively flattens a binary tree and appends the linked list of nodes y
             *        Returns the result as a linked list of nodes.
             *          Runtime: O(n)
             *          Described in "chapter 19 scapegoat trees" by Igal Galperin and Ronald L. Rivest.
             *          https://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.309.9376
             *
             * @param x Root of a tree.
             * @param y First element of a linked list of nodes.
             * @return Node* to the first element of the linked list.
             */
            Node* flatten(Node* x, Node* y) {
                if(x == nullptr)
                    return y;
                x->right = flatten(x->right, y);
                return flatten(x->left, x);
            }

            /**
             * @brief Wrapper for the flatten() method. Calls flatten and removes the parent and left pointers
             *          This results in a simple linked list where the right pointers act as next pointers.
             *          Runtime: O(n)
             *
             * @param x Root of a tree.
             * @param y First element of a linked list of nodes.
             * @return Node* to the first element of the linked list.
             */
            Node* flatten_wrapper(Node* x, Node* y) {
                Node* n = flatten(x, y);
                Node* tmp = n;
                while(tmp) { // Removes all left and parent pointers when flattening the tree.
                    tmp->left = nullptr;
                    tmp->parent = nullptr;
                    tmp = tmp->right;
                }
                return n;
            }

            /**
             * @brief Replaces the subtrees of one node with the subtree of the other.
             *         Described in: Introduction to Algorithms from Cormen et al. chapter 12 p.296
             *
             * @param u root of subtree to update
             * @param v root of subtree to replace with
             */
            void subtree_shift(Node* u, Node* v) {
                //replace subtree of u with subtree of v
                if(u->parent == nullptr)
                    root = v;
                else if (u == u->parent->left)
                    u->parent->left = v;
                else
                    u->parent->right = v;
                if(v != nullptr)
                    v->parent = u->parent;
            }

            /**
             * @brief Depth first walk through and makes a copy of each node with all their pointers.
             *         Should not be used alone. Use copy constructer or '=' operator.
             *         Runtime: O(n)
             *
             * @param root root of the tree to copy.
             * @return Node* root of the copied tree.
             */
            Node* copy_helper(Node* root) {
                //Goes through the original tree depth first and copies each node
                if(!root) return nullptr;
                Node* n = new Node(root->pair->first, root->pair->second);
                if(root->left) {
                    n->left = copy_helper(root->left);
                    if(n->left)
                        n->left->parent = n;
                }
                if(root->right) {
                    n->right = copy_helper(root->right);
                    if(n->right)
                        n->right->parent = n;
                }
                return n;
            }
    };
};
#endif