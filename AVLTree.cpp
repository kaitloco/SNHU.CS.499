//============================================================================
// Name        : AVLTree.cpp
// Author      : Kaitlyn OConnell
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : CS-499 Module 4
//============================================================================

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <sstream>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;

    Bid() {
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node* left;
    Node* right;

    // Added height so the AVL tree can stay balanced.
    int height;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;

        // A new node starts with a height of 1.
        height = 1;
    }

    // initialize with a bid
    Node(Bid aBid) : Node() {
        bid = aBid;
    }
};

//============================================================================
// AVL Tree class definition
//============================================================================

class AVLTree {

private:
    Node* root;

    // Added functions to check the height and balance of each node.
    int getHeight(Node* node);
    int getBalanceFactor(Node* node);

    // Added rotations to keep the AVL tree balanced.
    Node* rightRotate(Node* node);
    Node* leftRotate(Node* node);

    // addNode now returns a Node* because rotations can change
    // the root of a subtree.
    Node* addNode(Node* node, Bid bid);

    // removeNode returns a Node* so the tree can be rebalanced
    // after a bid is removed.
    Node* removeNode(Node* node, string bidId);

    // Added helper to find the smallest node in a subtree.
    Node* minValueNode(Node* node);

    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);

    // Added helper to delete all nodes when the tree is destroyed.
    void destroyTree(Node* node);

public:
    AVLTree();
    virtual ~AVLTree();

    void InOrder();
    void PostOrder();
    void PreOrder();

    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
AVLTree::AVLTree() {

    // FixMe (1): initialize housekeeping variables
    // root starts empty
    root = nullptr;
}

/**
 * Destructor
 */
AVLTree::~AVLTree() {

    // FixMe (2)

   
    // The BST used removeNode() repeatedly to delete the tree.
    // The AVL tree uses a separate function to delete all nodes.

    // recurse from root deleting every node
    destroyTree(root);

    root = nullptr;
}



// Delete the tree using post-order traversal.
// Children are deleted before the parent.

void AVLTree::destroyTree(Node* node) {

    if (node != nullptr) {

        destroyTree(node->left);
        destroyTree(node->right);

        delete node;
    }
}



// Return the height of a node.
// Used to determine whether the tree is balanced.

int AVLTree::getHeight(Node* node) {

    if (node == nullptr) {
        return 0;
    }

    return node->height;
}



// Calculate the balance factor of a node.
//
// left height - right height
//
// A balance factor between -1 and 1 means the node is balanced.

int AVLTree::getBalanceFactor(Node* node) {

    if (node == nullptr) {
        return 0;
    }

    return getHeight(node->left) - getHeight(node->right);
}



// Right rotation used when the left side becomes too heavy.

Node* AVLTree::rightRotate(Node* node) {

    Node* leftChild = node->left;
    Node* middleSubtree = leftChild->right;

    // Perform rotation
    leftChild->right = node;
    node->left = middleSubtree;

    // Update heights after rotation
    node->height = 1 + max(
        getHeight(node->left),
        getHeight(node->right)
    );

    leftChild->height = 1 + max(
        getHeight(leftChild->left),
        getHeight(leftChild->right)
    );

    return leftChild;
}



// Left rotation used when the right side becomes too heavy.

Node* AVLTree::leftRotate(Node* node) {

    Node* rightChild = node->right;
    Node* middleSubtree = rightChild->left;

    // Perform rotation
    rightChild->left = node;
    node->right = middleSubtree;

    // Update heights after rotation
    node->height = 1 + max(
        getHeight(node->left),
        getHeight(node->right)
    );

    rightChild->height = 1 + max(
        getHeight(rightChild->left),
        getHeight(rightChild->right)
    );

    return rightChild;
}


/**
 * Traverse the tree in order
 */
void AVLTree::InOrder() {

    // FixMe (3a): In order root
    // call inOrder function and pass root
    inOrder(root);
}


/**
 * Traverse the tree in post-order
 */
