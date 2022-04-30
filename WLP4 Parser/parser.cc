#include <iostream>
#include <sstream>
#include <stack>
#include "parser.h"
using std::string;
using std::istream;
using std::ostream;

// Rule constructor and destructor
Rule::Rule(std::string lhs, std::vector<std::string> rhs) : lhs(lhs), rhs(rhs) {}

Rule::~Rule() {
    rhs.clear();
    rhs.shrink_to_fit();
}

// State constructor and destructor
State::State(bool reducible, std::map<std::string, int> redutrans, 
          std::map<std::string, int> shifttrans) :
    reducible(reducible), redutrans(redutrans), shifttrans(shifttrans) {}

State::~State() {
    redutrans.clear();
    shifttrans.clear();
}

// Failure class functions
ParsingFailure::ParsingFailure(std::string message, int position) :
        message(std::move(message)), position(position) {}

// Returns the message associated with the exception.
const std::string & ParsingFailure::what() const { return message; }
    
const int ParsingFailure::pos() const { return position; }

// a function that breaks a phrase into words, meaning breaking
// into subtrings by white spaces
void sentenceToWord(std::vector<std::string> &words, std::string sentence) {
    std::string str;
    // the first word of sentence, want to attach it at the back of the vector
    // as it will be the LHS of the production rule, we don't want it in the
    // vector of RHS symbols
    std::string frontword;
    int indx = 0;
    for (auto s : sentence) {
        // if current character is space, skip an advance vector index
        if (s == ' ') {
            if (indx != 0) {
                words.push_back(str);
            }
            else {
                frontword = str;
            }
            str.erase();
            indx += 1;
        }
        else {
            // current character is not space append to str
            str = str + s;
        }
    }
    words.push_back(str);
    words.push_back(frontword);
}

// Given vector myrules, and the size of the vector, read from input and store
// the production rules in the input vector
void storeRules(std::vector<Rule> &myrules, std::string allrules) {
    std::istringstream iss;
    iss.str(allrules);
    std::string line;
    // read in size number of lines
    for (int i = 0; i < 49; i++) {
        getline(iss, line);
        
        std::vector<std::string> righthand; // vector of RHS
        sentenceToWord(righthand, line);
        if (righthand.size() == 2 && righthand[0] == "") {
            Rule currule{righthand[1], righthand};
            (currule.rhs).pop_back();
            (currule.rhs).pop_back();
            myrules.push_back(currule);
        }
        else if (righthand.size() == 2 && righthand[1] == "") {
            Rule currule{righthand[0], righthand};
            (currule.rhs).pop_back();
            (currule.rhs).pop_back();
            myrules.push_back(currule);
        }
        else {
            Rule currule{righthand[righthand.size()-1], righthand}; // create rule
            (currule.rhs).pop_back();
            myrules.push_back(currule);
        }
        
    }
}

// Given vector mystates, and the size of the vector, read from input and
// store the dfa transitions in mystates
void storeTransit(std::vector<State *> &mystates, std::string alltransits) {
    std::string line;
    std::istringstream iss;
    iss.str(alltransits);
    // read in size number of lines
    for (int i = 0; i < 863; i++) {
        getline(iss, line);
        
        std::vector<std::string> transition;
        sentenceToWord(transition, line);
        // note that every transition must be size 4, where the first
        // item is the symbol, second is either reduce or shift, and
        // third is either a rule number or state number, last item
        // is the state number transiting from
        int cur_state = std::stoi(transition[3]);
        if (mystates[cur_state] == NULL) {
            std::map<std::string, int> cur_transit;
            cur_transit[transition[0]] = std::stoi(transition[2]);
            std::map<std::string, int> empty_transit;
            if (transition[1] == "reduce") {
                State * reducestate = new State(true, cur_transit, empty_transit);
                mystates[cur_state] = reducestate;
            }
            else {
                State * shiftstate= new State(false, empty_transit, cur_transit);
                mystates[cur_state] = shiftstate;
            }
        }
        else {
            if (transition[1] == "reduce") {
                mystates[cur_state]->reducible = true;
                mystates[cur_state]->redutrans[transition[0]] = std::stoi(transition[2]);
            }
            mystates[cur_state]->shifttrans[transition[0]] = std::stoi(transition[2]);
        }
    }
}

