#include <iostream>    
#include <string>  
#include <vector>
#include <bits/stdc++.h>

// avl implementation
// structure for AVL nodes
struct Node {
    // store the key value pair where
    int key;
    int value;
    Node *left; // left subtree
    Node *right;  // right subtree
    Node *parent; // parent of the node
    int height;   // height of the current node
    
    // constructors and destructors to simplify initialization
    Node(int key, int value, Node *left, Node *right, Node *parent, int height);
    ~Node();
};

// node constructor
Node::Node(int key, int value, Node *left, Node *right, Node *parent, int height)
    : key{key},
      value{value},
      left{left},
      right{right},
      parent{parent},
      height{height}
{}

// node destructor
Node::~Node() {
    delete left;
    delete right;
}

// define AVL tree class
struct AVL_t {
    // root of the current tree
    Node *root;
    // number of key comparisons
    int comp;
    
    // AVL methods
    // constructor and destructor
    AVL_t(Node *root, int comp);
    ~AVL_t();
    // delete the old tree to an empty tree
    void avl_clear();
    // bst node insert helper for bst insert
    Node * insert_bstnode(Node *n, int k, int v);
    // bst insert helper for avl insert
    Node * bst_insert(int k, int v);
    // bst delete helper for avl delete
    Node * remove_bstnode(Node *n, int k, Node *parent);
    
    // restructure to fix an avl tree
    Node * restructure(Node *x, Node *y, Node *z);
    // avl insertion
    int insert(int k, int v);
    // avl delete
    int avldelete(int k);
    // avl search, the same as binary search
    Node * search(int k);
    // getter function for the height of avl tree
    int getHeight();
};

// AVL tree constructor
AVL_t::AVL_t(Node *root, int comp)
    : root{root},
      comp{comp}
{}

// AVL tree destructor
AVL_t::~AVL_t() {
    delete root;
}

// clears the old avl tree
void AVL_t::avl_clear() {
    delete root;
    root = nullptr;
    comp = 0;
}

///////////////////////////////////////////////////////////////////////////////////////
// CS136 BST implementation
// BST insert implementation
// modified to update the key comparison and return the leaf where the new key
// is now stored
Node * AVL_t::insert_bstnode(Node *n, int k, int v) {
    // extract the key for the current node
    int k2 = n->key;
    // insert into the left child
    if (comp++ && k < k2) {
        // if the left child exists recurse on it
        if (n->left) {
            return insert_bstnode(n->left, k, v);
        }
        else {
            // otherwise make the new node to be the left subtree
            n->left = new Node{k, v, nullptr, nullptr, n, 0};
            return n->left;
        }
    }
    // insert into right child
    else if (comp++ && k > k2) {
        // if the right child exists recurse on it
        if (n->right) {
            return insert_bstnode(n->right, k, v);
        }
        else {
            // otherwise make the new node to be the right subtree
            n->right = new Node{k, v, nullptr, nullptr, n, 0};
            return n->right;
        }
    }
    else {
        // the key is already in the tree
        return nullptr;
    }
}

Node * AVL_t::bst_insert(int k, int v) {
    // if the tree is not empty, insert into the root
    if (root) {
        return insert_bstnode(root, k, v);
    }
    else {
        // make the new node the root
        root = new Node{k, v, nullptr, nullptr, nullptr, 0};
        return root;
    }
}

