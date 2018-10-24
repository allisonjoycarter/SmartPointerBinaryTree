//
// Created by Hans Dulimarta.
//

#ifndef BINARYTREES_BINARYSEARCHTREE_H
#define BINARYTREES_BINARYSEARCHTREE_H
#include <memory>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <stack>

using namespace std;

namespace gv {
    template<typename E>     // textbook code E is Comparable
    class BinaryTree {

    public:
        /** WARNING: You will find a few overloaded functions declared in
         * both the public and private sections. This design is intentional
         * because many tree algorithms are recursive and the private counterpart
         * are the ones implementing the recursive work and they usually
         * require additional input argument(s).
         *
         * The non-recursive public functions simply call their recursive
         * counterpart to initiate the recursive work.
         */

        /**
         * Make the tree into an empty tree
         */
        void makeEmpty() noexcept {
            // TODO: complete this function
            root = nullptr;
        }

        /**
         * Insert a new data into the BST while maintaining no duplicate entries
         * @param item
         * @return true if the item can be inserted (no duplicate), false if the tree
         * already contains the same data item.
         */
        bool insert(const E &item) noexcept {
            if(contains(item))
                return false;

            insert(item, root);
            return true;

        }

        /**
         * Remove an item from the tree
         * @param item data to remove
         * @return true is the data is removed, false if the data is not in the tree
         * @throw length_error on an attempt to remove from an empty tree
         */
        bool remove(const E& item) {
            if(isEmpty())
                throw length_error("Nothing to remove!");
            return remove(item, root);
        }

        /**
         * Print the tree using in-order traversal. Separate data item by a single space
         * @param out destination of the print out
         */
        void printTree(ostream &targetStream = cout) const noexcept {
            // Be sure to use "targetStream" (and NOT cout) to print your data
            // For instance the following snippet would print "Hello"
            //   targetStream << "Hello";
            printInOrder(root, targetStream);
        }

        /**
         * Find the smallest value in the tree
         * @return the smallest value
         * @throw length_error if the tree is empty
         */
        const E findMin() const {
            if(isEmpty())
                throw length_error("Tree is empty");
            else {
                 Node *tmp = root.get();
                 while(tmp->left != nullptr) {
                     tmp = tmp->left.get();
                 }
                 return tmp->data;
            }
        };

        /**
         * Find the largest value in the tree
         * @return the largest value
         * @throw length_error if the tree is empty
         */
        const E findMax() const {
            if(isEmpty())
                throw length_error("Tree is empty");
            else {
                Node *tmp = root.get();
                while(tmp->right.get() != nullptr){
                    tmp = tmp->right.get();
                }
                return tmp->data;
            }
        }

        /**
         * Check if the given item is stored in the tree
         * @param val
         * @return true if the item is stored in the tree, false otherwise
         */
        bool contains(const E &val) const noexcept {
            return contains(val, root);
        }

        /**
         * Is the tree empty?
         * @return
         */
        bool isEmpty() const noexcept {
            return root == nullptr;
        }

        /**
         * Return the number of nodes in the tree (Problem 4.31a)
         * @return
         */
        int numberOfNodes() const noexcept {
//            return nodeCount; //apparently this is too easy
            return numberOfNodes(root);
        }

        /**
         * Return the number of leaves in the tree (Problem 4.31b)
         * @return
         */
        int numberOfLeaves() const noexcept {
            return numberOfLeaves(root);
        }

        /**
         * Return the number of full nodes (Problem 4.31c). A full node is a node with exactly two children
         * @return
         */
        int numberOfFullNodes() const noexcept {
            return numberOfFullNodes(root);
        }

        /**
         * Remove all the leaves from the tree and keep the data in these leaves into a vector
         * @return a vector of removed items
         */
        vector<E> remove_leaves() noexcept {
            vector<E> prunedLeaves;
            remove_leaves(root, prunedLeaves);
            // include the vector (prunedLeaves) about in your function invocation
            return prunedLeaves;
        }

        /**
         * Visit the node in level order (Problem 4.40)
         * @return
         */
        vector<E> levelOrder() const {
            vector<E> out;
            for (int i = 1; i <= getHeight(root)+1; ++i) {
                levelOrder(root, i, out);
            }
            return out;
        }

        static bool hasLeak() {
            return nodeCount != 0;
        }

        static int allocatedNodes() {
            return nodeCount;
        }

    private:
        struct Node;

        /**************************************************************
         * Makes a new node and inserts data into the tree using a
         * current node
         *
         * @param x data to insert into tree
         * @param t node used to find where to insert
         **************************************************************/
        void insert( const E &x, unique_ptr<Node> & t ) {
            unique_ptr<Node> newNode = make_unique<Node>();
            newNode -> data = x;
            if (t == nullptr){
                t = move(newNode);
            }
            else if( x < t->data)
                insert( x, t->left);
            else if( t->data < x )
                insert( x, t->right);
        }


