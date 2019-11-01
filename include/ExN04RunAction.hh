//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file eventgenerator/HepMC/HepMCEx01/include/ExN04RunAction.hh
/// \brief Definition of the ExN04RunAction class
//
// $Id: ExN04RunAction.hh 77801 2013-11-28 13:33:20Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ExN04RunAction_h
#define ExN04RunAction_h 1

#include "G4UserRunAction.hh"
#include "TTree.h"
#include "TFile.h"
#include "globals.hh"
#include "G4Event.hh"

class G4Run;
class ExN04HistoManager;
//aggiunte
class ExN04RunActionMessenger;
class G4Run;
//class ExN04AnalysisManager;


struct Particle
{
  int eventid;
  int trackid;
  int pdg;
  int type;      // 0- undefined, 1-pass-through, 2-absorbed
  };
  
struct TrackHit
{
  float x,y,z;               // point [?]
  float px,py,pz;            // momentum in this point[Gev/c]
  float e;                   // energy [Gev]
  float path;                // integral path of the track from start upto this point in [m]
  float effpath;             // integral path of the track from start upto this point in [m] only in materials
  float edep;                // integral energy deposit of the track from start upto this point in [GeV/m]
  int   flag;
};


class ExN04RunAction : public G4UserRunAction {
public:
  
  ExN04RunAction();
  ~ExN04RunAction(); 
  
  virtual void BeginOfRunAction(const G4Run*);
  virtual void   EndOfRunAction(const G4Run*);
  
  void SetsavehitsFile   (G4String val)        { savehitsFile   = val;};
  void SetsavepmtFile    (G4String val)        { savepmtFile    = val;};
  void SetsavehistFile   (G4String val)        { savehistFile   = val;};
  void Finish();	
  
  G4String GetsavehitsFile()                   {return savehitsFile;};
  G4String GetsavepmtFile()                    {return savepmtFile;};
   
  //void EventInfoMC(G4int iPartName, G4int iCreator, G4int iInteraction,G4double iEnergy, G4int iStep, G4int iTrackId, G4int iParentId, G4int iVolume, G4ThreeVector *Mom);

  //void EventInfoBoundary(G4int iPartName, G4int iCreator, G4int iInteraction,G4double iEnergy, G4int iStep, G4int iTrackId, G4int iParentId, G4int iBoundaryId, G4ThreeVector *Mom, G4ThreeVector *LastPos);
  
  //void EventInfoRecoilLast(G4double iEnergyRecoil, G4ThreeVector *Pos, G4ThreeVector *Mom, G4double iTrackLength, G4int iPartName, G4int iVolume, G4double iStepLength, G4int iBoundary);    
  

  //void ClearVectors();

  TFile    *eFileMC;
  TTree    *eTree;
  Particle eParticle;
  TrackHit eTrackStart;
  TrackHit eTrackFinish;
  TrackHit eTrackZ0;
  
  void Set0() { eParticle.eventid=0; eParticle.trackid=0; eParticle.pdg=0, eParticle.type=0;
    eTrackStart.x=0; eTrackStart.y=0; eTrackStart.z=0; eTrackStart.px=0; eTrackStart.py=0;eTrackStart.pz=0; eTrackStart.e=0; eTrackStart.path=0; eTrackStart.effpath=0, eTrackStart.edep=0;
    eTrackZ0.x=0; eTrackZ0.y=0; eTrackZ0.z=0; eTrackZ0.px=0; eTrackZ0.py=0;eTrackZ0.pz=0; eTrackZ0.e=0; eTrackZ0.path=0; eTrackZ0.effpath=0, eTrackZ0.edep=0;
    eTrackFinish.x=0; eTrackFinish.y=0; eTrackFinish.z=0; eTrackFinish.px=0; eTrackFinish.py=0; eTrackFinish.pz=0; eTrackFinish.e=0; eTrackFinish.path=0; eTrackFinish.effpath=0, eTrackFinish.edep=0;
  } // reset to the default values all variables
  void MakeTree();
  void Fill() { if(eTree) eTree->Fill(); }
  void EventStart(G4int iEvent, G4int iTrackId, G4int iPartName, G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4double iStepLength,  G4double iEnergyDeposit);
  void EventGround( G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4double iStepLength, G4double iEnergyDeposit);
  void EventFinish(G4int iType, G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4double iStepLength, G4double iEnergyDeposit);
  

private:
  G4String savehitsFile;
  G4String savepmtFile;
  G4String savehistFile;
  ExN04RunActionMessenger* runMessenger;
  
