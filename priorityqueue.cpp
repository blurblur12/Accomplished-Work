#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <queue>

// 3 Priority Queue implementations
// implement 3 realizations of priority queue using linked-list,
// max-heap, and vectors of queues

// implementation 1, uses linked list
// runtime of insert:
// O(n), allocating space to define the new node and inserting the node
// takes constant time, so the runtime depends on the while loop, in the
// worst case the new node that we are inserting has the lowest priority
// then we need to traverse the whole linked-list, which takes O(n)
// runtime of deleteMax:
// O(1), there is no loop, assigning values to variables takes constant time
// and delete takes constant time since we broke the link between the node
// that we are deleting and the rest of the linked list
// runtime of lookMax:
// O(1), simply returning the pair of priority and timestamp values of the
// front node
// space acquired:
// the space acquired for insert is O(1), we will always be allocating
// space for one new node
// there is no space being allocated for deleteMax and lookMax
struct Node {
        // stores the priority timestamp pair where the first value is
        // the priority and the second value is the timestamp
        std::pair<int, int> n;
        // link to the next node
        Node *next;

        // Constructor and Destructor
        Node(std::pair<int, int> n, Node *next = nullptr);
        ~Node();
};

// Normal constructor
Node::Node(std::pair<int, int> n, Node *next) : n{n}, next{next} {}

// Destructor
Node::~Node() {
        delete next;
}

// inserts a new node into the priority queque such that the node
// with the highest priority is at the front
Node *insert1(Node *pq, int prio, int val) {
        // create the new node
        std::pair<int, int> pairs = std::make_pair(prio, val);
        Node *new_n = new Node{pairs, nullptr};
        
        // if current node is a nullptr, set pq = new_n and return
        if (pq == nullptr) {
                pq = new_n;
                return pq;
        }

        Node *cur = pq;
        // if the new node has higher priority than front, then insert it at the front
        if (prio > (cur->n).first) {
                new_n->next = cur;
                pq = new_n;
                return pq;
        }
        
        // if there is only one node, simply attach the new node to it
        if (cur->next == nullptr) {
                cur->next = new_n;
                return cur;
        }

        // otherwise find the right position to insert the new node
        while ((cur->n).first >= prio) {
                if (cur->next == nullptr) {
                        break;
                }
                if ((cur->n).first == prio) {
                        break;
                }
                cur = cur->next;
        }
        // insert the new node at the position found
        new_n->next = cur->next;
        cur->next = new_n;
        return pq;
}

// delete the front node, which is the node with highest priority
// and return its information
std::pair<int, int> deleteMax1(Node *pq) {
        Node *temp = pq;
        // here suppose to set pq to pq next but for some reason the pq pointer
        // does not modify in the main function, so need to do the delete in main
        // make the second highest priority the front
        //pq = pq->next;
        // temp->next = nullptr
        std::pair<int, int> pair = temp->n;
        //delete temp;
        return pair;
}

// look up the information of the node with the highest priority
std::pair<int, int> lookMax1(Node *pq) {
        return pq->n;
}


// implementation 2, uses max-heap
// define size to be the capacity of the dynamic array
// runtime of insert:
// within insert we called heapify which calls fix-down, the implementation of
// heapify and fix-down follows the pseudo code given in class thus heapify has
// runtime O(n), and within insert there is a loop for going through the heap
// and copying items into the newly allocated array, which also has runtime O(n),
// so overall insert has runtime O(2n) = O(n)
// runtime of deleteMax:
// deleteMax uses the fix-down function which has runtime O(log n) has given
// in lecture slides, and deleteMax, in the worst case, needs to resize the
// array, which takes O(n) time to copy items from heap to the newly allocated
// array, so overall, runtime is O(log n + n) = O(n)
// runtime of lookMax:
// simply returning the root, so O(1) as desired
// space acquired:
// insert in the worst case needs to acquire 2*size more space, however, we
// only allocate new space when n = size, so we are allocating 2n more space,
// so space allocated by insert is O(2n) = O(n)
// deleteMax in the worst case allocated 2n more space to resize the array,
// so space acquired is O(2n) = O(n)
// no space being allocated in lookMax
struct Maxheap {
        // keep track of size of array
        int size;
        // keep track of length used
        int length;
        // the heap tree stored as dynamic array
        std::pair<int, int> *heap;

        // Constructor and Destructor
        Maxheap(int size, int length, std::pair<int, int> *heap = nullptr);
        ~Maxheap();

        // fix-down function
        void fixDown(int i);
        // heapify function
        void heapify();
        // inserts a new node into the tree
        void insert2(int val, int prio);
        // delete the root
        std::pair<int, int> deleteMax2();
        // look up the root
        std::pair<int, int> lookMax2();
};

