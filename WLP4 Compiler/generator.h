#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <utility>
#include <map>

// a Node struct for building parse tree
struct Node {
    // these two components stores the type and lexeme for the current Node
    // if it stores a terminal, otherwise, the type component stores the
    // rule for the non-terminal, lexeme is the empty string
    std::string type;
    std::string lexeme;
    std::string data_type;
    
    // stores the children of the Node
    std::vector<Node *> children;
    
    // constructor and destructor
    Node(std::string type, std::string lexeme, std::string data_type, std::vector<Node *> children);
    ~Node();
};

Node::Node(std::string type, std::string lexeme, std::string data_type, std::vector<Node *> children)
    : type(type),
      lexeme(lexeme),
      data_type(data_type),
      children(children) {}
            
Node::~Node() {
    int childrensize = children.size();
    for (int i = 0; i < childrensize; i++) {
        delete children[i];
    }
    children.clear();
    children.shrink_to_fit();
}
/////////////////////////////////////////////////////////////////////////////////

// An exception class thrown when an error is encountered
class SemanError {
    std::string message;

  public:
    SemanError(std::string message);

    // Returns the message associated with the exception.
    const std::string &what() const;
};

// Failure class functions
SemanError::SemanError(std::string message) :
        message(std::move(message)) {}

// Returns the message associated with the exception.
const std::string & SemanError::what() const { return message; }

////////////////////////////////////////////////////////////////////////////////

// a function that counts the number of words in a sentence, parsing by spaces
int countWord(std::string sentence) {
    if (sentence.size() == 0) {
        return 0;
    }
    int num_words = 0;
    for (int i = 0; i < sentence.size(); i++) {
        // if current character is space reached end of a word, increment counter
        if (sentence[i] == ' ' && i != 0) {
            num_words = num_words + 1;
        }
    }
    return num_words + 1;
}

////////////////////////////////////////////////////////////////////////////////

// this function takes in a string token as parameter and return the position of
// the first space found in token
// a helper function for pre_to_tree, used to parse a token into type and lexeme
int parseToken(std::string token) {
    int pos = 0;
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == ' ') {
            pos = i + 1;
            i = token.size();
        }
    }
    return pos;
}

// given node, size of the node's children set, build
// the full children set of the node by reading from standard input
void pre_to_tree(Node * cur_node, int num) {
    int s = 0;
    std::string input;
    // continue to read input to build children set of the current node
    for (int i = 0; i < num; i++) {
        // read new line from derivation
        std::getline(std::cin, input);
        s = countWord(input);
        
        // children set of the child
        std::vector<Node *> c;
        Node * child_node = NULL;
        
        // if the input contains only two symbols and the first is upper case,
        // then it must be a token with a terminal and lexeme
        if (s == 2 && isupper(input[0])) {
            int pos = parseToken(input);
            child_node = new Node(input.substr(0, pos - 1), 
                                  input.substr(pos, input.size() - pos), "", c);
        }
        else {
            // input must be a rule, need to recurse on the child
            child_node = new Node(input, "", "", c);
            pre_to_tree(child_node, s - 1);
        }
        
        // add this child into cur_node's children set
        (cur_node->children).push_back(child_node);
    }
}

#endif