void AVLTree::PostOrder() {

    // FixMe (4a): Post order root
    postOrder(root);
}


/**
 * Traverse the tree in pre-order
 */
void AVLTree::PreOrder() {

    // FixMe (5a): Pre order root
    preOrder(root);
}


// Traverse the tree in order
void AVLTree::inOrder(Node* node) {

    if (node != nullptr) {

        // left(small)
        inOrder(node->left);

        // output
        cout << node->bid.bidId << ": "
            << node->bid.title << " | "
            << node->bid.amount << " | "
            << node->bid.fund << endl;

        // right(large)
        inOrder(node->right);
    }
}


// Traverse the tree in post-order
void AVLTree::postOrder(Node* node) {

    if (node != nullptr) {

        postOrder(node->left);
        postOrder(node->right);

        cout << node->bid.bidId << ": "
            << node->bid.title << " | "
            << node->bid.amount << " | "
            << node->bid.fund << endl;
    }
}


// Traverse the tree in pre-order
void AVLTree::preOrder(Node* node) {

    if (node != nullptr) {

        cout << node->bid.bidId << ": "
            << node->bid.title << " | "
            << node->bid.amount << " | "
            << node->bid.fund << endl;

        preOrder(node->left);
        preOrder(node->right);
    }
}


/**
 * Insert a bid
 */
void AVLTree::Insert(Bid bid) {

    // FIXME (6a) Implement inserting a bid into the tree

  
    // addNode now returns the root of the subtree.
    // This is needed because an AVL rotation can change the root.

    root = addNode(root, bid);
}


/**
 * Remove a bid
 */
void AVLTree::Remove(string bidId) {

    // FIXME (7a) Implement removing a bid from the tree


    // This is needed because removing a node can cause a rotation.

    root = removeNode(root, bidId);
}


/**
 * Search for a bid
 */
Bid AVLTree::Search(string bidId) {

    // FIXME (8) Implement searching the tree for a bid

    Node* current = root;

    // keep looping downwards until bottom reached or matching bidId found
    while (current != nullptr) {

        // if match found, return current bid
        if (current->bid.bidId == bidId) {
            return current->bid;
        }

        // if bid is smaller than current node then traverse left
        if (bidId < current->bid.bidId) {
            current = current->left;
        }
        else {
            // else larger so traverse right
            current = current->right;
        }
    }

    // return empty bid if not found
    Bid bid;
    return bid;
}


/**
 * Add a bid to some node (recursive)
 */
Node* AVLTree::addNode(Node* node, Bid bid) {

    // FIXME (6b) Implement inserting a bid into the tree


	// The AVL tree returns node* because the root of a subtree can change after a rotation.

    // if node is empty, create a new node
    if (node == nullptr) {

        return new Node(bid);
    }

    // if bid is smaller then go left
    if (bid.bidId < node->bid.bidId) {

        // recurse down the left node
        node->left = addNode(node->left, bid);
    }
    else if (bid.bidId > node->bid.bidId) {

        // else go right
        node->right = addNode(node->right, bid);
    }
    else {

     
        // If the bid already exists, update the existing bid
        // instead of adding a duplicate node.

        node->bid = bid;

        return node;
    }

    // height of this ancestor node is 1 + max of left height and right height.
    // This is used to calculate the balance factor of each node.
    node->height = 1 + max(
        getHeight(node->left),
        getHeight(node->right)
    );


    // If the node becomes unbalanced, then there are 4 cases
    int balance = getBalanceFactor(node);


    // The new node was inserted into the left subtree of the left child.

    // Left Left case.


    if (balance > 1 && bid.bidId < node->left->bid.bidId) {

        return rightRotate(node);
    }


    // The new node was inserted into the right subtree of the right child.

    // Right Right case.


    if (balance < -1 && bid.bidId > node->right->bid.bidId) {

        return leftRotate(node);
    }


    // The new node was inserted into the right subtree of the left child.

    // Left Right case.
  

    if (balance > 1 && bid.bidId > node->left->bid.bidId) {

        node->left = leftRotate(node->left);

        return rightRotate(node);
    }


    // The new node was inserted into the left subtree of the right child.
    // Rotate the right child right, then rotate the current node left.

    if (balance < -1 && bid.bidId < node->right->bid.bidId) {

        node->right = rightRotate(node->right);

        return leftRotate(node);
    }

    return node;
}


