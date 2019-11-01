#ifndef ExN04PhysicsList_h
#define ExN04PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN04PhysicsList: public G4VUserPhysicsList
{
  //aggiyìunta
public:
  ExN04PhysicsList();
  ~ExN04PhysicsList();
  //  virtual ~ExN04PhysicsList();


public:
  virtual void SetCuts();


protected:
  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
    
  // these methods Construct physics processes and register them
  virtual void ConstructGeneral();
  virtual void ConstructEM();
  virtual void ConstructHad();
  //virtual void ConstructOp();  //no optical processes 23/11/2017


  /*
  // these methods Construct all particles in each category
  virtual void ConstructAllBosons();
  virtual void ConstructAllLeptons();
  virtual void ConstructAllMesons();
  virtual void ConstructAllBaryons();
  virtual void ConstructAllIons();
  virtual void ConstructAllShortLiveds();
  */
  
  virtual void AddTransportation();
  
private:
  G4int VerboseLevel;
  G4int OpVerbLevel;
  
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;

  // these methods Construct particles 
  void ConstructMyBosons();
  void ConstructMyLeptons();
  void ConstructMyHadrons();
  void ConstructMyShortLiveds(); 




 /*
public:
  ExN04PhysicsList();
  virtual ~ExN04PhysicsList();

  // Construct particle and physics
  virtual void ConstructParticle();
  virtual void ConstructProcess();
 
  virtual void SetCuts();
   
private:
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  
  // these methods Construct physics processes and register them
  void ConstructDecay();
  void AddTransportation();
  void ConstructEM();
  void ConstructHad();
  */
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif



