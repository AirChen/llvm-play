#define DEBUG_TYPE "bbCounter"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/CodeGen/MachineLoopInfo.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>
using namespace llvm;

namespace {
    struct BBinLoops : public FunctionPass {
        static char ID;
        BBinLoops() : FunctionPass(ID) {}
        
        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
            AU.addRequired<LoopInfo>();
            AU.setPreservesAll();
        }
        
        void countBlocksInLoop(Loop *L, unsigned nesting) {
            unsigned numBlocks = 0;
            Loop::block_iterator bb;
            for (bb = L->block_begin(); bb != L->block_end(); ++bb) {
                numBlocks++;
            }
            
            errs() << "Loop level " << nesting << " has " << numBlocks << " blocks\n";
            std::vector<Loop*> subLoops = L->getSubLoops();
            Loop::iterator j, f;
            for (j = subLoops.begin(), f = subLoops.end(); j != f; ++j) {
                countBlocksInLoop(*j, nesting + 1);
            }
        }
        
        virtual bool runOnFunction(Function &F) {
            LoopInfo &Ll = getAnalysis<LoopInfo>();
            errs() << F.getName() << "\n";
            for (LoopInfo::iterator i = Ll.begin(), e = Ll.end(); i != e; ++i) {
                countBlocksInLoop(*i, 0);
            }
            
            return false;
        }
    };
}

char BBinLoops::ID = 0;
static RegisterPass<BBinLoops> X("bbCounter", "Count the number of BBs inside each loop");
