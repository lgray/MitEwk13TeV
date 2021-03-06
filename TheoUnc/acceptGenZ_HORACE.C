#if !defined(__CINT__) //|| defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TClonesArray.h>           // ROOT array class
#include <TBenchmark.h>             // class to track macro running statistics
#include <TVector2.h>               // 2D vector class
#include <TMath.h>                  // ROOT math library
#include <TRandom3.h>
#include <vector>                   // STL vector class
#include <iostream>                 // standard I/O
#include <sstream>                 // standard I/O
#include <iomanip>                  // functions to format standard I/O
#include <fstream>                  // functions for file I/O
#include <TChain.h>
#include <TH1.h>

using namespace std;

#endif

enum PROC {zee=0, zmm};

Bool_t acceptBB(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id, Double_t genL1_pt, Double_t genL1_eta, Double_t genL2_pt, Double_t genL2_eta);
Bool_t acceptBE(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id, Double_t genL1_pt, Double_t genL1_eta, Double_t genL2_pt, Double_t genL2_eta);
Bool_t acceptEE(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id, Double_t genL1_pt, Double_t genL1_eta, Double_t genL2_pt, Double_t genL2_eta);
Bool_t isProc(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id);
Bool_t acceptB(Double_t genL_id, Double_t genL_pt, Double_t genL_eta);
Bool_t acceptE(Double_t genL_id, Double_t genL_pt, Double_t genL_eta);

