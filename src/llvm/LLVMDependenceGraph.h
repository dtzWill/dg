/// XXX add licence
//

#ifndef _LLVM_DEPENDENCE_GRAPH_H_
#define _LLVM_DEPENDENCE_GRAPH_H_

#ifndef HAVE_LLVM
#error "Need LLVM"
#endif

#ifndef ENABLE_CFG
#error "Need CFG enabled"
#endif

#include <map>

// forward declaration of llvm classes
namespace llvm {
    class Module;
    class Value;
    class Function;
} // namespace llvm

#include "LLVMNode.h"
#include "DependenceGraph.h"

namespace dg {

typedef dg::BBlock<LLVMNode> LLVMBBlock;

/// ------------------------------------------------------------------
//  -- LLVMDependenceGraph
/// ------------------------------------------------------------------
class LLVMDependenceGraph : public DependenceGraph<LLVMNode>
{
public:
    // free all allocated memory and unref subgraphs
    virtual ~LLVMDependenceGraph();

    // build a DependenceGraph from module. This method will
    // build all subgraphs (called procedures). If entry is nullptr,
    // then this methods looks for function named 'main'.
    bool build(llvm::Module *m, llvm::Function *entry = nullptr);

    // build DependenceGraph for a function. This will automatically
    // build subgraphs of called functions
    bool build(llvm::Function *func);

private:
    // add formal parameters of the function to the graph
    // (graph is a graph of one procedure)
    void addFormalParameters();

    // take action specific to given instruction (while building
    // the graph). This is like if the val is call-site, build
    // subgraph or if it is a pointer-handling instr. then
    // add PSS edges etc.
    void handleInstruction(const llvm::Value *val, LLVMNode *node);

    // convert llvm basic block to our basic block
    // That includes creating all the nodes and adding them
    // to this graph and creating the basic block and
    // setting first and last instructions
    LLVMBBlock *build(const llvm::BasicBlock& BB);

    // build subgraph for a call node
    bool buildSubgraph(LLVMNode *node);

    std::map<const llvm::Value *, LLVMDependenceGraph *> constructedFunctions;
};

} // namespace dg

#endif // _DEPENDENCE_GRAPH_H_
