/**
 * @file List.hpp
 * @author Emil Ovcina (emovc18@student.sdu.dk)
 * @brief A generic doubly linked list.
 *
 * @date 2022-05-16
 */
#ifndef LIST_H
#define LIST_H

#include <utility>

namespace DM852 {
template<typename T>
struct List {
    using value_type = T;
    private:
        /**
         * @brief Nested Node class
         */
        struct Node {
            T data;
            Node* prev;
            Node* next;

            /**
             * @brief Construct a new Node object. Cannot be default constructed.
             * @param data to store in the node.
             */
            Node(const T& data) : data(data) {
                prev = nullptr;
                next = nullptr;
            }

            /**
             * @brief Move Construct
             * @param data to move into the node.
             */
            Node(T&& data) : data(std::move(data)) {
                prev = nullptr;
                next = nullptr;
            }

            /**
             * @brief Destroy the Node object
             */
            ~Node() {
                delete next;
            }
        };
    public:
        struct iterator {
            friend class List;
            friend class const_iterator;
            using value_type = T;
            using reference = value_type&;

            /**
             * @brief Default constructer.
             */
            iterator() = default;

            /**
             * @brief Constructer.
             * @param list reference to the list which the node belongs to.
             * @param ptr pointer to the node where the iterator starts.
             */
            iterator(List& list, Node* ptr) : list(list), ptr(ptr) {}

            /**
             * @brief Copy Construct iterator.
             * @param other iterator reference.
             */
            iterator(const iterator& other) : list(other.list), ptr(other.ptr) {}

            /**
             * @brief Copy assignment for iterator.
             * @param other iterator reference.
             */
            iterator& operator=(const iterator& other) {
                list = other.list;
                ptr = other.ptr;
                return *this;
            }

            /**
             * @brief Desctruct the iterator.
             */
            ~iterator() = default;

            /**
             * @brief dereference operator overloaded.
             * @return reference to the data stored in the node
             */
            reference operator*() const {
                return (ptr->data);
            }

            /**
             * @brief Pre-increment operator. Moves the pointer to the nodes next node.
             * @return iterator ref with the next node.
             */
            iterator& operator++() {
                ptr = ptr->next;
                return *this;
            }

            /**
             * @brief Post-increment operator. Moves the pointer to the nodes next node,
             *                                  but returns a copy of the iterator before it is moved.
             * @return iterator ref.
             */
            iterator operator++(int) {
                auto tmp = *this;
                ptr = ptr->next;
                return tmp;
            }

            /**
             * @brief Pre-decrement operator. Moves the pointer to the nodes prev node.
             * @return iterator ref with the prev node.
             */
            iterator& operator--() {
                if(ptr == nullptr)
                    ptr = list.tail;
                else
                    ptr = ptr->prev;
                return *this;
            }

            /**
             * @brief Post-decrement operator. Moves the pointer to the nodes prev node,
             *                                  but returns a copy of the iterator before it is moved.
             * @return iterator ref.
             */
            iterator operator--(int) {
                auto tmp = *this;
                if(ptr == nullptr)
                    ptr = list.tail;
                else
                    ptr = ptr->prev;
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
             * @return The data in node.
             */
            value_type operator->() {
                return ptr->data;
            }

            private:
                List& list;
                Node* ptr;
        };

        struct const_iterator {
            friend class List;
            using value_type = T;
            using reference = const value_type&;

            /**
             * @brief Default constructer.
             */
            const_iterator() = default;

            /**
             * @brief Constructer.
             * @param list reference to the list which the node belongs to.
             * @param ptr pointer to the node where the const_iterator starts.
             */
            const_iterator(const List& list, List::Node* ptr) : list(list), ptr(ptr) {}

            /**
             * @brief Copy Construct const_iterator.
             * @param other const_iterator reference.
             */
            const_iterator(const const_iterator& other) : list(other.list), ptr(other.ptr) {}

            /**
             * @brief Copy Construct const_iterator. Used to convert iterator to const_iterator.
             * @param other iterator reference.
             */
            const_iterator(const iterator& other) : list(other.list), ptr(other.ptr) {}

            /**
             * @brief Copy assignment for const_iterator.
             * @param other const_iterator reference.
             */
            const_iterator& operator=(const const_iterator& other) {
                list = other.list;
                ptr = other.ptr;
                return *this;
            }

            /**
             * @brief Copy assignment for const_iterator. Used to convert iterator to const_iterator.
             * @param other iterator reference.
             */
            const_iterator& operator=(const iterator& other) {
                list = other.list;
                ptr = other.ptr;
                return *this;
            }

            /**
             * @brief Desctruct the const_iterator.
             */
            ~const_iterator() {}

            /**
             * @brief dereference operator overloaded.
             * @return reference to the data stored in the node
             */
            reference operator*() const {
                return ptr->data;
            }

