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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04SteppingAction.cc
/// \brief Implementation of the ExN04SteppingAction class
//
// $Id: ExN04SteppingAction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "ExN04SteppingAction.hh"

#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4TransportationManager.hh" 

#include "ExN04RunAction.hh"
#include "ExN04SteppingAction.hh"
#include "ExN04TrackingAction.hh"
#include "ExN04EventAction.hh"
#include "ExN04DetectorConstruction.hh"
#include "ExN04Analysis.hh"
#include "ExN04AnalysisManager.hh"
#include "ExN04PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh" 
#include "G4VProcess.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4TouchableHandle.hh"
#include <math.h>
#include "G4VParticleChange.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04SteppingAction::ExN04SteppingAction(const ExN04DetectorConstruction* detectorConstruction, ExN04EventAction* eventAction, ExN04RunAction* runAction,  ExN04PrimaryGeneratorAction* primarygeneratorAction, ExN04TrackingAction* trackingAction)
: G4UserSteppingAction(),
  fDetConstruction(detectorConstruction),
  fEventAction(eventAction),
  fRunAction(runAction),
  fTrkAction(trackingAction),
  fGenAction(primarygeneratorAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04SteppingAction::~ExN04SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04SteppingAction::UserSteppingAction(const G4Step * step)
{
  
  
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->SetPushVerbosity(0);

  //------------VARIABLES ------------------------------------// 
  //G4String volume_name = volume->GetCurrentLV();
  G4Track *track = step->GetTrack();
  G4String name = track->GetDefinition()->GetParticleName();
  G4double tracklength= track->GetTrackLength() / (m);
  G4double steplength= step->GetStepLength()/(m);
  G4double trklen_soil=0;
  G4double tot_edep=0;
  //G4String volume_name = volume->GetCurrentLV();
  /// Definizioni
  //G4double trackangle=0.;
  G4int charge = track->GetDefinition()->GetPDGCharge();
  G4int pdgcode = track->GetDefinition()->GetPDGEncoding();
  G4StepPoint* prePoint      = step->GetPreStepPoint();
  G4StepPoint* postPoint     = step->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = postPoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePrePV = prePoint->GetPhysicalVolume();
  G4ThreeVector prePointPos  = prePoint->GetPosition() / (m); 
  G4ThreeVector postPointPos = postPoint->GetPosition()/ (m);
  G4double kinene = prePoint->GetKineticEnergy() / (GeV);
  G4double edep = step->GetTotalEnergyDeposit() / (GeV); 
  G4ThreeVector momvect = prePoint->GetMomentum() / (GeV);
  const G4VProcess *proc = postPoint->GetProcessDefinedStep();
  G4String volume_name = prePoint->GetPhysicalVolume()->GetName();
  G4String material_name = prePoint->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  G4double density = prePoint->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/(g/cm3); 
  G4int parentID = track->GetParentID();
  G4int trackID  = track->GetTrackID();
  G4int StepNo = step->GetTrack()->GetCurrentStepNumber();
  G4int whichInter=-2;
  G4int whichVol=0;
  G4int boundary=0;
  G4int copyNo=-1;
  G4int creatorID=-2;
  G4int type=0;
  //G4double mass_pdg = GetDefinition->GetPDGMass();
  //G4double Imean = 142.378; //eV  (for rockMaterial)
  
  
  //------------- PHYSICAL PROCESSES-----------------------------//
  G4String Transportation = "Transportation";
  G4String Scintillation = "Scintillation";
  G4String inelastic = "inelastic";
  G4String hadElastic = "hadElastic";
  G4String nCapture = "nCapture";
  G4String hBertiniCaptureAtRest = "hBertiniCaptureAtRest";
  G4String msc = "msc";
  G4String eIoni = "eIoni";
  G4String eBrem = "eBrem";
  G4String phot = "phot";
  G4String Rayl = "Rayl";
  G4String compt = "compt";
  G4String hIoni = "hIoni";
  G4String ionIoni = "ionIoni";
  G4String neutronInelastic = "neutronInelastic";
  G4String anti_neutronInelastic = "anti_neutronInelastic";
  G4String RadioactiveDecay = "RadioactiveDecay";
  G4String Decay = "Decay";
  G4String protonInelastic = "protonInelastic";
  G4String pimenoInelastic = "pi-Inelastic";
  G4String pipiuInelastic = "pi+Inelastic";
  G4String kaonmenoInelastic = "kaon-Inelastic";
  G4String kaonpiuInelastic = "kaon+Inelastic";
  G4String conv = "conv";
  G4String CoulombScat = "CoulombScat";
  G4String dInelastic = "dInelastic";
  G4String tInelastic = "tInelastic";
  G4String He3Inelastic = "He3Inelastic";
  G4String alphaInelastic = "alphaInelastic";
  G4String kaon0LInelastic = "kaon0LInelastic";
  G4String muIoni = "muIoni";
  G4String muBrems = "muBrems";
  G4String muPairProd = "muPairProd";
  G4String muonNuclear = "muonNuclear";
  G4String photonNuclear = "photonNuclear";
  G4String muMinusCaptureAtRest = "muMinusCaptureAtRest";
  G4String muPlusCaptureAtRest = "muPlusCaptureAtRest";
  G4String creator = "null";
  if(track->GetCreatorProcess() != 0) creator = track->GetCreatorProcess()->GetProcessName();
  //-----------------------------------------------------------------------------------------//
  //--------------EVENT NUMBER----------------------------// 
  /////   Numero evento  //////////////////////
  G4int eID=0;
  const G4Event* evt = G4RunManager::GetRunManager()->GetCurrentEvent();
  if(evt) eID = evt->GetEventID();
  //G4cout << "Vale" << eID << G4endl;
  //-------------------------------------------------------------------//

  //-------- PROCESS INDEX-------------------------------------------------------//
  ////////////////////////////////////////
  if(track->GetCreatorProcess() != 0){
  if(track->GetCreatorProcess()->GetProcessName()==Transportation) creatorID = -1;
  if(track->GetCreatorProcess()->GetProcessName()==Scintillation) creatorID = 1;
  if(track->GetCreatorProcess()->GetProcessName()==msc) creatorID = 2;
  if(track->GetCreatorProcess()->GetProcessName()==eIoni) creatorID = 3;
  if(track->GetCreatorProcess()->GetProcessName()==eBrem) creatorID = 4;
  if(track->GetCreatorProcess()->GetProcessName()==phot) creatorID = 5;
  if(track->GetCreatorProcess()->GetProcessName()==Rayl) creatorID = 6;
  if(track->GetCreatorProcess()->GetProcessName()==compt) creatorID = 7;
  if(track->GetCreatorProcess()->GetProcessName()==hIoni) creatorID = 8;
  if(track->GetCreatorProcess()->GetProcessName()==ionIoni) creatorID = 9;
  if(track->GetCreatorProcess()->GetProcessName()==neutronInelastic) creatorID = 10;
  if(track->GetCreatorProcess()->GetProcessName()==protonInelastic) creatorID = 11;
  if(track->GetCreatorProcess()->GetProcessName()==RadioactiveDecay) creatorID = 12;
  if(track->GetCreatorProcess()->GetProcessName()==Decay) creatorID = 13;
  if(track->GetCreatorProcess()->GetProcessName()==hadElastic) creatorID = 14;
  if(track->GetCreatorProcess()->GetProcessName()==nCapture) creatorID = 15;
  if(track->GetCreatorProcess()->GetProcessName()==hBertiniCaptureAtRest) creatorID = 16;
  if(track->GetCreatorProcess()->GetProcessName()==pimenoInelastic) creatorID = 17;
  if(track->GetCreatorProcess()->GetProcessName()==pipiuInelastic) creatorID = 18;
  if(track->GetCreatorProcess()->GetProcessName()==conv) creatorID = 19;
  if(track->GetCreatorProcess()->GetProcessName()==CoulombScat) creatorID = 20;
  if(track->GetCreatorProcess()->GetProcessName()==dInelastic) creatorID = 21;
  if(track->GetCreatorProcess()->GetProcessName()==tInelastic) creatorID = 22;
  if(track->GetCreatorProcess()->GetProcessName()==He3Inelastic) creatorID = 23;
  if(track->GetCreatorProcess()->GetProcessName()==alphaInelastic) creatorID = 24;
  if(track->GetCreatorProcess()->GetProcessName()==kaon0LInelastic) creatorID = 25;
  if(track->GetCreatorProcess()->GetProcessName()==muIoni) creatorID = 26;
  if(track->GetCreatorProcess()->GetProcessName()==muonNuclear) creatorID = 27;
  if(track->GetCreatorProcess()->GetProcessName()==photonNuclear) creatorID = 28;
  if(track->GetCreatorProcess()->GetProcessName()==anti_neutronInelastic) creatorID = 29;
  if(track->GetCreatorProcess()->GetProcessName()==kaonmenoInelastic) creatorID = 30;
  if(track->GetCreatorProcess()->GetProcessName()==kaonpiuInelastic) creatorID = 31;
  if(track->GetCreatorProcess()->GetProcessName()==muMinusCaptureAtRest) creatorID = 32;
  if(track->GetCreatorProcess()->GetProcessName()==muPlusCaptureAtRest) creatorID = 33;
  if(track->GetCreatorProcess()->GetProcessName()==muPairProd) creatorID = 34;
  if(track->GetCreatorProcess()->GetProcessName()==muBrems) creatorID = 35;
}

  if(proc->GetProcessName()==Transportation) whichInter = -1;
  if(proc->GetProcessName()==Scintillation) whichInter = 1;
  if(proc->GetProcessName()==msc) whichInter = 2;
  if(proc->GetProcessName()==eIoni) whichInter = 3;
  if(proc->GetProcessName()==eBrem) whichInter = 4;
  if(proc->GetProcessName()==phot) whichInter = 5;
  if(proc->GetProcessName()==Rayl) whichInter = 6;
  if(proc->GetProcessName()==compt) whichInter = 7;
  if(proc->GetProcessName()==hIoni) whichInter = 8;
  if(proc->GetProcessName()==ionIoni) whichInter = 9;
  if(proc->GetProcessName()==neutronInelastic) whichInter = 10;
  if(proc->GetProcessName()==protonInelastic) whichInter = 11;
  if(proc->GetProcessName()==RadioactiveDecay) whichInter = 12;
  if(proc->GetProcessName()==Decay) whichInter = 13;
  if(proc->GetProcessName()==hadElastic) whichInter = 14;
  if(proc->GetProcessName()==nCapture) whichInter = 15;
  if(proc->GetProcessName()==hBertiniCaptureAtRest) whichInter = 16;
  if(proc->GetProcessName()==pimenoInelastic) whichInter = 17;
  if(proc->GetProcessName()==pipiuInelastic) whichInter = 18;
  if(proc->GetProcessName()==conv) whichInter = 19;
  if(proc->GetProcessName()==CoulombScat) whichInter = 20;
  if(proc->GetProcessName()==dInelastic) whichInter = 21;
  if(proc->GetProcessName()==tInelastic) whichInter = 22;
  if(proc->GetProcessName()==He3Inelastic) whichInter = 23;
  if(proc->GetProcessName()==alphaInelastic) whichInter = 24;
  if(proc->GetProcessName()==kaon0LInelastic) whichInter = 25;
  if(proc->GetProcessName()==muIoni) whichInter = 26;
  if(proc->GetProcessName()==muPairProd) whichInter = 27;
  if(proc->GetProcessName()==muonNuclear) whichInter = 28;
  if(proc->GetProcessName()==muBrems) whichInter = 29;
  //--------------------------------------------------------------------//
  //----------VOLUME INDEX----------------------------------------------//   
  if(whichVol!=0)G4String volume_name_post = postPoint->GetPhysicalVolume()->GetName();

  G4int pdgMC=TMath::Abs(13);

  /*
  if(volume_name=="unde_holes") whichVol=-1;
  if(volume_name=="unde_solids") whichVol=-2;
  /*
  if(volume_name=="boxwall") whichVol=1;
  if(volume_name=="IpogeiSurf_PV") whichVol=2;
  if(volume_name=="world_volume_PV") whichVol=3;
  */
  /*
  if(volume_name=="melogranigeo_PV") whichVol=3;
  if(volume_name=="surface") whichVol=2;
  */

  if(volume_name=="Cube") whichVol=-1;
  if(volume_name=="Soil") whichVol=-2;
  if(volume_name=="World") whichVol=3;
  if(volume_name=="myWorld") whichVol=7;
  if(volume_name=="Wall") whichVol=1;
  if(volume_name=="intWall") whichVol=4;
  if(volume_name=="outWall") whichVol=6;
  if(volume_name=="Surface") whichVol=2;
  if(volume_name=="Emulsion") whichVol=5;
  if(volume_name=="sBld1") whichVol=11;
  if(volume_name=="Bld1") whichVol=-11;
  if(volume_name=="sBld2") whichVol=12;
  if(volume_name=="Bld2") whichVol=-12;
  if(volume_name=="sBld3") whichVol=13;
  if(volume_name=="Bld3") whichVol=-13;
  if(volume_name=="sBld4") whichVol=14;
  if(volume_name=="Bld4") whichVol=-14;
  if(volume_name=="sBld5") whichVol=15;
  if(volume_name=="Bld5") whichVol=-15;
  
  if(postPoint->GetStepStatus() == fGeomBoundary && (whichVol!=0 || whichVol!=3)){
    /*
    if(thePrePV->GetName()=="IpogeiSurf_PV" && thePostPV->GetName()=="world_volume_PV") boundary=1;
    if(thePrePV->GetName()=="unde_solids" && thePostPV->GetName()=="world_volume_PV") boundary=2;
       
    if(thePrePV->GetName()=="unde_solids" && thePostPV->GetName()=="boxwall") boundary=3;//ground=1;
    if(thePrePV->GetName()=="unde_solids" && thePostPV->GetName()=="IpogeiSurf_PV") boundary=3;// ground=1;
    if(thePrePV->GetName()=="unde_holes" && thePostPV->GetName()=="boxwall") boundary=3;// ground=1;
    if(thePrePV->GetName()=="unde_holes" && thePostPV->GetName()=="IpogeiSurf_PV") boundary=3;// ground=1;
    if(thePrePV->GetName()=="IpogeiUnde_PV" && thePostPV->GetName()=="boxwall") boundary=3;// ground=1;
    if(thePrePV->GetName()=="IpogeiUnde_PV" && thePostPV->GetName()=="IpogeiSurf_PV") boundary=3;// ground=1;
    */

    if(thePrePV->GetName()=="myWorld" && thePostPV->GetName()=="World") boundary=1;
    
    if(thePrePV->GetName()=="Surface" && thePostPV->GetName()=="myWorld") boundary=1;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="myWorld") boundary=-1;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="Surface") boundary=3;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="outWall") boundary=3;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="sBld1") boundary=3;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="sBld2") boundary=3;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="sBld3") boundary=3;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="sBld4") boundary=3;
    if(thePrePV->GetName()=="Soil" && thePostPV->GetName()=="sBld5") boundary=3;
   
    
    //if(thePrePV->GetName()=="surface" && thePostPV->GetName()=="melogranigeo_PV") boundary=1;
    //if(thePrePV->GetName()=="unde_solids" && thePostPV->GetName()=="surface") boundary=3;
    //if(thePrePV->GetName()=="unde_holes" && thePostPV->GetName()=="surface") boundary=3;
    
  }

  G4double bld_density_ratio=0.06;
  G4double mean_density_buildings = bld_density_ratio*2.3+(1-bld_density_ratio)*1;  //(weighted:-->(concrete+air))
  
  if(whichVol==-2 || whichVol==4 || whichVol==6 || whichVol==11 || whichVol==12 || whichVol==13 || whichVol==14 || whichVol==15){
    fTrkAction->StepInSoil(steplength);  // density (g/cm^3) * steplength (m)    
    fTrkAction->Edep(steplength,density); 
  }
  if(whichVol==1 || whichVol==-11 || whichVol==-12 || whichVol==-13 || whichVol==-14 || whichVol==-15){
    fTrkAction->StepInSoil(steplength*bld_density_ratio);  //(weighted:-->(concrete+air))
    fTrkAction->Edep(steplength,mean_density_buildings); 
  }
  
  if(abs(pdgcode)!=pdgMC)track->SetTrackStatus(fKillTrackAndSecondaries);
  trklen_soil= fTrkAction->GetStepInSoil();
  tot_edep = fTrkAction->GetTotEdep();


  //fTrkAction->myStoppingPower(mass_pdg, charge, kinene, G4double ro, G4double Z, G4double A, Imean);


  //-------------------------------------------------------------------------------------//
  //-----------COPY NUMBER---------------------------------------------------------------//
  //if(whichVol==1 || whichVol==2 || whichVol==11 || whichVol==22)copyNo = prePoint->GetTouchableHandle()->GetCopyNumber(1);
  //-------------------------------------------------------------------------------------//  

  
  
  //------Event Start-----------------//
  if(parentID==0 && StepNo==1) {
    //G4cout << eID << G4endl;
    fRunAction->Set0();
    fRunAction->EventStart(eID,trackID,pdgcode,kinene,&prePointPos,&momvect,tracklength,trklen_soil,tot_edep);    
  }
  
  //------Event Ground-----------------//
  if(boundary==3 && abs(pdgcode)==pdgMC) {
    fRunAction->EventGround(kinene,&postPointPos,&momvect,tracklength,trklen_soil,tot_edep);   
  }
  
  //------Event Finish-----------------//
  if((abs(boundary)==1 || (kinene==edep && kinene!=0)) && abs(pdgcode)==pdgMC && whichVol!=3){
    if(boundary==1)type=1;
    if(kinene==edep && kinene!=0)type=2;
    if(boundary==-1)type=-1;
    fRunAction->EventFinish(type,kinene,&postPointPos,&momvect,tracklength,trklen_soil,tot_edep);
    fRunAction->Fill();
  }
    

  // SCOMMENTARE SOLO QUANDO SI CAMBIA UNA POSIZIONE (X VERIFICA)
  if(abs(pdgcode)==pdgMC && (StepNo==1 || (boundary>0  && (whichVol!=3 || whichVol!=0)))){
    
    /* G4cout << eID << " " << parentID << " " << trackID << " " << StepNo << " " <<  tracklength << " " << trklen_soil << " " << prePointPos[0] << " " << " " << prePointPos[1] << " " <<  prePointPos[2] << " " << material_name << " " << density << " " << volume_name << " " << boundary << " " << creator << " " <<  proc->GetProcessName() << " " << whichInter << " " << kinene << " " << tot_edep << G4endl;  */
    }
  //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
  
}