/**
 * Find the node with the smallest bidId in a subtree.
 *
 * This is used when removing a node that has two children.
 * The smallest value in the right subtree is used to replace the value of the node being removed.
 */
Node* AVLTree::minValueNode(Node* node) {

	// The node is not empty, so start with the current node.

    Node* current = node;

    // The leftmost node contains the smallest bidId.
    while (current != nullptr && current->left != nullptr) {
        current = current->left;
    }

    return current;
}


/**
 * Remove a bid from some node (recursive)
 */
Node* AVLTree::removeNode(Node* node, string bidId) {

    // FIXME (7b) Implement removing a bid from the tree

    // If the node is empty, return it.
    if (node == nullptr) {
        return node;
    }

    // go left
    if (bidId < node->bid.bidId) {

        node->left = removeNode(node->left, bidId);
    }

    // go right
    else if (bidId > node->bid.bidId) {

        node->right = removeNode(node->right, bidId);
    }

    else {

        // node found

        // no children
        if (node->left == nullptr && node->right == nullptr) {

            delete node;
            return nullptr;
        }

        // one child (right/large)
        else if (node->left == nullptr) {

            Node* temp = node->right;

            delete node;

            return temp;
        }

        // one child (left/small)
        else if (node->right == nullptr) {

            Node* temp = node->left;

            delete node;

            return temp;
        }

        // two children
        else {

            

            // Find the smallest node in the right subtree.
            Node* temp = minValueNode(node->right);

            // Copy the bid information into the node being removed.
            node->bid = temp->bid;

            // Remove the duplicate node from the right subtree.
            node->right = removeNode(
                node->right,
                temp->bid.bidId
            );
        }
    }

  

    // Update the height after removing a node.
    node->height = 1 + max(
        getHeight(node->left),
        getHeight(node->right)
    );

	// Added code to check the balance factor after removing a node.
    // The tree may need to rotate after a removal.

    // Determine whether the tree is still balanced.
    int balance = getBalanceFactor(node);


    // Left Left case
    if (balance > 1 &&
        getBalanceFactor(node->left) >= 0) {

        return rightRotate(node);
    }


    // Left Right case
    if (balance > 1 &&
        getBalanceFactor(node->left) < 0) {

        node->left = leftRotate(node->left);

        return rightRotate(node);
    }


    // Right Right case
    if (balance < -1 &&
        getBalanceFactor(node->right) <= 0) {

        return leftRotate(node);
    }


    // Right Left case
    if (balance < -1 &&
        getBalanceFactor(node->right) > 0) {

        node->right = rightRotate(node->right);

        return leftRotate(node);
    }

    return node;
}


/**
 * Display the bid information
 */
void displayBid(Bid bid) {

    cout << bid.bidId << ": "
        << bid.title << " | "
        << bid.amount << " | "
        << bid.fund << endl;

    return;
}


/**
 * Load a CSV file containing bids into the tree
 */
void loadBids(string csvPath, AVLTree* bst) {

    cout << "Loading CSV file " << csvPath << endl;

    try {

        // initialize the CSV Parser
        csv::Parser file = csv::Parser(csvPath);

        vector<string> header = file.getHeader();

        for (auto const& c : header) {
            cout << c << " | ";
        }

        cout << endl;

        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;

            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            // push this bid to the tree
            bst->Insert(bid);
        }

        cout << file.rowCount()
            << " bids loaded successfully."
            << endl;
    }
    catch (csv::Error& e) {

        // Display the error and return to the menu.
        // This prevents the program from getting stuck after a CSV error.
        cerr << "Error loading CSV file: "
            << e.what()
            << endl;
    }
}