            /**
             * @brief Pre-increment operator. Moves the pointer to the nodes next node.
             * @return const_iterator ref with the next node.
             */
            const_iterator& operator++() {
                ptr = ptr->next;
                return *this;
            }

            /**
             * @brief Post-increment operator. Moves the pointer to the nodes next node,
             *                                  but returns a copy of the iterator before it is moved.
             * @return const_iterator ref.
             */
            const_iterator operator++(int) {
                auto tmp = *this;
                ptr = ptr->next;
                return tmp;
            }

            /**
             * @brief Pre-decrement operator. Moves the pointer to the nodes prev node.
             * @return const_iterator ref with the prev node.
             */
            const_iterator& operator--() {
                if(ptr == nullptr)
                    ptr = list.tail;
                else
                    ptr = ptr->prev;
                return *this;
            }

            /**
             * @brief Post-decrement operator. Moves the pointer to the nodes prev node,
             *                                  but returns a copy of the iterator before it is moved.
             * @return const_iterator ref.
             */
            const_iterator operator--(int) {
                auto tmp = *this;
                if(ptr == nullptr)
                    ptr = list.tail;
                else
                    ptr = ptr->prev;
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
             * @return The data in node.
             */
            const value_type operator->() const {
                return ptr->data;
            }

            private:
                const List& list;
                Node* ptr;
        };

        /**
         * @brief Construct a new List object.
         */
        List() {
            head = nullptr;
            tail = nullptr;
            list_size = 0;
        }

        /**
         * @brief Copy Construct a new List object.
         *          Goes through each element and makes a copy of the node.
         *
         *          Runtime: O(n)
         * @param other list object.
         */
        List(const List& other) : list_size(other.list_size) {
            if(other.head) {
                head = new Node(other.head->data);
                Node* tmp = head;

                const_iterator iter = other.begin();
                iter++;
                for(;iter != other.end(); iter++) {
                    tmp->next = new Node(*iter);
                    tmp->next->prev = tmp;
                    tmp = tmp->next;
                }
                tail = tmp;
            } else {
                head = nullptr;
                tail = nullptr;
            }
        }

        /**
         * @brief Move Constructer. This list takes ownership of other lists nodes.
         * @param other list object.
         */
        List(List&& other) {
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;
            other.head = other.tail = nullptr;
            other.list_size = 0;
        }

        /**
         * @brief Move Assignment operator.
         *          This list takes ownership of other lists nodes.
         * @param other list object.
         */
        List& operator=(List&& other) {
            if(this == &other) return *this;
            delete head;
            head = other.head;
            tail = other.tail;
            list_size = other.list_size;
            other.head = other.tail = nullptr;
            other.list_size = 0;
            return *this;
        }

        /**
         * @brief Desctruct the List object.
         */
        ~List() {
            delete head;
        }

        /**
         * @brief Copy Assign - overloading '='
         *          Goes through each element and makes a copy of the node.
         *
         *          Runtime: O(n)
         * @param other list object.
         * @return copied List&
         */
        List& operator=(const List& other) {
            delete head;
            head = nullptr;
            tail = nullptr;
            if(other.head) {
                head = new Node(other.head->data);
                Node* tmp = head;

                const_iterator iter = other.begin();
                iter++;
                for(;iter != other.end(); iter++) {
                    tmp->next = new Node(*iter);
                    tmp->next->prev = tmp;
                    tmp = tmp->next;
                }

                tail = tmp;
                list_size = other.list_size;
            }
            return *this;
        }

        /**
         * @brief Equality comparison - overloading '=='
         *         This is implemented such that the lists must have the same order of elements as well.
         *         Another possibility would so the order does not matter, however this would make the runtime much worse (O(n^2)).
         *
         *          Runtime: O(n)
         * @param other list to compare with.
         * @return true if lists are equal.
         */
        bool operator==(const List& other) {
            if(other.size() != size()) return false;
            auto tmp = head;
            for(const_iterator iter = other.begin(); iter != other.end(); iter++) {
                if (tmp->data != *iter) return false;
                tmp = tmp->next;
            }
            return true;
        }

        /**
         * @brief Return size of list
         * @return int
         */
        int size() const {
            return list_size;
        }

        /**
         * @brief Checks the size of the list, and if 0 it is empty.
         * @return true if list is empty.
         */
        bool empty() const {
            return list_size == 0;
        }

        /**
         * @brief Inserts a new node at the end of the list and moves the tail pointer.
         * @param elem value_type to insert.
         */
        void push_back(const value_type& elem) {
            Node* node = new Node(elem);
            list_size++;
            if(head == nullptr) { //empty list
                head = node;
                tail = node;
                return;
            }
            node->prev = tail;
            tail->next = node;
            tail = node;
        }