        /**************************************************************
         * Recursively iterates through tree to remove desired data
         *
         * @param x data to remove from tree
         * @param t current node to find where to remove
         * @return true if removed, false if not found
         *************************************************************/
        bool remove( const E & x, unique_ptr<Node>& t ) {
            //Return false if no item is found
            if( t == nullptr )
                return false;
            // Iterate through to item to remove
            if( x < t->data )
                return remove( x, t->left );
            else if( t->data < x )
                return remove( x, t->right);
            else if(t->data == x
                && t->left != nullptr
                && t->right != nullptr ) // Two children
            {

                //find the min of the right subtree and store it
                Node *tmp = t->right.get();
                E min;
                while(tmp->left != nullptr) {
                    tmp = tmp->left.get();
                }
                min = tmp->data;

                //replace the data of the node to delete
                t->data = min;

                //remove the min data node since it was "moved"
                return remove( t->data, t->right );
            }

            else if (t->data == x
            && (t->left == nullptr ^ t->right == nullptr)) //one child
            {
                //make a node
                unique_ptr<Node> oldNode = make_unique<Node>();

                //set that node equal to the node to remove
                oldNode = move(t);

                //set the node to remove to either its left or right child
                t = ( oldNode->left != nullptr ) ? move(oldNode->left) : move(oldNode->right);

                //delete the unneeded node
                oldNode.reset();

                //call deconstructor?
                //delete oldNode;
            }
            else //no children, just get rid of it
                t.reset();
                //delete t;
            return true;
        }

        /**************************************************************
         * Recursively finds item that contains data and returns a bool
         * to represent whether the item was found or not
         *
         * @param val value to find
         * @param t current node to search
         * @return true if found, false if reaches null (not found)
         *************************************************************/
        bool contains(const E &val, const unique_ptr<Node>& t) const noexcept {
            if(t == nullptr) //if is null
                return false;
            else if(val > t->data) //if val is in the right subtree
                return contains(val, t->right);
            else if(val < t->data) //if val is in the left subtree
                return contains(val, t->left);

            return true; //otherwise we found it
        }

        /**************************************************************
         * Recursively counts the amount of nodes in the tree
         * @param root the node to count
         * @return the amount of nodes including root
         *************************************************************/
        int numberOfNodes(const unique_ptr<Node>& root) const noexcept {
            if(root == nullptr)
                return 0;
            if(root->right == nullptr && root->left == nullptr) //if leaf
                return 1;
            if(root->right != nullptr && root-> left != nullptr)
                return 1 + numberOfNodes(root->left) + numberOfNodes(root->right);
            if(root->right != nullptr)
                return 1 + numberOfNodes(root->right);
            if(root->left != nullptr)
                return 1 + numberOfNodes(root->left);
        }

        /**************************************************************
         * Recursively determines the amount of leaves
         *
         * @param root the node to count from
         * @return the amount of nodes without any children
         *************************************************************/
        int numberOfLeaves(const unique_ptr<Node>& root) const noexcept {
            if(root == nullptr)
                return 0;
            if(root->right == nullptr && root->left == nullptr)
                return 1;
            else
                return numberOfLeaves(root-> left) + numberOfLeaves(root->right);
        }

        /**************************************************************
         * Recursively counts the amount of nodes that have 2 children
         *
         * @param current the node to count from
         * @return the amount of nodes with 2 children
         *************************************************************/
        int numberOfFullNodes(const unique_ptr<Node>& current) const noexcept {
            if(current == nullptr) //if current does not exist
                return 0;
            if(current->left != nullptr && current->right != nullptr) { //if node is full
                return 1 + numberOfFullNodes(current->left) + numberOfFullNodes(current->right);
            } else //if anything else
                return numberOfFullNodes(current->left) + numberOfFullNodes(current->right);
        }

        /*************************************************************
         * Recursively finds and removes nodes without any children
         *
         * @param current the node to remove leaves from
         * @param out vector of leaf data
         ************************************************************/
        void remove_leaves(unique_ptr<Node>& current, vector<E>& out) {
            //exit if we are at a null
            if (current == nullptr)
                return;
            //go to left node
            remove_leaves(current->left, out);

            //go to right node
            remove_leaves(current->right, out);

            //if leaf, add to vector and delete pointer
            if (current->left == nullptr && current->right == nullptr) {
                out.push_back(current->data);
                current.reset();
//                delete current;
            }
        }

        /**************************************************************
         * Recursively adds data to vector in level order
         *
         * @param root the current node
         * @param level the current height
         * @param out vector to store data from nodes in level order
         *************************************************************/
        void levelOrder(const unique_ptr<Node>& root, int level, vector<E>& out) const {
            if(root == nullptr)
                return ;
            if(level == 1) {
                out.push_back(root->data);
            } else if( level > 1) {
                levelOrder(root->left, level - 1, out);
                levelOrder(root->right, level - 1, out);
            }
        }

        /**************************************************************
         * Recursively determines the height of a tree
         *
         * @param current node to determine the height of
         * @return the height of a tree
         *************************************************************/
        int getHeight(const unique_ptr<Node>& current) const {
            if(current == nullptr)
                return -1;

            int left = getHeight(current->left);
            int right = getHeight(current->right);

            if (left > right)
                return 1 + left;
            else
                return 1 + right;
        }

        /**************************************************************
         * Recursively adds data to ostream using in order traversal
         *
         * @param current node to traverse and add to stream
         * @param target stream to insert data to
         **************************************************************/
        void printInOrder(const unique_ptr<Node>& current, ostream& target) const noexcept {
            //move left
            if(current->left != nullptr)
                printInOrder(current->left, target);
            //print current
            target << current->data << " ";
            //move right
            if(current->right != nullptr)
                printInOrder(current->right, target);
        }

        static int nodeCount;
        // TODO: Make necessary modification to the Node
        // structure if you decide to implement a threaded tree
        struct Node {
            Node() {
                nodeCount++;
            }

            ~Node() {
                nodeCount--;
            }
            E data;
            unique_ptr<Node> left, right;
        };
        unique_ptr<Node> root;

    };

    template<typename E>
    int BinaryTree<E>::nodeCount = 0;
}
#endif //BINARYTREES_BINARYSEARCHTREE_H