// Normal constructor
Maxheap::Maxheap(int size = 0, int length = 0, std::pair<int, int> *heap)
        : size{size},
          length{length},
          heap{heap}
{}

// Destructor
Maxheap::~Maxheap() {
        delete[] heap;
}

// fix-down function
void Maxheap::fixDown(int i) {
        // loop while the i-th item in the arry has a child
        while (2*i + 1 <= length - 1) {
                // left child of i
                int j = 2*i + 1;
                // check which child has the larger key if the right child exists
                if (2*i + 2 <= length - 1 && heap[2*i + 2].first > heap[j].first) {
                        j = 2*i + 2;
                }
                // if the i-th item already has greater priority than its child,
                // heap property satisfied break
                if (heap[i].first >= heap[j].first) {
                    break;
                }

                // otherwise swap position of i and its child j
                std::pair<int, int> temp = heap[i];
                heap[i] = heap[j];
                heap[j] = temp;
                i = j;
        }
}

// the heapify function, heapifies the tree
void Maxheap::heapify() {
        int parent = (length - 1)/2;
        // start at the parent of last, fix down all the way to root
        for (int i = parent; i >= 0; i--) {
                fixDown(i);
        }
}

// insert a new node into the tree
void Maxheap::insert2(int val, int prio) {
        // if the array is empty, initialize the size to 1
        if (size == 0) {
                heap = new std::pair<int, int>{prio, val};
                size += 1;
                length += 1;
                return;
        }

        // otherwise double the size if already reached the end of array
        if (length == size) {
                // reallocate space for new array with double the size
                std::pair<int, int> *temp = new std::pair<int, int>[size * 2];
                // move items to the new array
                for (int i = 0; i < length; i++) {
                        temp[i] = heap[i];
                }
                delete[] heap;
                heap = temp;
                // add the new node at the end of heap
                std::pair<int, int> new_n = std::make_pair(prio, val);
                temp[length] = new_n;
                size = size * 2;
        }
        else {
                // in this case current size is big enough to add a new node
                std::pair<int, int> new_n = std::make_pair(prio, val);
                heap[length] = new_n;
        }

        length += 1;
        // heapify the heap to resume max-heap properties
        heapify();
}

// delete the root and return its information
std::pair<int, int> Maxheap::deleteMax2() {
        int last = length - 1;
        // swap root and last
        std::pair<int, int> temp = heap[0];
        heap[0] = heap[last];
        heap[last] = temp;

        // fix-down to resume max-heap properties
        length -= 1;
        fixDown(0);

        // check if less than or equal to 25% of the array
        // contains items
        if ((length * 100) / size <= 25) {
                std::pair<int, int> *temp2 = new std::pair<int, int>[length * 2];
                for (int i = 0; i < length; i++) {
                        temp2[i] = heap[i];
                }

                // delete the old heap and keep the root that we want to delete
                temp = heap[length];
                delete[] heap;
                heap = temp2;
                size = length * 2;
        }

        return temp;
}

// returns the root of the heap, which is the node with highest
// priority
std::pair<int, int> Maxheap::lookMax2() {
        return heap[0];
}


// implementation 3, uses vector of queues
// runtime of insert:
// std::queue has O(1) complexity for push, and std::vector has O(1)
// complexity for push_back, O(n) complexity for resize so overall, insert
// has runtime O(n)
// runtime of deleteMax:
// there is no loop, every function of std::vector and std::queue called here
// has constant runtime, so overall runtime is O(1)
// runtime of lookMax:
// constant O(1);
// space acquired:
// insert in the worst case needs to expand the vector by one, and acquire one
// space to add an element in the queue, so total 2 spaces are acquired, so
// O(2) = O(1)
// deleteMax and lookMax do not acquire new space
struct Prique {
        // current highest priority stored note that this will be one
        // larger than the current size of the vector
        int highest;
        // a vector of queues such that each queue contains items of same priority
        std::vector<std::queue<int>> vq;

        // Constructor and Destructor
        Prique(int highest, std::vector<std::queue<int>> vq);
        ~Prique();
        // inserts a new item into the vector of queues
        void insert3(int prio, int val);
        // delete the item with highest priority and return it
        std::pair<int, int> deleteMax3();
        // look up the item with highest priority
        std::pair<int, int> lookMax3();
};

// Normal constructor
Prique::Prique(int highest, std::vector<std::queue<int>> vq) : highest{highest}, vq{vq} {}

// Destructor
Prique::~Prique() {
        for (int i = 0; i < vq.size(); i++) {
                for (int j = 0; j < vq[i].size(); j++) {
                        vq[i].pop();
                }
        }
        vq.clear();
        vq.shrink_to_fit();
}

