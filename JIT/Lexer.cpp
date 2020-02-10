#include "Lexer.h"
std::string Lexer::getToken() {
  while (isspace(lastChar)) { lastChar = getchar(); }
  if (isalpha(lastChar)) {
    std::string idStr;
    do { idStr += getNextChar(); } while (isalnum(lastChar));
    return idStr;
  } else if (isdigit(lastChar)) {
    std::string numStr;
    do { numStr += getNextChar(); } while (isdigit(lastChar));
    return numStr;
  } else if (lastChar == EOF) {
    return "";
  } else {
    std::string operatorStr;
    operatorStr = getNextChar();
    return operatorStr;
  }
}
