#define DEBUG_TYPE "countphis"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>
using namespace llvm;

namespace {
    struct Count_Phis : public FunctionPass {
        static char ID;
        Count_Phis() : FunctionPass(ID) {}
        
        virtual bool runOnFunction(Function &F) {
            errs() << "Function " << F.getName() << "\n";
            for (inst_iterator i = inst_begin(F), e = inst_end(F); i != e; ++i) {
                //LLVM provides a very expressive API for runtime type inference (RTTI).
                // The isa<> template is a way to know the dynamic type of a value.
//                if (isa<PHINode>(*i)) {
//                    errs() << *i << "\n";
//                    errs() << "- has " << cast<PHINode>(*i).getNumIncomingValues() << " arguments.\n";
//                }
                
                if (PHINode *PN = dyn_cast<PHINode>(&*i)) {
                    errs() << *PN << "\n";
                    int numArgs = PN->getNumIncomingValues();
                    errs() << "- has " << numArgs << " parameters.\n";
                    for (int arg = 0; arg < numArgs; arg++) {
                        errs() << " Argument " << arg << ":\n";
                        errs() << " " << PN->getIncomingBlock(arg)->getName() << " : " << *(PN->getIncomingValue(arg)) << "\n";
                    }
                }
            }
            
            return false;
        }
    };
}

char Count_Phis::ID = 0;
static RegisterPass<Count_Phis> X("countphis", "Counts phi-­‐instructions per function");