// insert a new item into the correct queue in the vector
void Prique::insert3(int prio, int val) {
        // if the priority already exists, enqueue the new item into
        // the corresponding queue
        if (prio <= highest) {
                // enqueue the new item's timestamp
                vq[prio].push(val);
                return;
        }

        // otherwise need to resize the vector to be prio then push back
        // a new queue to the end making the size of the vector to be prio + 1
        // now prio is the highest priority
        vq.resize(prio);
        vq.push_back(std::queue<int>());
        highest = prio;
        // enqueue the new item's timestamp
        vq[prio].push(val);
}

// delete the first item in the highest priority queue and return its information
std::pair<int, int> Prique::deleteMax3() {
        // extract information of the deleted item
        int val = vq[highest].front();
        std::pair<int, int> temp = std::make_pair(highest, val);
        // delete the item
        vq[highest].pop();
        // if this priority has no more items in it remove this queue
        if (vq[highest].size() == 0) {
                vq.pop_back();
                highest = highest - 1;
        }

        return temp;
}

// look up the very first item in the highest priority queue in the vector
// and return its information
std::pair<int, int> Prique::lookMax3() {
        int val = vq[highest].front();
        return std::make_pair(highest, val);
}


// a function for printing the priority value pair
void printPair(std::ostream &out, std::pair<int, int> p) {
        out << p.first << " " << p.second << std::endl;
}

int main()
{
        // priority queue 1
        int priq1_size = 0;
        Node *priq1 = nullptr;
        // priority queue 2
        Maxheap priq2{0,0,nullptr};
        // priority queue 3
        std::vector<std::queue<int>> vec;
        Prique priq3{-1, vec};

        std::string cmd;
        std::string option;
        // keep track of timestamp
        int count = 0;
        while (std::cin >> cmd) {
                // initializes or resets all priority queues to be empty
                if (cmd == "r") {
                        if (priq1_size != 0) {
                                // reset priority queue 1
                                delete priq1;
                                priq1_size = 0;
                                priq1 = nullptr;
                        }
                        // reset priority queue 2
                        if (priq2.size != 0) {
                                delete[] priq2.heap;
                                priq2.size = 0;
                                priq2.length = 0;
                                priq2.heap = nullptr;
                        }
                        // reset priority queue 3
                        for (int i = 0; i < (priq3.vq).size(); i++) {
                                for (int j = 0; j < (priq3.vq)[i].size(); j++) {
                                        (priq3.vq)[i].pop();
                                }
                        }
                        (priq3.vq).clear();
                        (priq3.vq).shrink_to_fit();
                        priq3.highest = -1;
                }
                else if (cmd == "i") {
                        // insert is called
                        std::cin >> option;
                        int priority;
                        std::cin >> priority;
                        count += 1;
                        if (option == "1") {
                                priq1 = insert1(priq1, priority, count);
                                priq1_size += 1;
                        }
                        else if (option == "2") {
                                priq2.insert2(count, priority);
                        }
                        else {
                                priq3.insert3(priority, count);
                        }
                }
                else if (cmd == "d") {
                        // delete max is called
                        std::cin >> option;
                        std::pair<int, int> max;
                        if (option == "1" && priq1 != nullptr) {
                                max = deleteMax1(priq1);
                                Node *tmp = priq1;
                                priq1 = priq1->next;
                                tmp->next = nullptr;
                                delete tmp;
                                printPair(std::cout, max);
                        }
                        else if (option == "2" && priq2.length != 0) {
                                max = priq2.deleteMax2();
                                printPair(std::cout, max);
                        }
                        else if (option == "3" && priq3.highest != -1){
                                max = priq3.deleteMax3();
                                printPair(std::cout, max);
                        }
                        // if the priority queque indicated is empty do nothing
                        else {}
                }
                else if (cmd == "l") {
                        // calls lookMax
                        std::cin >> option;
                        std::pair<int, int> max;
                        if (option == "1" && priq1 != nullptr) {
                                max = lookMax1(priq1);
                                printPair(std::cout, max);
                        }
                        else if (option == "2" && priq2.length != 0) {
                                max = priq2.lookMax2();
                                printPair(std::cout, max);
                        }
                        else if (option == "3" && priq3.highest != -1){
                                max = priq3.lookMax3();
                                printPair(std::cout, max);
                        }
                        // if the priority queque indicated is empty do nothing
                        else {}
                }
                else if (cmd == "x") {
                        // terminates the program
                        delete priq1;
                        break;
                }
                else {
                        std::cerr << "Invalid command." << std::endl;
                }
        }
}