// given a state, the next input symbol, returns the rule index
// of the rule to be used to reduce if the state
// is reducible, otherwise return -1
int Reduce(State *topstate, std::string next_sym) {
    // if the state is a reduce state check if the next
    // input symbol is a transitable symbol
    if (topstate->reducible == true) {
        if ((topstate->redutrans).count(next_sym) != 0) {
            return (topstate->redutrans)[next_sym];
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

Token::Token(std::string kind, std::string lexeme) : kind(kind), lexeme(lexeme) {}

Node::Node(std::string val, std::string lex, Node * parent, std::vector<Node *> children)
    : val(val),
      lex(lex),
      parent(parent),
      children(children) {}
            
Node::~Node() {
    int childrensize = children.size();
    for (int i = 0; i < childrensize; i++) {
        delete children[i];
    }
    children.clear();
    children.shrink_to_fit();
}

// print the tree rooted at curnode through preorder traversing
void preTraverse(Node * curnode) {
    // note that terminals must be leaves of the tree, so if a
    // node has children it must be a non-terminal, then print
    // the rule associated with the non-terminal
    std::cout << curnode->val;
    if ((curnode->children).size() != 0) {
        // print from right to left as when we were popping from the stack, order
        // of the reduction rule is reversed
        for (int i = (curnode->children).size() - 1; i >= 0 ; i--) {
            std::cout << ' ' << (curnode->children[i])->val;
        }
        std::cout << std::endl;
        // need to recurse on the children from right to left as when we were
        //  popping from the stack, the top of the stack should be the righmost
        // symbol
        for (int j = (curnode->children).size() - 1; j >= 0 ; j--) {
            preTraverse(curnode->children[j]);
        }
    }
    else {
        // current node is a leaf print the token kind and lexeme
        std::cout << ' ' << curnode->lex << std::endl;
    }
}

void parseAlgo(std::stack<State *> &stateStack, std::stack<Node *> &symStack, std::vector<Rule> &productrule,
                std::vector<State *> &dfastates, std::vector<Token> &input) {
    int stateindex = 0;
    stateStack.push(dfastates[0]);
    for (int i = 0; i < input.size(); i++) {
        // skip indicates which rule should be used to reduce
        int skip = Reduce(stateStack.top(), input[i].kind);
        while (skip != -1) {
            // create a new node for the LHS of the reduction rule
            std::vector<Node *> child;
            Node * newtopsym = new Node(productrule[skip].lhs, "", NULL, child);
            // pop n items from both stacks, where n is the size of the
            // RHS of the reduction rule, push the popped Nodes
            // into the children vector of the new top Node
            for (int j = 0; j < (productrule[skip].rhs).size(); j++) {
                (symStack.top())->parent = newtopsym;
                newtopsym->children.push_back(symStack.top());
                symStack.pop();
                stateStack.pop();
            }
            symStack.push(newtopsym);
            stateindex = (stateStack.top())->shifttrans[productrule[skip].lhs];
            stateStack.push(dfastates[stateindex]);
            
            skip = Reduce(stateStack.top(), input[i].kind);
        }
        // shift an unread Token, create a node for it and push onto stack
        std::vector<Node *> child2;
        Node * newshiftsym = new Node(input[i].kind, input[i].lexeme, NULL, child2);
        symStack.push(newshiftsym);
        // if there is no valid transition for the next symbol, report error
        if (((stateStack.top())->shifttrans).count(input[i].kind) == 0) {
            productrule.clear();
            productrule.shrink_to_fit();
            int dfasize = dfastates.size();
            for (int i = 0; i < dfasize; i++) {
                delete dfastates[i];
            }
            dfastates.clear();
            dfastates.shrink_to_fit();
            input.clear();
            int symstacksize = symStack.size();
            for (int i = 0; i < symstacksize; i++) {
                delete symStack.top();
                symStack.pop();
            }
            throw ParsingFailure("ERROR at ", i);
        }
        stateindex = (stateStack.top())->shifttrans[input[i].kind];
        stateStack.push(dfastates[stateindex]);
    }
    
    // ran through the whole input without throwing error, thus it is in the
    // language, at this point symStack should store Node for BOF, subtree,
    // and Node for EOF
    // add rule 0 into the parse tree
    std::vector<Node *> child3;
    Node * root = new Node(productrule[0].lhs, "", NULL, child3);
    for (int i = 0; i < 3; i++) {
        (symStack.top())->parent = root;
        (root->children).push_back(symStack.top());
        symStack.pop();
    }
    
    // at this point we have the full parse tree, print via pre-order traversing
    // from right to left
    preTraverse(root);
    delete root;
}
