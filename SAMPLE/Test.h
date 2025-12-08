//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Dec  8 15:05:09 2025 by ROOT version 6.32.19
// from TTree tree/tree
// found on file: Tree_KO2507.root
//////////////////////////////////////////////////////////

#ifndef Test_h
#define Test_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "EvtSimple.h"
#include "EvtASGARD.h"
#include "EvtStarkJr.h"

class Test {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   ULong_t         ref_lgt;
   EvtSimple       *EvtSimple;
   EvtASGARD       *ASGARD;
   EvtStarkJr      *StarkJr;

   // List of branches
   TBranch        *b_ref_lgt;   //!
   TBranch        *b_EvtSimple;   //!
   TBranch        *b_ASGARD;   //!
   TBranch        *b_StarkJr;   //!

   Test(TTree *tree=0);
   virtual ~Test();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual bool     Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Test_cxx
Test::Test(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Tree_KO2507.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Tree_KO2507.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

Test::~Test()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Test::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Test::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Test::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   EvtSimple = 0;
   ASGARD = 0;
   StarkJr = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ref_lgt", &ref_lgt, &b_ref_lgt);
   fChain->SetBranchAddress("EvtSimple", &EvtSimple, &b_EvtSimple);
   fChain->SetBranchAddress("ASGARD", &ASGARD, &b_ASGARD);
   fChain->SetBranchAddress("StarkJr", &StarkJr, &b_StarkJr);
   Notify();
}

bool Test::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}

void Test::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Test::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Test_cxx
