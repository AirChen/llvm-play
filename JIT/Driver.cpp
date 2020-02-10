#include "llvm/Analysis/Verifier.h"
#include "llvm/ADT/APInt.h"
#include "llvm/Analysis/Passes.h" // this
#include "llvm/PassManager.h" // this
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/IR/DataLayout.h" // this
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Scalar.h" // this

#include "Expr.h"
#include "Lexer.h"
#include "Parser.h"

llvm::Function *createEntryFunction(
    llvm::Module *module,
    llvm::LLVMContext &context) {
  llvm::Function *function =
     llvm::cast<llvm::Function>(
         module->getOrInsertFunction("fun",
           llvm::Type::getInt32Ty(context),
           llvm::Type::getInt32Ty(context),
           (llvm::Type *)0)
         );
  llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", function);
  llvm::IRBuilder<> builder(context);
  builder.SetInsertPoint(bb);
  llvm::Argument *argX = function->arg_begin();
  argX->setName("x");
  VarExpr::varValue = argX;
  Lexer lexer;
  Parser parser(&lexer);
  Expr* expr = parser.parseExpr();
  llvm::Value* retVal = expr->gen(&builder, context);
  builder.CreateRet(retVal);
  return function;
}

llvm::ExecutionEngine* createEngine(llvm::Module *module) {
  llvm::InitializeNativeTarget();

  std::string errStr;
  llvm::ExecutionEngine *engine =
    llvm::EngineBuilder(module)
    .setErrorStr(&errStr)
    .setEngineKind(llvm::EngineKind::JIT)
    .create();

  if (!engine) {
    llvm::errs() << "Failed to construct ExecutionEngine: " << errStr << "\n";
  } else if (llvm::verifyModule(*module)) {
    llvm::errs() << "Error constructing function!\n";
  }
  return engine;
}

void JIT(llvm::ExecutionEngine* engine, llvm::Function* function, int arg) {
  std::vector<llvm::GenericValue> Args(1);
  Args[0].IntVal = llvm::APInt(32, arg);
  llvm::GenericValue retVal = engine->runFunction(function, Args);
  llvm::outs() << "Result: " << retVal.IntVal << "\n";
}

void optimizeFunction(
  llvm::ExecutionEngine* engine,
  llvm::Module *module,
  llvm::Function* function
) {
  llvm::FunctionPassManager passManager(module);
  passManager.add(new llvm::DataLayout(*engine->getDataLayout()));
  passManager.add(llvm::createInstructionCombiningPass());
  passManager.add(llvm::createReassociatePass());
  passManager.add(llvm::createGVNPass());
  passManager.add(llvm::createCFGSimplificationPass());
  passManager.doInitialization();
  passManager.run(*function);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    llvm::errs() << "Inform an argument to your expression.\n";
    return 1;
  } else {
    llvm::LLVMContext context;
    llvm::Module *module = new llvm::Module("Example", context);
    llvm::Function *function = createEntryFunction(module, context);
    llvm::errs() << "Module before optimizations:\n";
    module->dump();
    llvm::errs() << "Module after optimizations:\n";
    llvm::ExecutionEngine* engine = createEngine(module);
    optimizeFunction(engine, module, function);
    module->dump();
    JIT(engine, function, atoi(argv[1]));
  }
}
