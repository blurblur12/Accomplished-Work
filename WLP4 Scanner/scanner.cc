#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <utility>
#include <set>
#include <array>
#include "scanner.h"

// Token constructor
Token::Token(Token::Kind kind, std::string lexeme):
  kind(kind), lexeme(std::move(lexeme)) {}

// kind and lexeme getter
Token::Kind Token::getKind() const { return kind; }
const std::string &Token::getLexeme() const { return lexeme; }

// Prints a string representation of a token, so it prints the token Kind
//  and the lexeme string
std::ostream &operator<<(std::ostream &out, const Token &tok) {
  switch (tok.getKind()) {
    case Token::ID:         out << "ID";         break;
    case Token::NUM:        out << "NUM";      break;
    case Token::LPAREN:     out << "LPAREN";     break;
    case Token::RPAREN:     out << "RPAREN";     break;
    case Token::LBRACE:     out << "LBRACE";     break;
    case Token::RBRACE:     out << "RBRACE";     break;
    case Token::RETURN:     out << "RETURN";     break;
    case Token::IF:         out << "IF";         break;
    case Token::ELSE:       out << "ELSE";       break;
    case Token::WHILE:      out << "WHILE";      break;
    case Token::PRINTLN:    out << "PRINTLN";    break;
    case Token::WAIN:       out << "WAIN";       break;
    case Token::BECOMES:    out << "BECOMES";    break;
    case Token::INT:        out << "INT";        break;
    case Token::EQ:         out << "EQ";         break;
    case Token::NE:         out << "NE";         break;
    case Token::LT:         out << "LT";         break;
    case Token::GT:         out << "GT";         break;
    case Token::LE:         out << "LE";         break;
    case Token::GE:         out << "GE";         break;
    case Token::PLUS:       out << "PLUS";       break;
    case Token::MINUS:      out << "MINUS";      break;
    case Token::STAR:       out << "STAR";       break;
    case Token::SLASH:      out << "SLASH";      break;
    case Token::PCT:        out << "PCT";        break;
    case Token::COMMA:      out << "COMMA";      break;
    case Token::SEMI:       out << "SEMI";       break;
    case Token::NEW:        out << "NEW";        break;
    case Token::DELETE:     out << "DELETE";     break;
    case Token::LBRACK:     out << "LBRACK";     break;
    case Token::RBRACK:     out << "RBRACK";     break;
    case Token::AMP:        out << "AMP";        break;
    case Token::NUL:        out << "NULL";        break;
    case Token::SPACE:      out << "SPACE";      break;
    case Token::COMMENT:    out << "COMMENT";    break;
  }
  out << " " << tok.getLexeme();

  return out;
}


// Prints an error message when tokenization fails
ScanningFailure::ScanningFailure(std::string message):
  message(std::move(message)) {}

const std::string &ScanningFailure::what() const { return message; }

// converts NUM lexeme to integer
int toNumber(std::string num_lexeme) {
    std::istringstream iss;
    int result;
    iss.str(num_lexeme);
    iss >> result;
    return result;
}

// Representation of a DFA, used to handle the scanning process.
class WlpDFA {
  public:
    enum State {
      // All states of the DFA
      WHITESPACE = Token::SPACE,
      SLASH,
      COMMENT,
      START,
      ZERO,
      NUMS,
      SINGLECHAR,
      NEG,
      CONDIT1,
      CONDIT2,
      IDKEY,
      DIGIT,
      FAIL,

      // Hack to let this be used easily in arrays. This should always be the
      // final element in the enum, and should always point to the previous
      // element.

      LARGEST_STATE = FAIL
    };

  private:
    // A set of all accepting states for the DFA.
    std::set<State> acceptingStates;

    // The transition function for the DFA, stored as array of arrays
    // since the total number of states is equal to LARGEST_STATE + 1
    // outter array is of that size; the inner arrays stores the transitions
    // associated to the states
    std::array<std::array<State, 128>, LARGEST_STATE + 1> transitionFunction;

