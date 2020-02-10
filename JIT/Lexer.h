#ifndef LEXER_H
#define LEXER_H

#include <string>

class Lexer {
  public:
    std::string getToken();
    Lexer() : lastChar(' ') {}
  private:
    char lastChar;
    inline char getNextChar() {
      char c = lastChar;
      lastChar = getchar();
      return c;
    }
};

#endif
