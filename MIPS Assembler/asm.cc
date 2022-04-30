#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <map>
#include "scanner.h"

// An exception class thrown when an error is encountered
// during assembler execution
// Referenced from the ScanningFailure exception class in scanner.h
class AssemError {
    std::string error_mess;

  public:
    AssemError(std::string error_mess);

    // Returns the message associated with the exception.
    const std::string &giveMess() const;
};

AssemError::AssemError(std::string error_mess):
  error_mess(std::move(error_mess)) {}

const std::string &AssemError::giveMess() const { return error_mess; }

/*
 * C++ Starter code for CS241 A3
 * All code requires C++14, so if you're getting compile errors make sure to
 * use -std=c++14.
 *
 * This file contains the main function of your program. By default, it just
 * prints the scanned list of tokens back to standard output.
 */
int main() {
  std::vector<std::vector<Token>> alltokens;
  std::string line;
  std::map<std::string,int64_t> symbol_table;
  int64_t pc = 0;
  try {
    while (getline(std::cin, line)) {
      // Contains the scanned tokens
      std::vector<Token> tokenLine = scan(line);

      for (int i = 0; i < tokenLine.size(); i++) {
          // .word instruction
          if (tokenLine[i].getKind() == Token::WORD) {
              // check if immediate is given
              if (tokenLine.size() - i == 2) {
                  // check if the given is indeed an immediate or a label
                  if (tokenLine[i+1].getKind() == Token::INT || tokenLine[i+1].getKind() ==
                  Token::HEXINT || tokenLine[i+1].getKind() == Token::ID) {
                      pc += 4;
                  }
                  else {
                      // next token not an immediate or label
                      throw AssemError("ERROR");
                  }
                  i = i + 1;
              }
              else {
                  // did not provide the immediate token
                  throw AssemError("ERROR");
              }
          }
          else if (tokenLine[i].getKind() == Token::LABEL) {
              // obatin the label name with colon removed
              std::string labelname = tokenLine[i].getLexeme();
              labelname = labelname.substr(0, labelname.length()-1);
              if (symbol_table.count(labelname) == 0) {
                  // if there is no duplicate, add the label into the table
                  symbol_table[labelname] = pc;
              }
              else {
                  // duplicate label, throw error
                  throw AssemError("ERROR");
              }
          }
          else if (tokenLine[i].getKind() == Token::ID) {
              // jr, jalr, lis, mflo, or mfhi instruction
              if (tokenLine[i].getLexeme() == "jr" || tokenLine[i].getLexeme() == "jalr"
              || tokenLine[i].getLexeme() == "lis" || tokenLine[i].getLexeme() == "mflo"
              || tokenLine[i].getLexeme() == "mfhi") {
                  if (tokenLine.size() - i == 2) {
                      // check if the next token is a regsiter
                      if (tokenLine[i + 1].getKind() == Token::REG) {
                          // check if the register is within bound
                          int64_t reg = tokenLine[i + 1].toNumber();
                          if (reg >= 0 && reg <= 31) {
                              pc += 4;
                          }
                          else {
                              // register out of bound
                              throw AssemError("ERROR");
                          }
                      }
                      else {
                          // next token not register
                          throw AssemError("ERROR");
                      }
                      i = i + 1;
                  }
                  else {
                      // extra or not enough tokens given
                      throw AssemError("ERROR");
                  }
              }
              else if (tokenLine[i].getLexeme() == "add" || tokenLine[i].getLexeme() == "sub" ||
              tokenLine[i].getLexeme() == "slt" || tokenLine[i].getLexeme() == "sltu" ||
              tokenLine[i].getLexeme() == "beq" || tokenLine[i].getLexeme() == "bne") {
                  // if enough tokens are given
                  if (tokenLine.size() - i == 6) {
                      // check tokens are of correct type
                      if (tokenLine[i+1].getKind() == Token::REG &&
                      tokenLine[i+2].getKind() == Token::COMMA &&
                      tokenLine[i+3].getKind() == Token::REG &&
                      tokenLine[i+4].getKind() == Token::COMMA) {
                          // if instruction is one of beq or bne last token is an INT or HEXINT
                          if (tokenLine[i].getLexeme() == "beq" || tokenLine[i].getLexeme() == "bne") {
                              // check the last register is an INT or HEXINT or ID
                              if (tokenLine[i+5].getKind()!= Token::INT && 
                              tokenLine[i+5].getKind()!= Token::HEXINT &&
                              tokenLine[i+5].getKind() != Token::ID) {
                                  throw AssemError("ERROR");
                              }
                              // if offset is given as INT or HEXINT, check if within range
                              if (tokenLine[i+5].getKind() != Token::ID) {
                                  // check if immediate is within 16-bit range
                                  int64_t immediate = tokenLine[i+5].toNumber();
                                  if (tokenLine[i+5].getKind() == Token::INT) {
                                      if (!(immediate >= -32768 && immediate <= 32768)) {
                                          throw AssemError("ERROR");
                                      } 
                                  }
                                  else {
                                      if (!(immediate <= 0xffff)) {
                                          throw AssemError("ERROR");
                                      }
                                  }
                              }
                          }
                          else {
                              if (tokenLine[i+5].getKind() != Token::REG) {
                                  throw AssemError("ERROR");
                              }
                              // check last register is within range
                              int64_t reg = tokenLine[i+5].toNumber();
                              if (!(reg >= 0 && reg <= 31)) {
                                  throw AssemError("ERROR");
                              }
                          }
                          // check other registers within range
                          for (int k = i+1; k < i+5; k++) {
                              int64_t reg = tokenLine[k].toNumber();
                              if (reg >= 0 && reg <= 31) {
                                  k += 1;
                              }
                              else {
                                  // registers out of bound
                                  throw AssemError("ERROR");
                              }
                          }
                          pc += 4;
                      }
                      else {
                          throw AssemError("ERROR");
                      }
                      i = i + 5;
                  }
                  else {
                      // token numbers wrong
                      throw AssemError("ERROR");
                  }
              }
              else if (tokenLine[i].getLexeme() == "mult" || tokenLine[i].getLexeme() == "multu"
              || tokenLine[i].getLexeme() == "div" || tokenLine[i].getLexeme() == "divu") {
                  // if enough tokens are given
                  if (tokenLine.size() - i == 4) {
                      // Tokens are of the right kind
                      if (tokenLine[i+1].getKind() == Token::REG && tokenLine[i+2].getKind() == Token::COMMA
                      && tokenLine[i+3].getKind() == Token::REG) {
                          // check registers within range
                          int64_t reg;
                          for (int k = i+1; k < i+4; k++) {
                              reg = tokenLine[k].toNumber();
                              if (reg >= 0 && reg <= 31) {
                                  k = k + 1;
                              }
                              else {
                                  // not valid register
                                  throw AssemError("ERROR");
                              }
                          }
                      }
                      else {
                          // token kind incorrect
                          throw AssemError("ERROR");
                      }
                      i = i + 3;
                  }
                  else {
                      // not enough or extra tokens given
                      throw AssemError("ERROR");
                  }
              }
              else if (tokenLine[i].getLexeme() == "sw" || tokenLine[i].getLexeme() == "lw") {
                  // check enough tokens are given
                  if (tokenLine.size() - i == 7) {
                      // check tokens are of the right kind
                      if (tokenLine[i+1].getKind() == Token::REG && tokenLine[i+2].getKind() == Token::COMMA
                      && (tokenLine[i+3].getKind() == Token::INT || tokenLine[i+3].getKind() == Token::HEXINT)
                      && tokenLine[i+4].getKind() == Token::LPAREN && tokenLine[i+5].getKind() == Token::REG
                      && tokenLine[i+6].getKind() == Token::RPAREN) {
                          // check register within range
                          int64_t reg;
                          for (int k = i+1; k < i+6; k++) {
                              reg = tokenLine[k].toNumber();
                              if (reg >= 0 && reg <= 31) {
                                  k = k + 3;
                              }
                              else {
                                  // register out of bound
                                  throw AssemError("ERROR");
                              }
                          }
                          // check immediate within range
                          int64_t num = tokenLine[i+3].toNumber();
                          if (tokenLine[i+3].getKind() == Token::INT) {
                              if (!(num >= -32768 && num <= 32767)) {
                                  throw AssemError("ERROR");
                              }
                          }
                          else {
                              if (!(num <= 0xFFFF)) {
                                  throw AssemError("ERROR");
                              }
                          }
                          pc += 4;
                      }
                      else {
                          // token kinds incorrect
                          throw AssemError("ERROR");
                      }
                      i = i + 6;
                  }
                  else {
                      // not enough or extra tokens given
                      throw AssemError("ERROR");
                  }
              }
              else {
                  // not correct instructions
                  throw AssemError("ERROR");
              }
          }
          else {
              // not a .word, label, or id
              throw AssemError("ERROR");
          }
      }
      // if no errors thrown, add this line into the vector of all instructions
      alltokens.push_back(tokenLine);
    }
    // here restart pc to keep track of instruction location in the second pass
    pc = 0;
    // second pass
    for (int i = 0; i < alltokens.size(); i++) {
        for (int j = 0; j < alltokens[i].size(); j++) {
            if (alltokens[i][j].getKind() == Token::WORD) {
                int64_t instr;
                if (alltokens[i][j + 1].getKind() == Token::INT ||
                alltokens[i][j + 1].getKind() == Token::HEXINT) {
                    instr = alltokens[i][j + 1].toNumber();
                }
                else {
                    std::string label = alltokens[i][j + 1].getLexeme();
                    // check if label oprend is defined in the symbol table
                    if (symbol_table.count(label) == 1) {
                        instr = symbol_table[label];
                    }
                    else {
                        // label is undefined throw error
                        throw AssemError("ERROR");
                    }
                }
                // Check if immediate or label oprend is within range
                if ((instr != LLONG_MIN) && (instr != LLONG_MAX) && (instr >= INT_MIN) && (instr <= UINT_MAX)) {
                    unsigned char c = instr >> 24;
                    std::cout << c;
                    c = instr >> 16;
                    std::cout << c;
                    c = instr >> 8;
                    std::cout << c;
                    c = instr;
                    std::cout << c;
                }
                else {
                    // out bound throw errors
                    throw AssemError("ERROR");
                }
                j = j + 1;
                pc += 4;
            }
            else if (alltokens[i][j].getKind() == Token::ID) {
                int64_t instr;
                int64_t s;
                int64_t t;
                int64_t d;
                int64_t im;
                int64_t f; // represent the function bits
                int64_t opc; // represent the opcode
                if (alltokens[i][j].getLexeme() == "jr") {
                    s = alltokens[i][j + 1].toNumber();
                    instr = (0 << 26) | (s << 21) | (0 << 16) | (0 << 11) | 8;
                    j = j + 1;
                }
                else if (alltokens[i][j].getLexeme() == "jalr") {
                    s = alltokens[i][j + 1].toNumber();
                    instr = (0 << 26) | (s << 21) | (0 << 16) | (0 << 11) | 9;
                    j = j + 1;
                }
                else if (alltokens[i][j].getLexeme() == "add" || alltokens[i][j].getLexeme() == "sub"
                || alltokens[i][j].getLexeme() == "slt" || alltokens[i][j].getLexeme() == "sltu") {
                    // if instruction is add
                    if (alltokens[i][j].getLexeme() == "add") {
                        f = 32;
                    }
                    else if (alltokens[i][j].getLexeme() == "sub") {
                        f = 34;
                    }
                    else if (alltokens[i][j].getLexeme() == "slt") {
                        f = 42;
                    }
                    else {
                        f = 43;
                    }
                    d = alltokens[i][j + 1].toNumber();
                    s = alltokens[i][j + 3].toNumber();
                    t = alltokens[i][j + 5].toNumber();
                    instr = (0 << 26) | (s << 21) | (t << 16) | (d << 11) | f;
                    j = j + 5;
                }
                else if (alltokens[i][j].getLexeme() == "beq" || alltokens[i][j].getLexeme() == "bne") {
                    if (alltokens[i][j].getLexeme() == "beq") {
                        opc = 4;
                    }
                    else {
                        opc = 5;
                    }
                    // offset is given as label
                    if (alltokens[i][j+5].getKind() == Token::ID) {
                        std::string label = alltokens[i][j + 5].getLexeme();
                        // check if label oprend is defined in the symbol table
                        if (symbol_table.count(label) == 1) {
                            im = symbol_table[label];
                        }
                        else {
                            // label is undefined throw error
                            throw AssemError("ERROR");
                        }
                        // check label offset within range
                        im = (im - (pc + 4))/4;
                        if (!(im >= -32768 && im <= 32767)) {
                            throw AssemError("ERROR");
                        }
                    }
                    else {
                        im = alltokens[i][j + 5].toNumber(); 
                    }
                    s = alltokens[i][j + 1].toNumber();
                    t = alltokens[i][j + 3].toNumber();
                    instr = (opc << 26) | (s << 21) | (t << 16) | (im & 0xFFFF);
                    j = j + 5;
                }
                else if (alltokens[i][j].getLexeme() == "lis" || alltokens[i][j].getLexeme() == "mflo"
                || alltokens[i][j].getLexeme() == "mfhi") {
                    if (alltokens[i][j].getLexeme() == "lis") {
                        f = 20;
                    }
                    else if (alltokens[i][j].getLexeme() == "mflo") {
                        f = 18;
                    }
                    else {
                        f = 16;
                    }
                    d = alltokens[i][j+1].toNumber();
                    instr = (0 << 26) | (0 << 21) | (0 << 16) | (d << 11) | f;
                    j = j + 1;
                }
                else if (alltokens[i][j].getLexeme() == "mult" || alltokens[i][j].getLexeme() == "multu"
                || alltokens[i][j].getLexeme() == "div" || alltokens[i][j].getLexeme() == "divu") {
                    if (alltokens[i][j].getLexeme() == "mult") {
                        f = 24;
                    }
                    else if (alltokens[i][j].getLexeme() == "multu") {
                        f = 25;
                    }
                    else if (alltokens[i][j].getLexeme() == "div") {
                        f = 26;
                    }
                    else {
                        f = 27;
                    }
                    s = alltokens[i][j+1].toNumber();
                    t = alltokens[i][j+3].toNumber();
                    instr = (0 << 26) | (s << 21) | (t << 16) | (0 << 11) | f;
                    j = j + 3;
                }
                else if (alltokens[i][j].getLexeme() == "sw" || alltokens[i][j].getLexeme() == "lw") {
                    if (alltokens[i][j].getLexeme() == "sw") {
                        opc = 43;
                    }
                    else {
                        opc = 35;
                    }
                    s = alltokens[i][j+5].toNumber();
                    t = alltokens[i][j+1].toNumber();
                    im = alltokens[i][j+3].toNumber();
                    instr = (opc << 26) | (s << 21) | (t << 16) | (im & 0xFFFF);
                    j = j + 6;
                }
                unsigned char c = instr >> 24;
                std::cout << c;
                c = instr >> 16;
                std::cout << c;
                c = instr >> 8;
                std::cout << c;
                c = instr;
                std::cout << c;
                pc += 4;
            }
        }
    }
    
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  // You can add your own catch clause(s) for other kinds of errors.
  // Throwing exceptions and catching them is the recommended way to
  // handle errors and terminate the program cleanly in C++. Do not
  // use the std::exit function, which may leak memory.
  catch (AssemError &e) {
	  std::cerr << e.giveMess() << std::endl;
	  return 1;
  }
  symbol_table.clear();
  alltokens.clear();
  return 0;
}