    // Converts a state to a kind to allow construction of Tokens from States.
    // Throws an exception if conversion is not possible.
    Token::Kind stateToKind(State s, std::string input) const {
        // if state is ZERO, the lexeme must be 0, so token kind is NUM
        // if state is DIGIT, the lexeme must be an ID with ending digits
      switch(s) {
        case ZERO:       return Token::NUM;
        case DIGIT:      return Token::ID;
        case WHITESPACE: return Token::SPACE;
        case SLASH:      return Token::SLASH;
        case COMMENT:    return Token::COMMENT;
      }
      // in this case the input must be a valid single character
      if (s == SINGLECHAR) {
          if (input == "(") {
              return Token::LPAREN;
          }
          else if (input == ")") {
              return Token::RPAREN;
          }
          else if (input == "{") {
              return Token::LBRACE;
          }
          else if (input == "}") {
              return Token::RBRACE;
          }
          else if (input == "+") {
              return Token::PLUS;
          }
          else if (input == "-") {
              return Token::MINUS;
          }
          else if (input == "*") {
              return Token::STAR;
          }
          else if (input == "%") {
              return Token::PCT;
          }
          else if (input == ";") {
              return Token::SEMI;
          }
          else if (input == ",") {
              return Token::COMMA;
          }
          else if (input == "[") {
              return Token::LBRACK;
          }
          else if (input == "]") {
              return Token::RBRACK;
          }
          else if (input == "&") {
              return Token::AMP;
          }
          else {
              // should never occur
          }
      }
      else if (s == CONDIT1) {
          // the first condition state, must be one of =, <, >
          if (input == "=") {
              return Token::BECOMES;
          }
          else if (input == "<") {
              return Token::LT;
          }
          else if (input == ">") {
              return Token::GT;
          }
          else {}
      }
      else if (s == CONDIT2) {
          // the second condition state, must be one of ==, <=, >=, !=
          if (input == "==") {
              return Token::EQ;
          }
          else if (input == "<=") {
              return Token::LE;
          }
          else if (input == ">=") {
              return Token::GE;
          }
          else if (input == "!=") {
              return Token::NE;
          }
          else {}
      }
      else if (s == NUMS) {
          // numerical input, must be NUM, but need to check if within range
          int value = toNumber(input);
          if (value <= 2147483647) {
              return Token::NUM;
          }
          else {
              throw ScanningFailure("ERROR: Cannot convert state to kind.");
          }
      }
      else if (s == IDKEY) {
          // might be an ID, or a keyword
          // if it's a keyword, must be one of wain, int, if, else, while, println
          // return, NULL, new, or delete
          if (input == "wain") {
              return Token::WAIN;
          }
          else if (input == "int") {
              return Token::INT;
          }
          else if (input == "if") {
              return Token::IF;
          }
          else if (input == "else") {
              return Token::ELSE;
          }
          else if (input == "while") {
              return Token::WHILE;
          }
          else if (input == "println") {
              return Token::PRINTLN;
          }
          else if (input == "return") {
              return Token::RETURN;
          }
          else if (input == "NULL") {
              return Token::NUL;
          }
          else if (input == "new") {
              return Token::NEW;
          }
          else if (input == "delete") {
              return Token::DELETE;
          }
          else {
              // if not one of the keywords, must be an identifier
              return Token::ID;
          }
      }
      else {
          throw ScanningFailure("ERROR: Cannot convert state to kind.");
      }
    }

  public:
    // Tokenizes an input string according to the Simplified Maximal Munch
    // scanning algorithm.
    std::vector<Token> simplifiedMaximalMunch(const std::string &input) const {
      std::vector<Token> result;

      State state = start();
      std::string munchedInput;

      // We can't use a range-based for loop effectively here
      // since the iterator doesn't always increment.
      for (std::string::const_iterator inputPosn = input.begin();
           inputPosn != input.end();) {

        State oldState = state;
        state = transition(state, *inputPosn);

        if (!failed(state)) {
          munchedInput += *inputPosn;
          oldState = state;

          ++inputPosn;
        }

        if (inputPosn == input.end() || failed(state)) {
          if (accept(oldState)) {
            result.push_back(Token(stateToKind(oldState, munchedInput), munchedInput));

            munchedInput = "";
            state = start();
          } else {
            if (failed(state)) {
              munchedInput += *inputPosn;
            }
            throw ScanningFailure("ERROR: Simplified maximal munch failed on input: "
                                 + munchedInput);
          }
        }
      }

      return result;
    }

