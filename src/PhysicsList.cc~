#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysListFactory.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4HadronPhysicsQGSP_BIC.hh"
//#include "HadronPhysicsQGSP_BIC.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"
#include "G4OpticalPhysics.hh"
#include "G4Threading.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"

#include "G4ParticleHPElastic.hh"

#include "G4HadronElasticProcess.hh"

#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPThermalScattering.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ProcessTable.hh"
#include "G4HadronFissionProcess.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"


#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"
//===============================================


//This is my PhysicsList. There are many like it, but this one is mine
//Without my PhysicsList my simulation is useless. My PhysicsList
//must shoot straighter than its enemy.


//This is my PhysicsList, it is modular and can deal with most problems that I can forsee
//being seen: neutron, gamma, ion, hadron and photonuclear is represented
//there is some modifications, mainly to change the pulse shape for neutrons and gamma
//for scintillation, which means changing for alpha and ion
//this will not effect the light output, just the time, so don't worry about it
//unless you need accurate PSD info, then the values must be set
//These values will interact with the one in DetectorConstruction

PhysicsList::PhysicsList()
  :G4VModularPhysicsList()
{
  fOptical = 0;
  fMaxNumPhotonStep = 50;

  pMessenger = new PhysicsListMessenger(this);

  G4LossTableManager::Instance();

  defaultCutValue = 0.1*mm;
  cutForGamma = defaultCutValue;
  cutForElectron = defaultCutValue;
  cutForPositron = defaultCutValue;

  helIsRegistered = false;
  bicIsRegistered = false;
  biciIsRegistered = false;

  locIonIonInelasticIsRegistered = false;
  radioactiveDecayIsRegistered = false;

  SetVerboseLevel(1);

  //EM Physics
  emPhysicsList = new G4EmStandardPhysics_option3(1);
  emName = G4String("QGSP_BIC_EMY");

  decPhysicsList = new G4DecayPhysics();
  raddecayList = new G4RadioactiveDecayPhysics();
  
}

PhysicsList::~PhysicsList()
{
  delete pMessenger;
  delete emPhysicsList;
  delete decPhysicsList;
  delete raddecayList;
  for(size_t i = 0; i<hadronPhys.size();i++){delete hadronPhys[i];}
}
void PhysicsList::ConstructParticle()
{
  decPhysicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  emPhysicsList->ConstructProcess();
  em_config.AddModels();

  decPhysicsList->ConstructProcess();
  raddecayList->ConstructProcess();

  for(size_t i = 0;i<hadronPhys.size();i++)
    {
      hadronPhys[i]->ConstructProcess();
    }
  ConstructOptical();
  G4bool N_PHYS = true;
//add neutron stuff

  if(N_PHYS){
 G4ParticleDefinition* neutron = G4Neutron::Neutron();
G4ProcessManager* pManager = neutron->GetProcessManager();
  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();
  G4VProcess* process = 0;
  process = processTable->FindProcess("hadElastic", neutron);      
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("neutronInelastic", neutron);
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("nCapture", neutron);      
  if (process) pManager->RemoveProcess(process);
  //
  process = processTable->FindProcess("nFission", neutron);      
  if (process) pManager->RemoveProcess(process);      
         
  // (re) create process: elastic
  //
  G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
  pManager->AddDiscreteProcess(process1);
  //
  // model1a
  G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
  process1->RegisterMe(model1a);
  process1->AddDataSet(new G4ParticleHPElasticData());
  //
  // model1b
  if (true) {
    model1a->SetMinEnergy(4*eV);   
    G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
    process1->RegisterMe(model1b);
    process1->AddDataSet(new G4ParticleHPThermalScatteringData());         
  }
   
  // (re) create process: inelastic
  //
  G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
  pManager->AddDiscreteProcess(process2);   
  //
  // cross section data set
  G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
  process2->AddDataSet(dataSet2);                               
  //
  // models
  G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
  process2->RegisterMe(model2);    

  // (re) create process: nCapture   
  //
  G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
  pManager->AddDiscreteProcess(process3);    
  //
  // cross section data set
  G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
  process3->AddDataSet(dataSet3);                               
  //
  // models
  G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
  process3->RegisterMe(model3);
   
  // (re) create process: nFission   
  //
  G4HadronFissionProcess* process4 = new G4HadronFissionProcess();
  pManager->AddDiscreteProcess(process4);    
  //
  // cross section data set
  G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
  process4->AddDataSet(dataSet4);                               
  //
  // models
  G4ParticleHPFission* model4 = new G4ParticleHPFission();
  process4->RegisterMe(model4);       


  }
  }

