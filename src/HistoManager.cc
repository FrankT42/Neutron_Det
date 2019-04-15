#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "G4UnitsTable.hh"
#include "g4root.hh"
#include <fstream>
#include <iostream>
//#include "G4VAnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "SteppingAction.hh"
#include <sstream>
#include <algorithm>

extern G4double SiPM_pitch;
extern G4int SiPM_n;
extern G4int SiPM_m;
extern G4double SiPM_sep;
extern G4String filename;

//this is kinda cheeting, but extern just tells the compiler that
//it is defined somewhare and is non static and in file scope
//so this just takes my array details from the DetectorConstruction
//makes life easier but is harder to debug for mortals

//==================================
HistoManager::HistoManager()
  :fFactoryOn(false)
{
  std::ofstream fout("./Outputs/Pulses.dat");
}
//==================================
HistoManager::~HistoManager()
{}
//==================================

void HistoManager::Book()
{
  //make the outputs
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
  //This is what makes multithreading work
  analysisManager->SetHistoDirectoryName("histo");
  analysisManager->SetNtupleDirectoryName("ntuple");

  G4bool fileOpen = analysisManager->OpenFile(filename);
  if(!fileOpen){
    G4cerr<<"\n---->Cannot open "<<analysisManager->GetFileName()<<G4endl;
    return;
  }
  //make histograms
  analysisManager->CreateH1("Edep", "Edep in Scintillator (MeV)",300,0,6*MeV);
  analysisManager->CreateH1("Scint","Number of Scintillation Photons",200,0,100000);
  analysisManager->CreateH1("SiPM","Number of Photons on SiPM",1024,0,100000);
  analysisManager->CreateH1("x", "xposition", 512, -(SiPM_n*(SiPM_pitch+SiPM_sep))*mm, (SiPM_n*(SiPM_pitch+SiPM_sep))*mm);
  analysisManager->CreateH1("y", "yposition", 512, -(SiPM_m*(SiPM_pitch+SiPM_sep))*mm, (SiPM_m*(SiPM_pitch+SiPM_sep))*mm);
  analysisManager->CreateH1("time", "time1", 20000, -50*ns, 5*us);
  analysisManager->CreateH1("time2","time2", 1030, 0, 1*ns);
  analysisManager->CreateH2("location", "loc", 1024, -(SiPM_n*(SiPM_pitch+SiPM_sep))*mm,(SiPM_n*(SiPM_pitch+SiPM_sep))*mm,1024,-(SiPM_m*(SiPM_pitch+SiPM_sep))*mm,(SiPM_m*(SiPM_pitch+SiPM_sep))*mm);
  //make ntuples
  analysisManager->CreateNtuple("Ntuple1", "Edep");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleDColumn("Scint");
  analysisManager->CreateNtupleDColumn("SiPM");
  analysisManager->FinishNtuple();
  //Time and loc
  analysisManager->CreateNtuple("Ntuple2","Time");
  analysisManager->CreateNtupleDColumn("Time1");
  analysisManager->CreateNtupleDColumn("Time2");
  analysisManager->FinishNtuple();
 
  
  fFactoryOn = true;
  G4cout<<"\n---->Output File is Open in "<<analysisManager->GetFileName()<<"."
	<<analysisManager->GetFileType()<<G4endl;

  //  std::ofstream fout;
  //fout.open("./Outputs/Pulses.dat");
						
}

//=========================================
void HistoManager::Save()
{
  if(!fFactoryOn) return;
  //saves stuff self evident
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  // fout.close();
  G4cout<<"\n---->Histograms and ntuples saved\n"<<G4endl;
}
void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  //what is says on the tin
  //currently just gives total but can be made sensor independent
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(ih,xbin,weight);
 
}
void HistoManager::FillNtuple(G4double Edep, G4int Scint,G4int SiPM_num){
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //what it says on the tin
  if(Edep>0){
  analysisManager->FillNtupleDColumn(0,0,Edep);
  analysisManager->FillNtupleDColumn(0,1,Scint);
  analysisManager->FillNtupleDColumn(0,2,SiPM_num);
  analysisManager->AddNtupleRow(0);
  }
}
void HistoManager::FillTimeAndLoc(std::vector<G4double> x, std::vector<G4double> y, std::vector<G4double>t1,std::vector<G4double> t2)
{
  //this gets the location and time data
  //this is not a single data point
  //so needs some processing
  //it can also output raw pulses to file
  //if this is what your into
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  std::stringstream ss;
  for(int i = 0; i<x.size();i++){
    analysisManager->FillH1(3,x[i],1);
    analysisManager->FillH1(4,y[i],1);
    analysisManager->FillH2(0,y[i],x[i],1);

  }
  if(t1.size()>0){
  auto min = std::min_element(std::begin(t1),std::end(t1));
  auto mn = std::distance(t1.begin(), min);
  std::cout<<t1[mn]<<std::endl;
    for(int i = 0; i<t1.size();i++){
    analysisManager->FillH1(5,t1[i]-t1[mn],1);
    if(t1[i]<10*us){
    analysisManager->FillNtupleDColumn(1,0,t1[i]-t1[mn]);
    }
    analysisManager->FillH1(6,t2[i],1);
	if(t2[i]<10*us){
    analysisManager->FillNtupleDColumn(1,1,t2[i]);
    ss<<t1[i]<<" ";
	}
    analysisManager->AddNtupleRow(1);
    // SteppingAction::GetStream()<<std::endl;
    
    }
  }
    ss<<std::endl;
    SteppingAction::GetStream()<<ss.str();
    //        for(int i = 0; i<t2.size();i++){
    // analysisManager->FillH1(6,t2[i],1);
    //analysisManager->FillNtupleDColumn(1,1,t2[i]);

    //}
    //	analysisManager->AddNtupleRow(1);
}