    // Initializes the accepting states for the DFA.
    WlpDFA() {
      acceptingStates = {ZERO, NUMS, SINGLECHAR, CONDIT1, CONDIT2, IDKEY, DIGIT,
                         WHITESPACE, SLASH, COMMENT};
      //Non-accepting states are START and NEG

      // Initialize transitions for the DFA
      for (size_t i = 0; i < transitionFunction.size(); ++i) {
        for (size_t j = 0; j < transitionFunction[0].size(); ++j) {
          transitionFunction[i][j] = FAIL;
        }
      }

      registerTransition(START, "0", ZERO);
      registerTransition(START, "123456789", NUMS);
      registerTransition(NUMS, "0123456789", NUMS);
      registerTransition(START, "abcdefghijklmnopqrstuvwxyz", IDKEY);
      registerTransition(START, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", IDKEY);
      registerTransition(IDKEY, "abcdefghijklmnopqrstuvwxyz", IDKEY);
      registerTransition(IDKEY, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", IDKEY);
      registerTransition(IDKEY, "0123456789", DIGIT);
      registerTransition(DIGIT, "abcdefghijklmnopqrstuvwxyz", IDKEY);
      registerTransition(DIGIT, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", IDKEY);
      registerTransition(DIGIT, "0123456789", DIGIT);
      registerTransition(START, "!", NEG);
      registerTransition(NEG, "=", CONDIT2);
      registerTransition(START, "=", CONDIT1);
      registerTransition(START, ">", CONDIT1);
      registerTransition(START, "<", CONDIT1);
      registerTransition(CONDIT1, "=", CONDIT2);
      registerTransition(START, "(", SINGLECHAR);
      registerTransition(START, ")", SINGLECHAR);
      registerTransition(START, "{", SINGLECHAR);
      registerTransition(START, "}", SINGLECHAR);
      registerTransition(START, "+", SINGLECHAR);
      registerTransition(START, "-", SINGLECHAR);
      registerTransition(START, "*", SINGLECHAR);
      registerTransition(START, "%", SINGLECHAR);
      registerTransition(START, ";", SINGLECHAR);
      registerTransition(START, ",", SINGLECHAR);
      registerTransition(START, "[", SINGLECHAR);
      registerTransition(START, "]", SINGLECHAR);
      registerTransition(START, "&", SINGLECHAR);
      registerTransition(START, isspace, WHITESPACE);
      registerTransition(WHITESPACE, isspace, WHITESPACE);
      registerTransition(START, "/", SLASH);
      registerTransition(SLASH, "/", COMMENT);
      registerTransition(COMMENT, [](int c) -> int { return c != '\n'; },
          COMMENT);
    }

    // Register a transition on all chars in chars
    void registerTransition(State oldState, const std::string &chars,
        State newState) {
      for (char c : chars) {
        transitionFunction[oldState][c] = newState;
      }
    }
    
    // Register a transition on all chars matching test
    // For some reason the cctype functions all use ints, hence the function
    // argument type.
    void registerTransition(State oldState, int (*test)(int), State newState) {
      for (int c = 0; c < 128; ++c) {
        if (test(c)) {
          transitionFunction[oldState][c] = newState;
        }
      }
    }

    // Returns the state corresponding to following a transition
    // from the given starting state on the given character,
    // or a special fail state if the transition does not exist.
    State transition(State state, char nextChar) const {
      return transitionFunction[state][nextChar];
    }

    // Checks whether the state returned by transition
    // corresponds to failure to transition.
    bool failed(State state) const { return state == FAIL; }

    // Checks whether the state returned by transition
    // is an accepting state.
    bool accept(State state) const {
      return acceptingStates.count(state) > 0;
    }

    // Returns the starting state of the DFA
    State start() const { return START; }
};

std::vector<Token> scan(const std::string &input) {
  static WlpDFA theDFA;

  std::vector<Token> tokens = theDFA.simplifiedMaximalMunch(input);

  // We need to:
  // * Throw exceptions for WORD tokens whose lexemes aren't ".word".
  // * Remove WHITESPACE and COMMENT tokens entirely.

  std::vector<Token> newTokens;

  for (auto &token : tokens) {
    if (token.getKind() != Token::SPACE
        && token.getKind() != Token::Kind::COMMENT) {
      newTokens.push_back(token);
    }
  }

  return newTokens;
}
