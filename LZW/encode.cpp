#include <iostream>
#include <vector>
#include<string>
#include <memory>
using namespace std;

// nodes for the trie dictionary
struct Trie_node {
    // the label of the node
    string label;
    // the code word stored at the node
    int code;
    // children of the current node, can have 26 at most, index i stores the child
    // labelled with the i-th character in the english alphabet, for example 0->a, 1->b, etc.
    vector<shared_ptr<Trie_node>> children;
    
    // constructor to simplify initialization
    Trie_node(string label, int code, vector<shared_ptr<Trie_node>> children);
    
    // destructor
    ~Trie_node();
};

// trie node constructor
Trie_node::Trie_node(string label, int code, vector<shared_ptr<Trie_node>> children)
    : label{label},
      code{code},
      children{children}
{}

// trie node destructor
Trie_node::~Trie_node() {
    children.clear();
    children.shrink_to_fit();
}

// trie dictionary for storing code words, initialized to
// store the lower case english alphabet with code words:
// 0=a, 1=b, ..., 25=z, and aa=26, ab=27,..., za=676,..., zz=701
struct Trie_dict {
    // root of the trie
    shared_ptr<Trie_node> root;
    
    // constructor
    Trie_dict(shared_ptr<Trie_node> root);
    // destructor
    ~Trie_dict();
    // trie insert
    void trie_insert(int code, string source, shared_ptr<Trie_node> n);
};

// constructor of the trie dictionary
Trie_dict::Trie_dict(shared_ptr<Trie_node> root)
    : root{root}
{}

// destructor of the trie dictionary
Trie_dict::~Trie_dict() {}

// simple implementation for converting lower case english alphabet
// to integers 0 to 25
int charint_conv(string c) {
    if (c == "a") return 0;
    else if (c == "b") return 1;
    else if (c == "c") return 2;
    else if (c == "d") return 3;
    else if (c == "e") return 4;
    else if (c == "f") return 5;
    else if (c == "g") return 6;
    else if (c == "h") return 7;
    else if (c == "i") return 8;
    else if (c == "j") return 9;
    else if (c == "k") return 10;
    else if (c == "l") return 11;
    else if (c == "m") return 12;
    else if (c == "n") return 13;
    else if (c == "o") return 14;
    else if (c == "p") return 15;
    else if (c == "q") return 16;
    else if (c == "r") return 17;
    else if (c == "s") return 18;
    else if (c == "t") return 19;
    else if (c == "u") return 20;
    else if (c == "v") return 21;
    else if (c == "w") return 22;
    else if (c == "x") return 23;
    else if (c == "y") return 24;
    else return 25;
}

// simple implementation for converting integers in the range [0,25]
// to the lower case english alphabet
string intchar_conv(int c) {
    if (c == 0) return "a";
    else if (c == 1) return "b";
    else if (c == 2) return "c";
    else if (c == 3) return "d";
    else if (c == 4) return "e";
    else if (c == 5) return "f";
    else if (c == 6) return "g";
    else if (c == 7) return "h";
    else if (c == 8) return "i";
    else if (c == 9) return "j";
    else if (c == 10) return "k";
    else if (c == 11) return "l";
    else if (c == 12) return "m";
    else if (c == 13) return "n";
    else if (c == 14) return "o";
    else if (c == 15) return "p";
    else if (c == 16) return "q";
    else if (c == 17) return "r";
    else if (c == 18) return "s";
    else if (c == 19) return "t";
    else if (c == 20) return "u";
    else if (c == 21) return "v";
    else if (c == 22) return "w";
    else if (c == 23) return "x";
    else if (c == 24) return "x";
    else return "z";
}

// trie insertion, where code is the new code word to be inserted and source
// is the source text of code, and n is the parent of the new code word
// this is a modified version of the insert for the purpose of the LZW algorithm
void Trie_dict::trie_insert(int code, string source, shared_ptr<Trie_node> n) {
    // get the integer representation of source
    int i = charint_conv(source);
    // create a node for the new code word, the children vector is initialized
    // to allocate 26 spaces for future children
    vector<shared_ptr<Trie_node>> empty_child;
    empty_child.resize(26);
    shared_ptr<Trie_node> new_n = make_shared<Trie_node> (source, code, empty_child);
    
    // attach the new code word into the dictionary
    (n.get()->children)[i] = new_n;
    
    return;
}

// convert num to binary representation in 12 bits and return
// the binary representation as a string
string string_to_bin(int num) {
    string str{""};
    while (num >= 1) {
        int remain = num % 2;
        if (remain == 0) {
            str.insert(0, "0");
        }
        else {
            str.insert(0, "1");
        }
        num = num / 2;
    }
    // expand the binary representation to 12 bits
    while (str.size() < 12) {
        str.insert(0, "0");
    }
    return str;
}

// the LZW encode function
void LZW_encode_modified(const string& s, string& coded_s) {
    // initialize the trie dictionary
    // create children vector for the root, contain lower case english alphabet
    // and corresponding code word
    vector<shared_ptr<Trie_node>> child;
    child.resize(26);
    for (int i = 0; i < 26; i++) {
        vector<shared_ptr<Trie_node>> sub_child;
        sub_child.resize(26);
        // create code word for aa, ab, ac, ..., az, ..., za, ..., zz
        for (int j = 0; j < 26; j++) {
            vector<shared_ptr<Trie_node>> sub_child2;
            sub_child2.resize(26);
            shared_ptr<Trie_node> c2 = make_shared<Trie_node> (intchar_conv(j), j + 26 + i*26, sub_child2);
            sub_child[j] = c2;
        }
        shared_ptr<Trie_node> c = make_shared<Trie_node> (intchar_conv(i), i, sub_child);
        child[i] = c;
    }
    // the root of the trie dictionary contains no useful information other than its
    // children vector, so label it with none and give it an invalid code word -1 to
    // indicate it is the root
    shared_ptr<Trie_node> r = make_shared<Trie_node> ("none", -1, child);
    Trie_dict d{r};
    
    // after initializing the trie dictionary, the next available code word is 702
    int idx = 702;
    
    int k = 0;
    while (k < s.size()) {
        shared_ptr<Trie_node> v = d.root;
        // substring length
        int l = k;
        // find the longest substring of s currently stored in the trie dictionary
        while (l < s.size() && v.get() != nullptr && 
        (v.get()->children)[charint_conv(s.substr(l, 1))] != nullptr) {
            v = (v.get()->children)[charint_conv(s.substr(l,1))];
            l++;
        }
        // add the code word of the current character to the coded text
        int code_word = v.get()->code;
        coded_s.append(string_to_bin(code_word));
        
        // add new node into the trie dictionary
        if (k < s.size() && l < s.size()) {
            d.trie_insert(idx, s.substr(l,1), v);
            idx++;
            if (l + 1 < s.size()) {
                d.trie_insert(idx, s.substr(l+1, 1), (v.get()->children)[charint_conv(s.substr(l,1))]);
                idx++;
            }
        }
        k = l;
    }
}


int main()
{  
    string s;
    string coded_s = "";
    getline(cin, s);

    LZW_encode_modified(s, coded_s);

    cout << coded_s;
}