// BST delete implementation
// made a few modifications to update the number of key comparisons and return the
// parent of the deleted item
Node * AVL_t::remove_bstnode(Node *n, int k, Node *p) {
    // key did not exist
    if (n == nullptr) return nullptr;
    // extract the key value for the current node
    int k2 = n->key;
    // search for the node that contains the key
    if (comp++ && k < k2) {
        p = n;
        p = remove_bstnode(n->left, k, p);
    }
    else if (comp++ && k > k2) {
        p = n;
        p = remove_bstnode(n->right, k, p);
    }
    else if (n->left == nullptr) {
        // left child is empty makes right child the new node
        Node *temp = n->right;
        if (p != nullptr) {
            if (p->right == n) {
                p->right = temp;
            }
            else {
                p->left = temp;
            }
        }
        if (temp != nullptr) {
            temp->parent = p;
        }
        n->right = nullptr;
        n->parent = nullptr;
        delete n;
        return p;
    }
    else if (n->right == nullptr) {
        // right child is empty makes left child the new node
        Node *temp = n->left;
        if (p != nullptr) {
            if (p->right == n) {
                p->right = temp;
            }
            else {
                p->left = temp;
            }
        }
        if (temp != nullptr) {
            temp->parent = p;
        }
        n->left = nullptr;
        n->parent = nullptr;
        delete n;
        return p;
    }
    else {
        // neither children are null, replace current node by its successor
        // find the successor
        Node * next = n->right;
        while (next->left) {
            next = next->left;
        }
        // swap key/value
        n->key = next->key;
        n->value =  next->value;
        // remove the successor
        // don't want the parent to change
        p = n;
        p = remove_bstnode(n->right, next->key, p);
    }
    return p;
}
//////////////////////////////////////////////////////////////////////////////////////

// update height
void setHeightFromSubtrees(Node *u) {
    int max = 0;
    // let max be the greater of the heights of the left and right child
    if (u->left != nullptr && u->right != nullptr) {
        if ((u->left)->height >= (u->right)->height) {
            max = (u->left)->height;
        }
        else {
            max = (u->right)->height;
        }
    }
    else if (u->left != nullptr) {
        max = (u->left)->height;
    }
    else if (u->right != nullptr) {
        max = (u->right)->height;
    }
    else {
        max = -1;
    }
    
    u->height = max + 1;
}

// rotate right, helper function for avl restructure
Node * rotate_right(Node *z) {
    Node *y = z->left;
    z->left = y->right;
    if (z->left != nullptr) {
        (z->left)->parent = z;
    }
    y->right = z;
    y->parent = z->parent;
    z->parent = y;
    if (y->parent != nullptr) {
        if (y->parent->left == z) {
            (y->parent)->left = y;
        }
        else {
            (y->parent)->right = y;
        }
    }
    setHeightFromSubtrees(z);
    setHeightFromSubtrees(y);
    return y;
}

// rotate left, helper function for avl restructure
Node * rotate_left(Node *z) {
    Node *y = z->right;
    z->right = y->left;
    if (z->right != nullptr) {
        (z->right)->parent = z;
    }
    y->left = z;
    y->parent = z->parent;
    z->parent = y;
    if (y->parent != nullptr) {
        if (y->parent->left == z) {
            (y->parent)->left = y;
        }
        else {
            (y->parent)->right = y;
        }
    }
    setHeightFromSubtrees(z);
    setHeightFromSubtrees(y);
    return y;
}

// restructure, fixes the avl tree to retain avl properties
Node * AVL_t::restructure(Node *x, Node *y, Node *z) {
    // node x has parent y and y has parent z
    // case 1: y is the left child of z and x is the left child of y
    if (z->left == y && y->left == x) {
        return rotate_right(z);
    }
    else if (y == z->left && x == y->right) {
        // case 2: y is the left child of z and x is the right child of y
        z->left = rotate_left(y);
        return rotate_right(z);
    }
    else if(y == z->right && x == y->left) {
        // case 3: y is the right child of z and x is the left child of y
        z->right = rotate_right(y);
        return rotate_left(z);
    }
    else {
        // case 4: y is the right child of z and x is the right child of y
        return rotate_left(z);
    }
}