double strToDouble(string str, char ch) {

    str.erase(
        remove(str.begin(), str.end(), ch),
        str.end()
    );

    return atof(str.c_str());
}


int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath;

    switch (argc) {

    case 2:
        csvPath = argv[1];
        break;

    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a timer variable
    clock_t ticks;

	// AVL tree to hold all bids
    AVLTree* bst = new AVLTree();

    Bid bid;

    // Store menu input as a string so invalid input does not
    // leave cin in a failed state and cause the menu to repeat.
    string choiceInput;

    int choice = 0;

    bool running = true;

    while (running) {

        cout << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        // Read the entire menu choice as a line.
        // This prevents the program from continuously running
        // when invalid input is entered.
        if (!getline(cin, choiceInput)) {

            cout << endl;
            cout << "Exiting program." << endl;

            break;
        }

        // Check for an empty menu choice.
        if (choiceInput.empty()) {

            cout << "Please enter a value based on the options." << endl;

            continue;
        }

        stringstream choiceStream(choiceInput);

        // Try to convert the menu choice into an integer.
        if (!(choiceStream >> choice)) {

            cout << "Invalid Option. Please enter 1, 2, 3, 4, or 9."
                << endl;

            continue;
        }

        // Check for extra characters after the menu choice.
        string extraInput;

        if (choiceStream >> extraInput) {

            cout << "Invalid Option. Please enter 1, 2, 3, 4, or 9."
                << endl;

            continue;
        }

        switch (choice) {

        case 1: {

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            // Calculate elapsed time and display result
            ticks = clock() - ticks;

            cout << "time: "
                << ticks
                << " clock ticks"
                << endl;

            cout << "time: "
                << ticks * 1.0 / CLOCKS_PER_SEC
                << " seconds"
                << endl;

            // Return to the menu after loading bids.
            break;
        }


        case 2:

            cout << endl;
            cout << "Displaying all bids:" << endl;

            bst->InOrder();

            // Return to the menu after displaying the bids.
            break;


        case 3: {

            ticks = clock();

            cout << "Enter ID to search: ";

            string searchId;

            // getline waits for the user to enter the Auction ID.
            if (!getline(cin, searchId)) {

                cout << "Unable to read ID." << endl;

                running = false;

                break;
            }

            // Prevent an empty search from being treated as a valid ID.
            if (searchId.empty()) {

                cout << "ID cannot be empty."
                    << endl;

                break;
            }

            bid = bst->Search(searchId);

            ticks = clock() - ticks;

            if (!bid.bidId.empty()) {

                displayBid(bid);
            }
            else {

                cout << "Bid Id "
                    << searchId
                    << " not found."
                    << endl;
            }

            cout << "time: "
                << ticks
                << " clock ticks"
                << endl;

            cout << "time: "
                << ticks * 1.0 / CLOCKS_PER_SEC
                << " seconds"
                << endl;

            // Return to the menu.
            break;
        }


        case 4: {

            cout << "Enter ID to remove: ";

            string removeId;

            if (!getline(cin, removeId)) {

                cout << "Unable to read ID."
                    << endl;

                running = false;

                break;
            }

            // Prevent an empty ID from being sent to the tree.
            if (removeId.empty()) {

                cout << "ID cannot be empty."
                    << endl;

                break;
            }

            // Search first so the program can tell the user whether
            // a bid was actually found before removing it.
            bid = bst->Search(removeId);

            if (!bid.bidId.empty()) {

                bst->Remove(removeId);

                cout << "Bid "
                    << removeId
                    << " removed."
                    << endl;
            }
            else {

                cout << "Bid Id "
                    << removeId
                    << " not found."
                    << endl;
            }

            // Return to the menu.
            break;
        }


        case 9:

            // Exit the while loop.
            cout << "Good bye." << endl;

            running = false;

            break;


        default:

            cout << "Invalid option. Please enter 1, 2, 3, 4, or 9."
                << endl;

            // Return to the menu.
            break;
        }
    }

    delete bst;

    return 0;
}