        /**
         * @brief Inserts a new node at the end of the list, by moving, and moves the tail pointer.
         * @param elem rvalue value_type to insert by moving.
         */
        void push_back(value_type&& elem) {
            Node* node = new Node(elem);
            list_size++;
            if(head == nullptr) { //empty list
                head = node;
                tail = node;
                return;
            }
            node->prev = tail;
            tail->next = node;
            tail = node;
        }

        /**
         * @brief Creates and inserts a node into the list at the position right
         *           before the position given by the const_iterator.
         *          Pre-condition: The const_iterator pos must be an iterator of this list.
         *
         * @param pos const_iterator at the position to insert.
         * @param elem value_type to be insterted before pos.
         * @return iterator starting to the newly inserted node object.
         */
        iterator insert(const_iterator pos, const value_type& elem) {
            Node* prev = pos.ptr ? pos.ptr->prev : tail;
            Node* next = pos.ptr ? pos.ptr : nullptr;
            Node* newNode = new Node(elem);

            if(pos == begin()) // head
                head = newNode;
            else
                prev->next = newNode;
            if(pos == end()) // tail
                tail = newNode;
            else
                next->prev = newNode;
            newNode->next = next;
            newNode->prev = prev;
            list_size++;

            return iterator(*this, newNode);
        }

        /**
         * @brief Creates and inserts a node, by moving, into the list at the position right
         *           before the position given by the const_iterator.
         *          Pre-condition: The const_iterator pos must be an iterator of this list.
         *
         * @param pos const_iterator at the position to insert.
         * @param elem rvalue value_type to be insterted, by moving, before pos.
         * @return iterator starting to the newly inserted node object.
         */
        iterator insert(const_iterator pos, value_type&& elem) {
            Node* prev = pos.ptr ? pos.ptr->prev : tail;
            Node* next = pos.ptr ? pos.ptr : nullptr;
            Node* newNode = new Node(elem);

            if(pos == begin()) // head
                head = newNode;
            else
                prev->next = newNode;
            if(pos == end()) // tail
                tail = newNode;
            else
                next->prev = newNode;
            newNode->next = next;
            newNode->prev = prev;
            list_size++;

            return iterator(*this, newNode);
        }

        /**
         * @brief Removes the whole list by deleting all nodes starting from the head.
         *
         *          If memory de-allocation runtime should be counted:
         *          Runtime: O(n)
         */
        void clear() {
            delete head;
            head = nullptr;
            tail = nullptr;
            list_size = 0;
        }

        /**
         * @brief Removes the last element of the list
         *          And the node previous to the tail is set to be the new tail.
         */
        void pop_back() {
            if(tail) {
                if(tail == head) { // only one element
                    head = nullptr;
                    delete tail;
                    tail = nullptr;
                } else {
                    Node* tmp = tail->prev;
                    tail->prev->next = nullptr;
                    delete tail;
                    tail = tmp;
                }
                list_size--;
            }
        }

        /**
         * @brief Removes a node at the given position from the list.
         * @param pos const_iterator position to the node to be removed.
         */
        void erase(const_iterator pos) {
            Node* tmp = pos.ptr->prev;
            Node* tmp2 = tmp->next;
            if(pos == begin())
                head = pos.ptr->next;
            else
                tmp->next = pos.ptr->next;
            if(pos == --end())
                tail = tmp;
            else
                pos.ptr->next->prev = tmp;
            tmp2->next = tmp2->prev = nullptr; // set these to null, so only the node is deleted and not the next.
            list_size--;
            delete tmp2;
        }

        /**
         * @brief Returns the data of the head of the list.
         * @return const value_type& data of the head.
         *          Past the end iterator if list is empty.
         */
        const value_type& front() const {
            return *begin();
        }

        /**
         * @brief Returns the data of the head of the list.
         * @return value_type& data of the head.
         *          Past the end iterator if list is empty.
         */
        value_type& front() {
            return *begin();
        }

        /**
         * @brief Returns the data of the tail of the list.
         * @return value_type& data of the tail.
         *          Past the end iterator if list is empty.
         */
        const value_type& back() const {
            if(empty()) end();
            return *--end();
        }

        /**
         * @brief Returns the data of the tail of the list.
         * @return value_type& data of the tail.
         *          Past the end iterator if list is empty.
         */
        value_type& back() {
            if(empty()) end();
            return *--end();
        }

        /**
         * @return iterator at the start of the list,
         *          if list is empty, return a past the end iterator.
         */
        iterator begin() {
            if(empty()) return end();
            return iterator(*this, head);
        }

        /**
         * @return const_terator at the start of the list,
         *          if list is empty, return a past the end const_iterator.
         */
        const_iterator begin() const {
            if(empty()) return end();
            return const_iterator(*this, head);
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

    /**
     * @brief private member variables
     */
    private:
        int list_size;
        Node* head;
        Node* tail;
    };
};
#endif