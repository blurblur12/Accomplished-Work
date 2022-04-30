#include <iostream>
#include <string>
#include <vector>
#include "scanner.h"

/*
 * Referenced to C++ Starter code for CS241 A3
 * This file contains the main function of the scanner, it prints the
 * list of tokens and their lexeme back to standard output.
 */
int main() {
  std::string line;
  try {
    while (getline(std::cin, line)) {
      // prints the scanned tokens on each line.
      std::vector<Token> tokenLine = scan(line);

      for (auto &token : tokenLine) {
        std::cout << token << std::endl;
      }
    }
  } catch (ScanningFailure &f) {
    std::cerr << f.what() << std::endl;
    return 1;
  }
  return 0;
}
