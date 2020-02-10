#ifndef PARSER_H
#define PARSER_H

#include <string>

class Expr;
class Lexer;

class Parser {
  public:
    Parser(Lexer* argLexer) : lexer(argLexer) {}
    Expr* parseExpr();
  private:
    Lexer* lexer;
};

#endif