//void acceptGenZ_HORACE(TString input="/afs/cern.ch/work/j/jlawhorn/public/HORACE_NOT_FUCKED/z_electron_born_final.root",
void acceptGenZ_HORACE(TString input="/afs/cern.ch/work/j/jlawhorn/public/HORACE_NOT_FUCKED/z_muon_born_final_photos.root",
//void acceptGenZ_HORACE(TString input="z_muon_fsr_all_flat.root",
//void acceptGenZ_HORACE(TString input="root://eoscms//store/user/jlawhorn/Horace_new/z_electron_fsr.root",
		       TString outputDir="idfk/",
		       Int_t proc=1) {

  TString procName[2]={"zee", "zmm"};
  char output[150];
  sprintf(output, "%s%s_%s.root", outputDir.Data(), procName[proc].Data(), "foo");
  cout << output << endl;
  TChain chain("Events");
  chain.Add(input);
  
  //PDF info
  Double_t id_1,      id_2,       x_1,        x_2;
  Double_t xPDF_1,    xPDF_2,     scalePDF,   weight;
  //Generator level V+l info
  Double_t genV_id,   genL1_id,   genL2_id;
  Double_t genV_pt,   genV_eta,   genV_phi,   genV_m;
  Double_t genVf_pt,  genVf_eta,  genVf_phi,  genVf_m;
  Double_t genL1_pt,  genL1_eta,  genL1_phi,  genL1_m;
  Double_t genL2_pt,  genL2_eta,  genL2_phi,  genL2_m;
  Double_t genL1f_pt, genL1f_eta, genL1f_phi, genL1f_m;
  Double_t genL2f_pt, genL2f_eta, genL2f_phi, genL2f_m;

  chain.SetBranchAddress("id_1",       &id_1);
  chain.SetBranchAddress("id_2",       &id_2);
  chain.SetBranchAddress("x_1",        &x_1);
  chain.SetBranchAddress("x_2",        &x_2);
  chain.SetBranchAddress("xPDF_1",     &xPDF_1);
  chain.SetBranchAddress("xPDF_2",     &xPDF_2);
  chain.SetBranchAddress("scalePDF",   &scalePDF);
  chain.SetBranchAddress("weight",     &weight);
  chain.SetBranchAddress("genV_pt",    &genV_pt);
  chain.SetBranchAddress("genV_eta",   &genV_eta);
  chain.SetBranchAddress("genV_phi",   &genV_phi);
  chain.SetBranchAddress("genVf_m",     &genV_m);
  chain.SetBranchAddress("genV_id",    &genV_id);
  chain.SetBranchAddress("genVf_pt",   &genVf_pt);
  chain.SetBranchAddress("genVf_eta",  &genVf_eta);
  chain.SetBranchAddress("genVf_phi",  &genVf_phi);
  chain.SetBranchAddress("genV_m",    &genVf_m);
  chain.SetBranchAddress("genL1_pt",   &genL1_pt);
  chain.SetBranchAddress("genL1_eta",  &genL1_eta);
  chain.SetBranchAddress("genL1_phi",  &genL1_phi);
  chain.SetBranchAddress("genL1_m",    &genL1_m);
  chain.SetBranchAddress("genL1_id",   &genL1_id);
  chain.SetBranchAddress("genL2_pt",   &genL2_pt);
  chain.SetBranchAddress("genL2_eta",  &genL2_eta);
  chain.SetBranchAddress("genL2_phi",  &genL2_phi);
  chain.SetBranchAddress("genL2_m",    &genL2_m);
  chain.SetBranchAddress("genL2_id",   &genL2_id);
  chain.SetBranchAddress("genL1f_pt",  &genL1f_pt);
  chain.SetBranchAddress("genL1f_eta", &genL1f_eta);
  chain.SetBranchAddress("genL1f_phi", &genL1f_phi);
  chain.SetBranchAddress("genL1f_m",   &genL1f_m);
  chain.SetBranchAddress("genL2f_pt",  &genL2f_pt);
  chain.SetBranchAddress("genL2f_eta", &genL2f_eta);
  chain.SetBranchAddress("genL2f_phi", &genL2f_phi);
  chain.SetBranchAddress("genL2f_m",   &genL2f_m);

  TFile *outFile = new TFile(output, "recreate");

  char histname[100];
  sprintf(histname,"dEta");
  TH1D* dEta = new TH1D(histname, "", 20, -5, 5); dEta->Sumw2();
  sprintf(histname,"dPt");
  TH1D* dPt = new TH1D(histname, "", 25, 25, 100); dPt->Sumw2();
  
  sprintf(histname,"dPreBB");
  TH1D* dPreBB = new TH1D(histname, "", 1, 0, 2); dPreBB->Sumw2();
  sprintf(histname,"dPreBE");
  TH1D* dPreBE = new TH1D(histname, "", 1, 0, 2); dPreBE->Sumw2();
  sprintf(histname,"dPreEE");
  TH1D* dPreEE = new TH1D(histname, "", 1, 0, 2); dPreEE->Sumw2();
  
  sprintf(histname,"dPostBB");
  TH1D* dPostBB = new TH1D(histname, "", 1, 0, 2); dPostBB->Sumw2();
  sprintf(histname,"dPostBE");
  TH1D* dPostBE = new TH1D(histname, "", 1, 0, 2); dPostBE->Sumw2();
  sprintf(histname,"dPostEE");
  TH1D* dPostEE = new TH1D(histname, "", 1, 0, 2); dPostEE->Sumw2();
  
  sprintf(histname,"dTot");
  TH1D* dTot = new TH1D(histname, "", 1, 0, 2); dTot->Sumw2();
  
  Double_t nentries=0, nsel=0;
  
  for (Int_t i=0; i<chain.GetEntries(); i++) {
    chain.GetEntry(i);
    
    if (!isProc(proc, genV_id, genV_m, genL1_id, genL2_id)) continue;
    
    nentries+=weight;
    
    dTot->Fill(1.0,weight);

    Bool_t passBB =acceptBB(proc, genV_id, genV_m, genL1_id, genL2_id, genL1_pt, genL1_eta, genL2_pt, genL2_eta);
    Bool_t passBBF=acceptBB(proc, genV_id, genV_m, genL1_id, genL2_id, genL1f_pt, genL1f_eta, genL2f_pt, genL2f_eta);
    
    Bool_t passBE =acceptBE(proc, genV_id, genV_m, genL1_id, genL2_id, genL1_pt, genL1_eta, genL2_pt, genL2_eta);
    Bool_t passBEF=acceptBE(proc, genV_id, genV_m, genL1_id, genL2_id, genL1f_pt, genL1f_eta, genL2f_pt, genL2f_eta);
    
    Bool_t passEE =acceptEE(proc, genV_id, genV_m, genL1_id, genL2_id, genL1_pt, genL1_eta, genL2_pt, genL2_eta);
    Bool_t passEEF=acceptEE(proc, genV_id, genV_m, genL1_id, genL2_id, genL1f_pt, genL1f_eta, genL2f_pt, genL2f_eta);

    if      (passBB) dPreBB->Fill(1.0, weight);
    else if (passBE) dPreBE->Fill(1.0, weight);
    else if (passEE) dPreEE->Fill(1.0, weight);
    
    if      (passBBF) { dPostBB->Fill(1.0, weight); }
    else if (passBEF) { dPostBE->Fill(1.0, weight); }
    else if (passEEF) { dPostEE->Fill(1.0, weight); }
    
    if (passBBF || passBEF || passEEF) {
      //if (passBB || passBE || passEE) {
      dEta->Fill(genVf_eta, weight);
      dPt->Fill(genVf_pt, weight);        
      nsel+=weight;
    }
  }
  cout << "------------" << endl;
  Double_t acc=(dPreBB->Integral()+dPreBE->Integral()+dPreEE->Integral())/(dTot->Integral());
  
  cout << "Pre-FSR acceptance: " << dPreBB->Integral() + dPreBE->Integral() + dPreEE->Integral() << " / " << dTot->Integral() << " = " << acc << " +/- " << sqrt(acc*(1+acc)/dTot->GetEntries()) << endl;
  
  acc=(dPostBB->Integral()+dPostBE->Integral()+dPostEE->Integral())/(dTot->Integral());
  
  cout << "Post-FSR acceptance: " << dPostBB->Integral()+dPostBE->Integral()+dPostEE->Integral() << " / " << dTot->Integral() << " = " << acc << " +/- " << sqrt(acc*(1+acc)/dTot->GetEntries()) << endl;
  
}

