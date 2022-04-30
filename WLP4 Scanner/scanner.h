#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <vector>
#include <set>
#include <cstdint>
#include <ostream>


class Token;

//Scans a single line of input and produces a list of tokens.

std::vector<Token> scan(const std::string &input);

/* A scanned token produced by the scanner.
 * The "kind" tells us what kind of token it is
 * while the "lexeme" tells us exactly what text
 * the programmer typed. 
 */
class Token {
  public:
    enum Kind {
      ID = 0,
      NUM,
      LPAREN,
      RPAREN,
      LBRACE,
      RBRACE,
      RETURN,
      IF,
      ELSE,
      WHILE,
      PRINTLN,
      WAIN,
      BECOMES,
      INT,
      EQ,
      NE,
      LT,
      GT,
      LE,
      GE,
      PLUS,
      MINUS,
      STAR,
      PCT,
      COMMA,
      SEMI,
      NEW,
      DELETE,
      LBRACK,
      RBRACK,
      AMP,
      NUL,
      SPACE,
      SLASH,
      COMMENT
    };

  private:
    Kind kind;
    std::string lexeme;

  public:
    Token(Kind kind, std::string lexeme);
    Kind getKind() const;
    const std::string &getLexeme() const;
};


// Prints a string representation of a token, so it prints the token Kind
//  and the lexeme string
std::ostream &operator<<(std::ostream &out, const Token &tok);

// An exception class thrown when an error is encountered while scanning.
class ScanningFailure {
    std::string message;

  public:
    ScanningFailure(std::string message);

    // Returns the message associated with the exception.
    const std::string &what() const;
};

#endif