  G4int Nneu;
  G4int Nrec;
  G4int Nrec2;
};

  /*
  std::vector<G4int>    vMC_CreatorId;
  std::vector<G4int>    vMC_InteractionId;
  std::vector<G4double> vMC_EnergyKin;
  std::vector<G4int>    vMC_StepId;         
  std::vector<G4int>    vMC_TrackId;      
  std::vector<G4int>    vMC_ParentId;     
  std::vector<G4int>    vMC_VolumeId;
  std::vector<G4double> vMC_PX;    
  std::vector<G4double> vMC_PY;    
  std::vector<G4double> vMC_PZ;
  std::vector<G4int>    vMC_PartId;


  std::vector<G4int>    vBN_CreatorId;
  std::vector<G4int>    vBN_InteractionId;
  std::vector<G4double> vBN_EnergyKin;
  std::vector<G4int>    vBN_StepId;         
  std::vector<G4int>    vBN_TrackId;      
  std::vector<G4int>    vBN_ParentId;     
  std::vector<G4int>    vBN_BoundaryId;
  std::vector<G4double> vBN_PX;    
  std::vector<G4double> vBN_PY;    
  std::vector<G4double> vBN_PZ;
  std::vector<G4double> vBN_PostHitX;     
  std::vector<G4double> vBN_PostHitY;     
  std::vector<G4double> vBN_PostHitZ; 
  std::vector<G4int>    vBN_PartId;
  
  std::vector<G4int>    vCreatorId;
  std::vector<G4int>    vInteractionId;
  std::vector<G4double> vEnergyKin;
  std::vector<G4double> vEnergyDep; 
  std::vector<G4int>    vStepId;   
  std::vector<G4int>    vTrackId;
  std::vector<G4int>    vParentId;
  std::vector<G4double> vPreHitX;     
  std::vector<G4double> vPreHitY;     
  std::vector<G4double> vPreHitZ;
  std::vector<G4double> vPostHitX;     
  std::vector<G4double> vPostHitY;     
  std::vector<G4double> vPostHitZ;     
  std::vector<G4double> vHitPX;    
  std::vector<G4double> vHitPY;    
  std::vector<G4double> vHitPZ;    
  std::vector<G4double> vTrackLength;
  std::vector<G4double> vStepLength;    
  std::vector<G4int>    vPartId;      
  std::vector<G4int>    vVolumeId;
  std::vector<G4int>    vCopyNo;
  std::vector<G4int>    vBoundaryId;

  std::vector<G4int>    vCreatorLast;
  std::vector<G4int>    vInteractionLast;
  std::vector<G4double> vEnergyKinLast;
  std::vector<G4double> vEnergyDepLast; 
  std::vector<G4int>    vStepIdLast;   
  std::vector<G4int>    vTrackIdLast;
  std::vector<G4int>    vParentIdLast;
  std::vector<G4double> vLastHitX;     
  std::vector<G4double> vLastHitY;     
  std::vector<G4double> vLastHitZ;
  std::vector<G4double> vPostLastHitX;     
  std::vector<G4double> vPostLastHitY;     
  std::vector<G4double> vPostLastHitZ;     
  std::vector<G4double> vLastHitPX;    
  std::vector<G4double> vLastHitPY;    
  std::vector<G4double> vLastHitPZ;    
  std::vector<G4double> vTrackLengthLast;    
  std::vector<G4int>    vPartNameLast;      
  std::vector<G4int>    vVolumeLast;
  std::vector<G4int>    vCopyNoLast;

};


// inline functions
inline void ExN04RunAction::ClearVectors(){

  vMC_CreatorId.clear();
  vMC_InteractionId.clear();
  vMC_EnergyKin.clear();
  vMC_StepId.clear();         
  vMC_TrackId.clear();      
  vMC_ParentId.clear();     
  vMC_VolumeId.clear();
  vMC_PX.clear();
  vMC_PY.clear();
  vMC_PZ.clear();
  vMC_PartId.clear();

  vBN_CreatorId.clear();
  vBN_InteractionId.clear();
  vBN_EnergyKin.clear();
  vBN_StepId.clear();         
  vBN_TrackId.clear();      
  vBN_ParentId.clear();     
  vBN_BoundaryId.clear();
  vBN_PX.clear();
  vBN_PY.clear();
  vBN_PZ.clear();
  vBN_PostHitX.clear();
  vBN_PostHitY.clear();
  vBN_PostHitZ.clear();
  vBN_PartId.clear();
  

  
  vCreatorId.clear();
  vInteractionId.clear();
  vEnergyKin.clear();
  vEnergyDep.clear();
  vStepId.clear();
  vTrackId.clear();
  vParentId.clear();
  vPreHitX.clear();
  vPreHitY.clear();
  vPreHitZ.clear();
  vPostHitX.clear();
  vPostHitY.clear();
  vPostHitZ.clear();
  vHitPX.clear();
  vHitPY.clear();
  vHitPZ.clear();
  vTrackLength.clear();
  vStepLength.clear();
  vPartId.clear();
  vVolumeId.clear();
  vCopyNo.clear();
  vBoundaryId.clear();

  vCreatorLast.clear();
  vInteractionLast.clear();
  vEnergyKinLast.clear();
  vEnergyDepLast.clear();
  vStepIdLast.clear();
  vTrackIdLast.clear();
  vParentIdLast.clear();
  vLastHitX.clear();
  vLastHitY.clear();
  vLastHitZ.clear();
  vPostLastHitX.clear();
  vPostLastHitY.clear();
  vPostLastHitZ.clear();
  vLastHitPX.clear();
  vLastHitPY.clear();
  vLastHitPZ.clear();
  vTrackLengthLast.clear();
  vPartNameLast.clear();
  vVolumeLast.clear();
  vCopyNoLast.clear();

}

/*
inline void ExN04RunAction::EventInfoMC(G4int iPartName, G4int iCreator, G4int iInteraction,G4double iEnergy, G4int iStep, G4int iTrackId, G4int iParentId, G4int iVolume, G4ThreeVector *Mom) {

  vMC_PartId.push_back(iPartName);
  vMC_CreatorId.push_back(iCreator);
  vMC_InteractionId.push_back(iInteraction);
  vMC_EnergyKin.push_back(iEnergy);
  vMC_StepId.push_back(iStep);
  vMC_TrackId.push_back(iTrackId);
  vMC_ParentId.push_back(iParentId);
  vMC_VolumeId.push_back(iVolume);
  vMC_PX.push_back(Mom->x());
  vMC_PY.push_back(Mom->y());
  vMC_PZ.push_back(Mom->z());
}


inline void ExN04RunAction::EventInfoRecoilLast( G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4int iPartName, G4int iVolume, G4double iStepLength, G4int iBoundary){

  //vCreatorId.push_back(iCreator);
  //vInteractionId.push_back(iInteraction);
  vEnergyKin.push_back(iEnergyRecoil);
  //vEnergyDep.push_back(iEnergyDep);
  //vStepId.push_back(iStep);
  //vTrackId.push_back(iTrackId);
  //vParentId.push_back(iParentId);
  vPreHitX.push_back(PrePos->x());
  vPreHitY.push_back(PrePos->y());
  vPreHitZ.push_back(PrePos->z());
  //vPostHitX.push_back(PostPos->x());
  //vPostHitY.push_back(PostPos->y());
  //vPostHitZ.push_back(PostPos->z());
  vHitPX.push_back(Mom->x());
  vHitPY.push_back(Mom->y());
  vHitPZ.push_back(Mom->z());
  vTrackLength.push_back(iTrackLength);
  vStepLength.push_back(iStepLength);
  vPartId.push_back(iPartName);
  vVolumeId.push_back(iVolume);
  //vCopyNo.push_back(iCopyNo);
  vBoundaryId.push_back(iBoundary);
  }*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


inline void ExN04RunAction::MakeTree()
{
  eTree = new TTree("treemc","treemc");
  eTree->Branch("p" , &eParticle,    "eventid/I:trackid/I:pdg/I:type/I");
  eTree->Branch("ts", &eTrackStart,  "x/F:y/F:z/F:px/F:py/F:pz/F:e/F:path/F:effpath/F:edep/F");
  eTree->Branch("tf", &eTrackFinish, "x/F:y/F:z/F:px/F:py/F:pz/F:e/F:path/F:effpath/F:edep/F");
  eTree->Branch("tz0",&eTrackZ0,     "x/F:y/F:z/F:px/F:py/F:pz/F:e/F:path/F:effpath/F:edep/F");
}

inline void ExN04RunAction::EventStart(G4int iEvent, G4int iTrackId, G4int iPartName, G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4double iStepLength, G4double iEnergyDeposit){

  eParticle.eventid = iEvent;
  eParticle.trackid = iTrackId;
  eParticle.pdg = iPartName;

  eTrackStart.x = PrePos->x();
  eTrackStart.y = PrePos->y();
  eTrackStart.z = PrePos->z();

  eTrackStart.px = Mom->x();
  eTrackStart.py = Mom->y();
  eTrackStart.pz = Mom->z();

  eTrackStart.e = iEnergyRecoil;
  eTrackStart.path = iTrackLength;
  eTrackStart.effpath = iStepLength;
  eTrackStart.edep = iEnergyDeposit;
  
}

inline void ExN04RunAction::EventGround( G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4double iStepLength, G4double iEnergyDeposit){

  eTrackZ0.x = PrePos->x();
  eTrackZ0.y = PrePos->y();
  eTrackZ0.z = PrePos->z();

  eTrackZ0.px = Mom->x();
  eTrackZ0.py = Mom->y();
  eTrackZ0.pz = Mom->z();

  eTrackZ0.e = iEnergyRecoil;
  eTrackZ0.path = iTrackLength;
  eTrackZ0.effpath = iStepLength;
  eTrackZ0.edep = iEnergyDeposit;

}

inline void ExN04RunAction::EventFinish(G4int iType, G4double iEnergyRecoil, G4ThreeVector *PrePos, G4ThreeVector *Mom, G4double iTrackLength, G4double iStepLength, G4double iEnergyDeposit){

  eParticle.type = iType;
  
  eTrackFinish.x = PrePos->x();
  eTrackFinish.y = PrePos->y();
  eTrackFinish.z = PrePos->z();

  eTrackFinish.px = Mom->x();
  eTrackFinish.py = Mom->y();
  eTrackFinish.pz = Mom->z();

  eTrackFinish.e = iEnergyRecoil;
  eTrackFinish.path = iTrackLength;
  eTrackFinish.effpath = iStepLength;
  eTrackFinish.edep = iEnergyDeposit;

}

#endif