Bool_t acceptEE(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id, Double_t genL1_pt, Double_t genL1_eta, Double_t genL2_pt, Double_t genL2_eta) {
  
  if (!isProc(proc, genV_id, genV_m, genL1_id, genL2_id)) return kFALSE;
  else if (acceptE(genL1_id, genL1_pt, genL1_eta) && acceptE(genL2_id, genL2_pt, genL2_eta)) return kTRUE;
  else return kFALSE;
}

Bool_t acceptBE(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id, Double_t genL1_pt, Double_t genL1_eta, Double_t genL2_pt, Double_t genL2_eta) {

  if (!isProc(proc, genV_id, genV_m, genL1_id, genL2_id)) return kFALSE;
  else if (acceptB(genL1_id, genL1_pt, genL1_eta) && acceptE(genL2_id, genL2_pt, genL2_eta)) return kTRUE;
  else if (acceptB(genL2_id, genL2_pt, genL2_eta) && acceptE(genL1_id, genL1_pt, genL1_eta)) return kTRUE;
  else return kFALSE;
}

Bool_t acceptBB(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id, Double_t genL1_pt, Double_t genL1_eta, Double_t genL2_pt, Double_t genL2_eta) {
  
  if (!isProc(proc, genV_id, genV_m, genL1_id, genL2_id)) return kFALSE;
  else if (acceptB(genL1_id, genL1_pt, genL1_eta) && acceptB(genL2_id, genL2_pt, genL2_eta)) return kTRUE;
  else return kFALSE;
}

Bool_t acceptB(Double_t genL_id, Double_t genL_pt, Double_t genL_eta) {
  if (fabs(genL_id)==11) {
    if (genL_pt>25 && fabs(genL_eta)<1.4442) return kTRUE;
    else return kFALSE;
  }
  else if (fabs(genL_id)==13) {
    if (genL_pt>25 && fabs(genL_eta)<1.2) return kTRUE;
    else return kFALSE;
  }
  return kFALSE;
}

Bool_t acceptE(Double_t genL_id, Double_t genL_pt, Double_t genL_eta) {
  if (fabs(genL_id)==11) {
    if (genL_pt>25 && fabs(genL_eta)>1.566 && fabs(genL_eta)<2.5) return kTRUE;
    else return kFALSE;
  }
  else if (fabs(genL_id)==13) {
    if (genL_pt>25 && fabs(genL_eta)>1.2 && fabs(genL_eta)<2.4) return kTRUE;
    else return kFALSE;
  }
  return kFALSE;
}

Bool_t isProc(Int_t proc, Double_t genV_id, Double_t genV_m, Double_t genL1_id, Double_t genL2_id) {
  if (proc==zee) {
    if (fabs(genL1_id)==11 && fabs(genL2_id)==11 && genV_m>60 && genV_m<120) return kTRUE;
    else return kFALSE;
  }
  else if (proc==zmm) {
    if (fabs(genL1_id)==13 && fabs(genL2_id)==13 && genV_m>60 && genV_m<120) return kTRUE;
    //if (fabs(genL1_id)==13 && fabs(genL2_id)==13 && genV_m>55 && genV_m<125) return kTRUE;
    //if (fabs(genL1_id)==13 && fabs(genL2_id)==13) return kTRUE;
    else return kFALSE;
  }
  return kFALSE;
}
