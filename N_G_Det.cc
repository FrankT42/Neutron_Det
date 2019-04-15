#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
//=============================================
//             Detector Simulation
//                 April 2018
//
//=============================================
//  Author:Frank Thomson
//  Simulation of Scintillation Detector
//  with neutron sensitivity and optical
//  transport
//=============================================

#ifndef G4MULTITHREADED
void INThandeler(int); //neded for interupts
#endif
G4RunManager* runManager;
G4String filename = "./NPG_Det.root";
std::mutex mu; //i created a mutex to make things thread safe if
               //needed, but not sure if Geant4 uses std::thread
               //as a backend

namespace { 
  void PrintUsage() { 
    G4cerr << " Usage: " << G4endl; 
    G4cerr << "  [-m macro ] [-u UIsession] [-t nThreads] [-r seed]  [-f output Dir] " 
           << G4endl; 
    G4cerr << "   note: -t option is available only for multi-threaded mode." 
           << G4endl; 
  } 
} 



int main(int argc, char** argv){

  #ifndef G4MULTITHREADED
  signal(SIGINT,INThandeler);//listen for termination
  #endif
  
  G4UIExecutive* ui = 0;

  if(argc == 1){
    ui = new G4UIExecutive(argc,argv);
  }

  G4String macro; 
  G4String session; 
#ifdef G4MULTITHREADED 
  G4int nThreads = 8; 
#endif 
 
  G4long myseed = 345354; 
  for ( G4int i=1; i<argc; i=i+2 ) { 
    if      ( G4String(argv[i]) == "-m" ) macro   = argv[i+1]; 
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1]; 
    else if ( G4String(argv[i]) == "-r" ) myseed  = atoi(argv[i+1]); 
    else if ( G4String(argv[i]) == "-f" ) filename = argv[i+1];
    #ifdef G4MULTITHREADED 
     else if ( G4String(argv[i]) == "-t" ) { 
               nThreads = G4UIcommand::ConvertToInt(argv[i+1]); 
     } 
    #endif 
    // else { 
    //PrintUsage(); 
    //return 1; 
    //} 
  } 

  // Choose the Random engine 
  // 
  G4Random::setTheEngine(new CLHEP::RanecuEngine); 


  #ifdef G4MULTITHREADED
  //G4int nThreads = 8;//I just make it 8, can go to any value
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(nThreads);
  #else
  G4RunManager* runManager = new G4RunManager;
  runManager = new G4RunManager;
  #endif

  //Lets get the mandatory stuff done
  DetectorConstruction* det = new DetectorConstruction;
  runManager->SetUserInitialization(det);
  runManager->SetUserInitialization(new PhysicsList);
  runManager->SetUserInitialization(new ActionInitialization(det));

  //Make it SO!

  //Turn on the Telly (initialize visulization, but less posh)

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();//This shoudl be read in Brian Blessed's voice

  //Get the pointer to the UI, we will need that later
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if(!ui){//Damned thing is in batch mode, someone get a shovel
    G4String command = "/control/execute ";
    G4String fileName = macro; //argv[1]; //I know, my old way was cool, it will be back
    UImanager->ApplyCommand(command+fileName);
    
  }
  else{//Interactive mode. It must be too lonely in batch mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;//It was too good for this world :-(
  }
  //Termination...Find Sarah Connor
  delete visManager;
  delete runManager;

  return 0;//Skynet FTW

}
//=============================================
//           End of Main Method
//=============================================

//Interupt handeling
//this will cause all currently
//processed itterations to be written to
//file safely.
//Only works in single thread mode
//hence not defined for multithread

#ifndef G4MUTITHREADED
void INThandeler(int sig){
  char c;
  mu.lock();
  signal(sig, SIG_IGN);
  //  std:: cin.get();
  printf("Did you hit Ctrl-C?\n"
	 "Do you really want to Quit?[y/n] ");
  c = getchar();
  mu.unlock();
  if(c== 'y'|| c== 'Y')
    {
      //delete VisManager;
      delete runManager;
      exit(0);
    }
      else
	{
	  signal(SIGINT, INThandeler);
	  getchar();
	}
  
    }
#endif
