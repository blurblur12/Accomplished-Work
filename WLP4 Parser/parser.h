#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <vector>

// a class for production rules
struct Rule {
    // LHS of the rule, a single symbol
    std::string lhs;
    // RHS of the rule, a vector of symbols
    std::vector<std::string> rhs;
    
    // constructor and destructor
    Rule(std::string lhs, std::vector<std::string> rhs);
    ~Rule();
};

// a class for states of the LR(1) DFA
struct State {
    // an indicator of whether the state is a reduce state
    // true if yes otherwise no 
    bool reducible;
    
    // transitions associated with the state stored as a map where
    // the key value pair is <terminal, rule> for a reduce state,
    // and is <symbol, state> for a shift state.
    std::map<std::string, int> redutrans;
    std::map<std::string, int> shifttrans;
    
    // constructor and destructor
    State(bool reducible, std::map<std::string, int> redutrans, 
          std::map<std::string, int> shifttrans);
    ~State();
};

// An exception class thrown when an error is encountered while scanning.
class ParsingFailure {
    std::string message;
    int position;

  public:
    ParsingFailure(std::string message, int position);

    // Returns the message associated with the exception.
    const std::string &what() const;
    
    const int pos() const;
};

void storeRules(std::vector<Rule> &myrules, std::string allrules);
void storeTransit(std::vector<State *> &mystates, std::string alltransits);

struct Token {
    std::string kind;
    std::string lexeme;
    
    Token(std::string kind, std::string lexeme);
};

struct Node {
    // here value is the symbol being stored in this node, if the symbol
    // is a terminal, the lex component will contain the lexeme
    // if the symbol is a non-terminal it will only have a valid val component
    // which stores the non-terminal symbol, the lexeme will be the empty string
    std::string val;
    std::string lex;
    // the parent node
    Node * parent;
    // this vector stores all children of the node
    std::vector<Node *> children;
    
    Node(std::string val, std::string lex, Node * parent, std::vector<Node *> children);
    ~Node();
};

void parseAlgo(std::stack<State *> &stateStack, std::stack<Node *> &symStack, std::vector<Rule> &productrule,
                std::vector<State *> &dfastates, std::vector<Token> &input);

#endif
