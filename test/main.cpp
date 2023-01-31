#include "../src/List.hpp"
#include "../src/Tree.hpp"

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <vector>
#include <utility>

using namespace DM852;

void DDL(List<int>& list);
void SGT(Tree<int, std::string>& tree);
std::vector<std::string> tokenize(std::string s, std::string del);

/**
 * @brief If first argument is "DLL" run input on doubly linked list.
 *        if first argument is "SGT" run input on Scapegoat tree.
 */
int main(int argc, char **argv) {
    if(argc == 1) {
        DM852::Tree<int, int> test;
        auto t = *test.end();
        std::cout << t.first << "\n";

        std::cout << "Must give command line arguments!" << std::endl;
        return 0;
    }
    //Doubly linked list commands
    if(strcmp(argv[1], "DLL") == 0) {
        List<int> list;
        DDL(list);
    }

    //Scapegoat tree commands
    if(strcmp(argv[1], "SGT") == 0) {
        Tree<int, std::string> tree;
        SGT(tree);
    }
    std::cout << std::endl;
}

/**
 * @brief carries out operations given the list of commands from cin.
 * @param list
 */
void DDL(List<int>& list) {
    List<int> tmp_list;

    for (std::string line; std::getline(std::cin, line);) { // parse each line
        std::vector<std::string> command = tokenize(line, " ");
        int key = 0;
        int index = 0;
        if(command.size() > 1)
            key = std::stoi(command[1]);
        if(command.size() > 2)
            index = std::stoi(command[2]);
        auto cmd = command[0];

        if(cmd == "pop") { // pop_back
            list.pop_back();
            std::cout << "Popped last element" << "\n";
        }
        if(cmd == "push") { // push_back
            list.push_back(key);
            std::cout << "Pushed element: " << key << "\n";
        }
        if(cmd == "clear") { // clear
            list.clear();
            std::cout << "Cleared list" << "\n";
        }
        if(cmd == "insert") { // insert
            auto iter = list.begin();
            for(int i = 0; i < index; i++) ++iter;
            list.insert(iter, key);
            std::cout << "Inserted: " << key << " at index " << index << "\n";
        }
        if(cmd == "empty") { // empty
            bool f = list.empty();
            if(f)
                std::cout << "List is empty" << "\n";
            else
                std::cout << "List is not empty" << "\n";
        }
        if(cmd == "erase") { // erase
            auto iter = list.begin();
            for(int i = 0; i < key; i++) ++iter;
            std::cout << "Erased element: " << *iter << "\n";
            list.erase(iter);
        }
        if(cmd == "size") { // size
            std::cout << list.size() << "\n";
        }
        if(cmd == "front") { // front
            std::cout << list.front() << "\n";
        }
        if(cmd == "back") { // back
            std::cout << list.back() << "\n";
        }
        if(cmd == "print") {
            for(auto iter = list.begin(); iter != list.end(); iter++)
                std::cout << *iter << " -> ";

            std::cout << "NULL" << "\n";
        }
        if(cmd == "print_tmp") {
            for(auto iter = tmp_list.begin(); iter != tmp_list.end(); iter++)
                std::cout << *iter << " -> ";

            std::cout << "NULL" << "\n";
        }
        if(cmd == "copy") {
            tmp_list = list;
            std::cout << "List copied" << "\n";
        }
        if(cmd == "move") {
            tmp_list = std::move(list);
            std::cout << "List moved" << "\n";
        }
        if(cmd == "==") {
            if(list == tmp_list)
                std::cout << "== returned true" << "\n";
            else
                std::cout << "== returned false" << "\n";
        }
        if(cmd == "stop") // breaking out of loop
            break;
        std::cout << std::flush;
    }
    std::cout << std::flush;
}

/**
 * @brief carries out operations given the list of commands from cin.
 * @param tree
 */
void SGT(Tree<int, std::string>& tree) {
    Tree<int, std::string> tmp_tree;
    for (std::string line; std::getline(std::cin, line);) { // parse each line
        std::vector<std::string> command = tokenize(line, " ");
        int key = 0;
        std::string value = "";
        if(command.size() > 1)
            key = std::stoi(command[1]);
        if(command.size() > 2)
            value = command[2];
        auto cmd = command[0];

        if(cmd == "size") { //size
            std::cout << tree.size() << "\n";
        } else if (cmd == "empty") { // empty
            bool f = tree.empty();
            if(f)
                std::cout << "Tree is empty" << "\n";
            else
                std::cout << "Tree is not empty" << "\n";
        } else if (cmd == "insert") {
            auto res = tree.insert(key, value);
            if(!res.second)
                std::cout << "Failed insert\n";
            else
                std::cout << "Inserted: [" << key << "|" << value << "]\n";
        } else if(cmd == "find") {
            auto tmp = tree.find(key);
            std::cout << "[" << tmp->first << "|" << tmp->second << "]"  << "\n";
        } else if (cmd == "clear") {
            tree.clear();
            std::cout << "Cleared Tree" << "\n";
        } else if (cmd == "erase") {
            std::cout << "Erased node: " << key << "\n";
            tree.erase(key);
        } else if(cmd == "front") {
            std::cout << tree.front().first << "|" << tree.front().second  << "\n";
        } else if(cmd == "back") {
            std::cout << tree.back().first << "|" << tree.back().second << "\n";
        } else if(cmd == "print") {
            std::cout << "Print: ";
            for(auto iter = tree.begin(); iter != tree.end(); iter++) {
                auto p = *iter;
                std::cout << "[" << p.first << "|" << p.second << "] ";
            }
            std::cout << "\n";
        } else if(cmd == "print_tmp") {
            std::cout << "Print TMP: ";
            for(auto iter = tmp_tree.begin(); iter != tmp_tree.end(); iter++) {
                auto p = *iter;
                std::cout << "[" << p.first << "|" << p.second << "] ";
            }
            std::cout << "\n";
        }
        else if(cmd == "copy") {
            tmp_tree = tree;
            std::cout << "Tree copied" << "\n";
        }
        else if(cmd == "move") {
            tmp_tree = std::move(tree);
            std::cout << "Tree moved" << "\n";
        }
        else if(cmd == "==") {
            if(tree == tmp_tree)
                std::cout << "== returned true" << "\n";
            else
                std::cout << "== returned false" << "\n";
        }
        else if(cmd == "stop") // breaking out of loop
            break;
    }
    std::cout << std::flush;
}

/**
 * @brief Tokenizes the input string using the delimiter.
 *
 * @param s String to tokenize.
 * @param del Delimiter.
 * @return std::vector<std::string> An array of string tokens.
 */
std::vector<std::string> tokenize(std::string s, std::string del)
{
    std::vector<std::string> res;
    int start = 0;
    int end = s.find(del);
    if(end == -1) {
        res.push_back(s);
        return res;
    }
    while (end != -1) {
        res.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    res.push_back(s.substr(start, end - start));
    return res;
}