void PhysicsList::AddPhysicsList(const G4String& name)
{

  if (verboseLevel>1) {
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
  }
  if (name == emName) return;

  if (name == "standard_opt3") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmStandardPhysics_option3();
    G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmStandardPhysics_option3" << G4endl;

  } else if (name == "LowE_Livermore") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmLivermorePhysics();
    G4RunManager::GetRunManager()-> PhysicsHasBeenModified();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmLivermorePhysics" << G4endl;

  } else if (name == "LowE_Penelope") {
    emName = name;
    delete emPhysicsList;
    emPhysicsList = new G4EmPenelopePhysics();
    G4RunManager::GetRunManager()-> PhysicsHasBeenModified();
    G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: G4EmPenelopePhysics" << G4endl;
    
  } else if (name == "QGSP_BIC_EMY") {
    AddPhysicsList("emstandard_opt3");
    hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());

    //hadronPhys.push_back( new HadronPhysicsQGSP_BIC());
    hadronPhys.push_back( new G4EmExtraPhysics());
    hadronPhys.push_back( new G4HadronElasticPhysics());
    hadronPhys.push_back( new G4StoppingPhysics());
    hadronPhys.push_back( new G4IonBinaryCascadePhysics());
    hadronPhys.push_back( new G4NeutronTrackingCut());
  }
else if (name == "QGSP_BIC_EMY_CAP") {
    AddPhysicsList("emstandard_opt3");
    hadronPhys.push_back( new G4HadronPhysicsQGSP_BIC());
    // G4ParticleHPThermalScattering* model1 = new G4ParticleHPThermalScattering();

    //hadronPhys.push_back( new HadronPhysicsQGSP_BIC());
    hadronPhys.push_back( new G4EmExtraPhysics());
    hadronPhys.push_back( new G4HadronElasticPhysics());
    hadronPhys.push_back( new G4StoppingPhysics());
    hadronPhys.push_back( new G4IonBinaryCascadePhysics());
    hadronPhys.push_back( new G4NeutronTrackingCut());
    //    hadronPhys.push_back( model1);
    // hadronPhys.push_back(new G4NeutronInelasticProcess());
    //hardonPhys.push_back(new G4ParticleHPInelastic());
    //hadronPhys.push_back(new G4HadronCaptureProcess());
    //hadronPhys.push_back(new G4ParticleHPCapture());
    // hadronPhys.push_back(new G4HadronFissionProcess());
    
 }
 else { 
    G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
	   << " is not defined"
	   << G4endl;
  }
}

//---------------------------------------------------------------------------

void PhysicsList::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");

  // Set cuts for detector
  if (verboseLevel>0) DumpCutValuesTable();
}

//---------------------------------------------------------------------------

void PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

//---------------------------------------------------------------------------

void PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

//---------------------------------------------------------------------------

void PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}

//---------------------------------------------------------------------------

void PhysicsList::ConstructPhotoNuclear()
{
  G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
  G4PhotoNuclearProcess* process = new G4PhotoNuclearProcess();
  G4CascadeInterface* bertini = new G4CascadeInterface();
  bertini->SetMaxEnergy(10*GeV);
  process->RegisterMe(bertini);
  pManager->AddDiscreteProcess(process);
}

//---------------------------------------------------------------------------

void PhysicsList::ConstructOptical()
{
  G4Cerenkov* cerenkovProcess = new G4Cerenkov("Cerenkov");
  cerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  cerenkovProcess->SetMaxBetaChangePerStep(10.0);
  cerenkovProcess->SetTrackSecondariesFirst(true);
  G4Scintillation* scintillationProcess = new G4Scintillation("Scintillation");
  scintillationProcess->SetScintillationYieldFactor(0.95);
  scintillationProcess->SetTrackSecondariesFirst(true);
  G4Scintillation* NscintillationProcess = new G4Scintillation("Scintillation");
  NscintillationProcess->SetScintillationYieldFactor(.65);
  NscintillationProcess->SetTrackSecondariesFirst(true);
  G4OpAbsorption* absorptionProcess = new G4OpAbsorption();
  G4OpRayleigh* rayleighScatteringProcess = new G4OpRayleigh();
  G4OpMieHG* mieHGScatteringProcess = new G4OpMieHG();
  G4OpBoundaryProcess* boundaryProcess = new G4OpBoundaryProcess();

  // Use Birks Correction in the Scintillation process
  if(!G4Threading::IsWorkerThread())
  {
    G4EmSaturation* emSaturation =
              G4LossTableManager::Instance()->EmSaturation();
      scintillationProcess->AddSaturation(emSaturation);
  }

  auto aParticleIterator=GetParticleIterator();
  aParticleIterator->reset();
  while( (*aParticleIterator)() ){
    G4ParticleDefinition* particle = aParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (cerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(cerenkovProcess);
      pmanager->SetProcessOrdering(cerenkovProcess,idxPostStep);
    }
    if (scintillationProcess->IsApplicable(*particle)) {
      if(particleName == "neutron"||particleName=="alpha"||particleName=="ion"){
	pmanager->AddProcess(NscintillationProcess);
      }
      else{
	pmanager->AddProcess(scintillationProcess);
      }
      pmanager->SetProcessOrderingToLast(scintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(scintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(absorptionProcess);
      pmanager->AddDiscreteProcess(rayleighScatteringProcess);
      pmanager->AddDiscreteProcess(mieHGScatteringProcess);
      pmanager->AddDiscreteProcess(boundaryProcess);
    }
  }
}

//---------------------------------------------------------------------------

