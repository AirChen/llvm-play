#ifndef AST_H
#define AST_H

#include "llvm/IR/IRBuilder.h"

class Expr {
  public:
    virtual ~Expr() {}
    virtual int eval() const = 0;
    virtual llvm::Value *gen(llvm::IRBuilder<> *builder, llvm::LLVMContext& con) const = 0;
};

class NumExpr : public Expr {
  public:
    NumExpr(int argNum) : num(argNum) {}
    int eval() const { return num; }
    llvm::Value *gen(llvm::IRBuilder<> *builder, llvm::LLVMContext& con) const;
    static const unsigned int SIZE_INT = 32;
  private:
    const int num;
};

class VarExpr : public Expr {
  public:
    int eval() const { return 0; }
    llvm::Value *gen(llvm::IRBuilder<> *builder, llvm::LLVMContext& con) const;
    static llvm::Value* varValue;
};

class AddExpr : public Expr {
  public:
    AddExpr(Expr* op1Arg, Expr* op2Arg) : op1(op1Arg), op2(op2Arg) {}
    int eval() const { return op1->eval() + op2->eval(); }
    llvm::Value *gen(llvm::IRBuilder<> *builder, llvm::LLVMContext& con) const;
  private:
    const Expr* op1;
    const Expr* op2;
};

class MulExpr : public Expr {
  public:
    MulExpr(Expr* op1Arg, Expr* op2Arg) : op1(op1Arg), op2(op2Arg) {}
    int eval() const { return op1->eval() * op2->eval(); }
    llvm::Value *gen(llvm::IRBuilder<> *builder, llvm::LLVMContext& con) const;
  private:
    const Expr* op1;
    const Expr* op2;
};

#endif
