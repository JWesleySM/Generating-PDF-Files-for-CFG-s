
//Author: José Wesley de Souza Magalhães

//This pass produces a dot file to represent the CFG of a function. It takes into account four requirements
//
//	-Each instruction must be printed with an opcode.
//	-All the arguments of an instruction must be printed next to this instruction.
//	-Arguments that do not have names, such as getelementptr in function calls, should not be printed.
//	-Type information should not be printed.


#define DEBUG_TYPE "Project1"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include<iostream>
#include<fstream>
#include<sstream>

using namespace llvm;
using namespace std;


namespace{
    struct GenDot: public ModulePass{
      static char ID;
      int temporary_index;
      GenDot(): ModulePass(ID){}
      
      void drawCFGEdge(BasicBlock &BB, string BBname, ofstream *dotFile){
	TerminatorInst *TI = BB.getTerminator();
	for(unsigned int i=0; i < TI->getNumSuccessors(); i++){ //we have to draw one edge for each successor
	  string succName = TI->getSuccessor(i)->getName().str(); 
	  succName.erase(remove(succName.begin(), succName.end(), '.'), succName.end()); //for some syntax specification of dot files, the name of a block cannot contain '.' character, so we remove any of it
	  *dotFile << BBname << " -> " << succName <<";" <<endl;
	}
      }
      
      void drawCFGNode(BasicBlock &BB, ofstream *dotFile, int *temporary_index){
	string BBname = BB.getName().str();
	BBname.erase(remove(BBname.begin(), BBname.end(), '.'), BBname.end()); 
	*dotFile << BBname <<" [shape=record, label=\"{" <<BB.getName().str() <<":\\l\\l"<<endl;	
	for(Instruction &I : BB){
	  if(I.hasName()){
	    *dotFile << I.getName().str() <<" = ";
	  }
	  else if (!I.getType()->isVoidTy()){ //if we have an argument like %x, we create a name for it. The same for Instruction names
	    stringstream ss;
	    ss<<*temporary_index;
	    string argName = "%";
	    argName += ss.str();
	    I.setName(argName);
	    ++*temporary_index;
	    *dotFile << I.getName().str() <<" = ";
	  }
	  *dotFile <<I.getOpcodeName(); //print the opcode of the function 
	  for(unsigned int i=0; i<I.getNumOperands();i++){	    
	    Value* operand = I.getOperand(i);
	    if(operand->hasName())
	      *dotFile <<" " << operand->getName().str(); //print the arguments of a instruction that have a name
	    else if(I.getOpcode()!= Instruction::Alloca){
	      if(ConstantInt *CI = dyn_cast<ConstantInt>(operand)) //if an argument is constatn, we print it's value
		*dotFile <<" " << CI->getZExtValue();
	    }
	    
	  }
	  *dotFile<< "\\l" << endl;
	}
	*dotFile << "}\"];" <<endl;
	if(BB.getTerminator()->getNumSuccessors() > 0) //If a basic block has sucessors, we draw the correspondent edges of the CFG
	   drawCFGEdge(BB, BBname, dotFile);
      }
      
      bool runOnModule(Module &M){
	for(Function &F: M){
	  temporary_index = 0;
	  if(!F.isDeclaration()){ //we create a dot file for every user defined function in the source code
	    string filename = (F.getName() + ".dot").str(); 
	    ofstream dotFile(filename);
	    dotFile << "digraph \"CFG for '" << F.getName().str() <<"' function\" {" <<endl;
	    for(BasicBlock &BB : F){  
		drawCFGNode(BB, &dotFile, &temporary_index); //every basic block is a node in the CFG
	    }
	    dotFile << "}" << endl;
	    dotFile.close();
	  }
	}
	return false;
      }
      
    };
  
}
char GenDot::ID = 0;
static RegisterPass<Project1> X("GenDot", "Convert bytecode files to dot format");