// avl insertion, modified to return the number of key comparisons
int AVL_t::insert(int k, int v) {
    // whenever runs insertion, reset key comparison counter
    comp = 1;
    // run bst insert and get the leaf where k is now stored
    Node *z = bst_insert(k, v);
    // fix the avl properties and reset the height
    while (z != nullptr) {
        // get the balance factor of z's left and right children
        int balance_f = 0;
        if (z->left != nullptr && z->right != nullptr) {
            balance_f = (z->left)->height - (z->right)->height;
        }
        else if (z->left != nullptr) {
            balance_f = (z->left)->height - -1;
        }
        else if (z->right != nullptr){
            balance_f = -1 - (z->right)->height;
        }
        else {
            balance_f = -1 - -1;
        }
        
        // if out of balance fix
        if (balance_f < -1 || balance_f > 1) {
            Node *y = nullptr;
            // let y be the taller child of z
            if (balance_f < -1) {
                y = z->right;
            }
            else {
                y = z->left;
            }
            // let x be the taller child of y
            Node *x = nullptr;
            int balance_f2 = 0;
            
            if (y->left != nullptr && y->right != nullptr) {
                balance_f2 = (y->left)->height - (y->right)->height;
            }
            else if (y->left != nullptr) {
                balance_f2 = (y->left)->height - -1;
            }
            else if (y->right != nullptr){
                balance_f2 = -1 - (y->right)->height;
            }
            else {
                balance_f2 = -1 - -1;
            }
            
            if (balance_f2 < 0) {
                x = y->right;
            }
            else {
                x = y->left;
            }
            z = restructure(x, y, z);
            if (z->parent == nullptr) {
                root = z;
            }
            break;
        }
        setHeightFromSubtrees(z);
        z = z->parent;
    }
    return comp - 1;
}

// avl deletion, modified to return the number of key comparisons
int AVL_t::avldelete(int k) {
    // whenever runs deletion, reset the key comparison counter
    comp = 1;
    // run bst delete and get the parent of the key being deleted
    Node *z = remove_bstnode(root, k, nullptr);
    // restructure to fix the avl properties
    while (z != nullptr) {
        // get the balance factor of z's left and right children
        int balance_f = 0;
        if (z->left != nullptr && z->right != nullptr) {
            balance_f = (z->left)->height - (z->right)->height;
        }
        else if (z->left != nullptr) {
            balance_f = (z->left)->height - -1;
        }
        else if (z->right != nullptr){
            balance_f = -1 - (z->right)->height;
        }
        else {
            balance_f = -1 - -1;
        }
        // if out of balance fix
        if (balance_f < -1 || balance_f > 1) {
            Node *y = nullptr;
            // let y be the taller child of z
            if (balance_f < -1) {
                y = z->right;
            }
            else {
                y = z->left;
            }
            // let x be the taller child of y
            Node *x = nullptr;
            int balance_f2 = 0;
            
            if (y->left != nullptr && y->right != nullptr) {
                balance_f2 = (y->left)->height - (y->right)->height;
            }
            else if (y->left != nullptr) {
                balance_f2 = (y->left)->height - -1;
            }
            else if (y->right != nullptr){
                balance_f2 = -1 - (y->right)->height;
            }
            else {
                balance_f2 = -1 - -1;
            }
            
            if (balance_f2 < 0) {
                x = y->right;
            }
            else if (balance_f2 > 0){
                x = y->left;
            }
            else {
                // break ties to prefer single rotation
                if (y == z->left) {
                    x = y->left;
                }
                else {
                    x = y->right;
                }
            }
            z = restructure(x, y, z);
        }
        setHeightFromSubtrees(z);
        if (z->parent == nullptr) {
                root = z;
        }
        z = z->parent;
    }
    return comp - 1;
}

// avl search, the same as bineary search, updates number of key comparisons
// returns nullptr if the key is not found otherwise returns the node with
// the correct key
Node * AVL_t::search(int k) {
    // reset key comparison counter
    comp = 1;
    Node *n = root;
    while (root) {
        if (comp++ && k < n->key) {
            n = n->left;
        }
        else if (comp++ && k > n->key) {
            n = n->right;
        }
        else {
            return n;
        }
    }
    return nullptr;
}

// getter function for avl height
int AVL_t::getHeight() {
    return root->height;
}
//////////////////////////////////////////////////////////////////////////////////////

// skiplist implementation
// structure for skiplist nodes
struct Sknode {
    // key value pair
    int key2;
    int value2;
    // nodes linking to the next item in the same level and linking to next item
    // in the tower
    Sknode *after;
    Sknode *below;
    
    // constructor and destructor to simplify definition
    Sknode(int key2, int value2, Sknode *after, Sknode *below);
    ~Sknode();
};

