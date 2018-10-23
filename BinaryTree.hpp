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
            // TODO: complete this function by invoking a private recursive function
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
            // TODO: complete this function by invoking a private recursive function
            return remove(item, root);
        }

        /**
         * Print the tree using in-order traversal. Separate data item by a single space
         * @param out destination of the print out
         */
        void printTree(ostream &targetStream = cout) const noexcept {
            // TODO: complete this function by invoking a private recursive function
            // Be sure to use "targetStream" (and NOT cout) to print your data
            // For instance the following snippet would print "Hello"
            //   targetStream << "Hello";
        }

        /**
         * Find the smallest value in the tree
         * @return the smallest value
         * @throw length_error if the tree is empty
         */
        const E findMin() const {
            // TODO: complete this function
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
            // TODO: complete this function
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
            // TODO: complete this function by invoking a private recursive function
            return contains(val, root);
        }

        /**
         * Is the tree empty?
         * @return
         */
        bool isEmpty() const noexcept {
            // TODO: complete this function
            return root == nullptr;
        }

        /**
         * Return the number of nodes in the tree (Problem 4.31a)
         * @return
         */
        int numberOfNodes() const noexcept {
            // TODO: complete this function by invoking a private recursive function
//            return nodeCount; //apparently this is too easy
            return numberOfNodes(root);
        }

        /**
         * Return the number of leaves in the tree (Problem 4.31b)
         * @return
         */
        int numberOfLeaves() const noexcept {
            // TODO: complete this function by invoking a private recursive function
            return numberOfLeaves(root);
        }

        /**
         * Return the number of full nodes (Problem 4.31c). A full node is a node with exactly two children
         * @return
         */
        int numberOfFullNodes() const noexcept {
            // TODO: complete this function by invoking a private recursive function
            return numberOfFullNodes(root);
        }

        /**
         * Remove all the leaves from the tree and keep the data in these leaves into a vector
         * @return a vector of removed items
         */
        vector<E> remove_leaves() noexcept {
            vector<E> prunedLeaves;
            // TODO: complete this function by invoking a private recursive function
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
            // TODO: complete this function
//            out.push_back(root->data);
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

        // TODO: write your private functions here

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


        bool remove( const E & x, unique_ptr<Node>& t ) {
            if( t == nullptr )
                return false;
            // Item not found; do nothing
            if( x < t->data )
                return remove( x, t->left );
            else if( t->data < x )
                return remove( x, t->right);
            else if(t->data == x
                && t->left != nullptr
                && t->right != nullptr ) // Two children
            {
//                gv::BinaryTree<string> subtree;
//                subtree.root = t->right;
//                t->data = subtree.findMin();
                return remove( t->data, t->right );
            }
            else if (t->data == x
            && (t->left == nullptr ^ t->right == nullptr))
            {
                unique_ptr<Node> oldNode = make_unique<Node>();
                        oldNode = move(t);
                t = ( oldNode->left != nullptr ) ? move(oldNode->left) : move(oldNode->right);
                delete oldNode.release();
            }
            else //no children
                delete t.release();
            return true;
        }


        bool contains(const E &val, const unique_ptr<Node>& t) const noexcept {
            if(t == nullptr) //if is null
                return false;
            else if(val > t->data) //if val is in the right subtree
                return contains(val, t->right);
            else if(val < t->data) //if val is in the left subtree
                return contains(val, t->left);

            return true; //otherwise we found it
        }

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

        int numberOfLeaves(const unique_ptr<Node>& root) const noexcept {
            if(root == nullptr)
                return 0;
            if(root->right == nullptr && root->left == nullptr)
                return 1;
            else
                return numberOfLeaves(root-> left) + numberOfLeaves(root->right);
        }

        int numberOfFullNodes(const unique_ptr<Node>& current) const noexcept {
            if(current == nullptr) //if current does not exist
                return 0;
            if(current->left != nullptr && current->right != nullptr) { //if node is full
                return 1 + numberOfFullNodes(current->left) + numberOfFullNodes(current->right);
            } else //if anything else
                return numberOfFullNodes(current->left) + numberOfFullNodes(current->right);
        }

        vector<E> remove_leaves(unique_ptr<Node>& root, vector<E> out) {
            if (root == nullptr)
                return out;
            if (root->left == nullptr && root->right == nullptr) {
                out.push_back(root->data);
                root.reset();
//                delete root;
                return out;
            }
            if(root->left != nullptr)
                return remove_leaves(root->left, out);
            if(root-> right != nullptr)
                return remove_leaves(root->right, out);
        }

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