// sknode constructor and destructor
Sknode::Sknode(int key2, int value2, Sknode *after, Sknode *below)
    : key2{key2},
      value2{value2},
      after{after},
      below{below}
{}

Sknode::~Sknode() {
    delete after;
    delete below;
}

struct SkipList {
    // pointer to the root of the skip list
    Sknode *root2;
    // counter for number of key comparisons
    int comp2;
    // counter for number of nodes
    int num_node;
    
    // skip list methods
    // skip list constructor and destructor
    SkipList(Sknode *root2, int comp2, int num_node);
    ~SkipList();
    // initializes an empty skip list or delete the old lists
    void init_sklist();
    // get a stack of predecessors of the node with key k
    std::vector<Sknode *> getPredecessors(int k);
    // skip list insert
    int skinsert(int k, int v, int coin);
    // skip list delete
    int skdelete(int k);
    // skip list search
    Sknode * sksearch(int k);
    // getter function for the number of nodes in the skip list
    int getNum_node();
};

// skip list constructor and destructor
SkipList::SkipList(Sknode *root2, int comp2, int num_node)
    : root2{root2},
      comp2{comp2},
      num_node{num_node}
{}

SkipList::~SkipList() {
    delete root2;
}

// initializes an empty skip list with only two sentinel nodes
// delete the old list if one exists and reset it to the empty skip list
void SkipList::init_sklist() {
    if (num_node != 0) {
        delete root2;
    }
    comp2 = 0;
    num_node = 0;
    Sknode *sentinel = new Sknode{INT_MAX, 0, nullptr, nullptr};
    root2 = new Sknode{INT_MIN, 0, sentinel, nullptr};
}

// get a stack of predecessors of the node with key k and return a vector
// storing the predecessors
std::vector<Sknode *> SkipList::getPredecessors(int k) {
    Sknode *p = root2;
    std::vector<Sknode *> pred;
    pred.push_back(p);
    // keep dropping until reach S0
    while (p->below != nullptr) {
        p = p->below;
        while (comp2++ && k > (p->after)->key2 && (p->after)->key2 != INT_MAX) {
            p = p->after;
        }
        pred.push_back(p);
    }
    return pred;
}

// skip list insert, returns the number of key comparisons
int SkipList::skinsert(int k, int v, int coin) {
    // reset key comparison counter to 0
    comp2 = 1;
    // get current height of the skip list
    int h = 0;
    for (Sknode * n = root2->below; n != nullptr; n = n->below) {
        h++;
    }
    // increase the neight of skip list if needed
    while (coin >= h) {
        Sknode *sentinel = new Sknode{INT_MAX, 0, nullptr, root2->after};
        root2 = new Sknode{INT_MIN, 0, sentinel, root2};
        h++;
    }
    // get the predecessors for which where the new node should be inserted after
    std::vector<Sknode *> pred = getPredecessors(k);
    Sknode *p = pred.back();
    pred.pop_back();
    // create the node with the new key
    Sknode *zBellow = new Sknode{k, v, p->after, nullptr};
    // insert after p in S0
    p->after = zBellow;
    num_node = num_node + coin + 1;
    // insert into higher levels
    while (coin > 0) {
        // get the next predecessor
        p = pred.back();
        pred.pop_back();
        Sknode *z = new Sknode{k, v, p->after, zBellow};
        p->after = z;
        zBellow = z;
        coin = coin - 1;
    }
    return comp2 - 1;
}

// skip list deletion, modified to return number of key comparisons
int SkipList::skdelete(int k) {
    // whenever deletion is called reset key comparison counter to 0
    comp2 = 1;
    // get the stack of predecessors of the item to be deleted
    std::vector<Sknode *> pred = getPredecessors(k);
    // delete k from the skip list if it exists
    while (!(pred.empty())) {
        Sknode *p = pred.back();
        pred.pop_back();
        // if k is found delete it
        if (comp2++ && (p->after)->key2 == k) {
            Sknode *temp = p->after;
            p->after = temp->after;
            temp->after = nullptr;
            temp->below = nullptr;
            delete temp;
            num_node = num_node - 1;
        }
        else {
            // otherwise k does not exist don't need to delete anything
            break;
        }
    }
    // delete unnecessary sentinel levels
    Sknode *p = root2;
    while ((p->below)->after->key2 == INT_MAX) {
        // the two top lists are both only sentinels remove one
        Sknode *temp2 = p->below;
        p->below = temp2->below;
        (p->after)->below = (temp2->after)->below;
        temp2->after = nullptr;
        temp2->below = nullptr;
        delete temp2;
    }
    return comp2 - 1;
}

// skip list search, return the node where the key is found otherwise return nullptr
Sknode * SkipList::sksearch(int k) {
    // whenever called, reset key comparison counter
    comp2 = 1;
    // get the stack of predecessors of the key k
    std::vector<Sknode *> pred = getPredecessors(k);
    Sknode *top =  pred.back();
    if (comp2++ && (top->after)->key2 == k) {
        return top->after;
    }
    else {
        // the k is not in the skip list
        return nullptr;
    }
}

// getter function for the number of nodes in the skip list
int SkipList::getNum_node() {
    return num_node;
}
////////////////////////////////////////////////////////////////////////////////////
//
// main function
int main() {
    // counter for number of items currently in the avl tree and skip list
    int items = 0;
    // avl tree and a skip list
    AVL_t avl{nullptr, 0};
    SkipList sklist{nullptr, 0, 0};
    
    // get command line arguments
    std::string cmd;
    int key = 0;
    int val = 0;
    int coin = 0;
    while (std::cin >> cmd) {
        // initializes or resets the avl tree and skip list
        if (cmd == "r") {
            // if avl tree is not empty, reset it
            if (avl.root != nullptr) {
                avl.avl_clear();
            }
            // for sklist just call the init function for SkipList which
            // takes care of the case that the skip list is not empty
            sklist.init_sklist();
        }
        else if (cmd == "i") {
            // the insert command
            std::cin >> key;
            std::cin >> val;
            std::cin >> coin;
            // call avl insertion and get number of key comparisons
            int k_comp = avl.insert(key, val);
            // call skip list insertion and get number of key comparisons
            int k_comp2 = sklist.skinsert(key, val, coin);
            
            // an item is added into avl tree and skip list
            items++;
            
            std::cout << k_comp << " " << k_comp2 << std::endl;
        }
        else if (cmd == "d") {
            // the delete command
            std::cin >> key;
            // call avl deletion and get number of key comparisons
            int k_comp = avl.avldelete(key);
            // call skip list deletion and get number of key comparisons
            // before doing so record the number of nodes before deletion
            int old_item = sklist.getNum_node();
            int k_comp2 = sklist.skdelete(key);
            
            // the number of nodes is different the deletion was successful
            // then decrement the number of items
            if (sklist.getNum_node() != old_item) {
                items = items - 1;
            }
            
            std::cout << k_comp << " " << k_comp2 << std::endl;
        }
        else if (cmd == "savl") {
            // avl search command
            std::cin >> key;
            Node * n = avl.search(key);
            // check if the key is found
            if (n == nullptr) {
                // key not found
                std::cout << "ERROR" << " ";
            }
            else {
                // otherwise print value
                std::cout << n->value << " ";
            }
            // print number of key comparisons
            std::cout << avl.comp - 1 << std::endl;
        }
        else if (cmd == "ssl") {
            // skip list search command
            std::cin >> key;
            Sknode * n = sklist.sksearch(key);
            // check if the key is found
            if (n == nullptr) {
                // key not found
                std::cout << "ERROR" << " ";
            }
            else {
                // otherwise print value
                std::cout << n->value2 << " ";
            }
            // print number of key comparisons
            std::cout << sklist.comp2 - 1 << std::endl;
        }
        else if (cmd == "stats") {
            // print number of items currently in the avl tree and skip list, 
            // height of the avl tree and the number of nodes in the skip list
            std::cout << items << " " << avl.getHeight() << " " << sklist.getNum_node() << std::endl;
        }
        //else {}
    }
}
