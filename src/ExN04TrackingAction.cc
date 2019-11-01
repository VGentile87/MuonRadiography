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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04TrackingAction.cc
/// \brief Implementation of the ExN04TrackingAction class
//
// $Id: ExN04TrackingAction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "ExN04TrackingAction.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  // ExN04TrackingAction*  fTrackingAction;
  //fTrackingAction->StepInSoilInitialize();
  stepl=0;
  tot_edep=0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  //G4cout << "POST USER TRACKING ACTION!!!!!!!!!!" << G4endl;

  // step information
  //const G4Step* step = track->GetStep();
  //G4double stepl = step->GetStepLength();
  //G4cout << stepl << G4endl;
  G4int TrackID = track->GetTrackID();
  //G4double  Ekin = track->GetKineticEnergy();
  G4int ParentID = track->GetParentID();
  //G4int pdg = track->GetParticleDefinition()->GetPDGEncoding();
  // G4int whichEmu=0;
  G4String name = track->GetDefinition()->GetParticleName();
  //G4double gunenergy = fGenAction->fparticleGun->GetParticleEnergy();
  //G4double tracklength= track->GetTrackLength()/(um);
  //G4int charge = track->GetDefinition()->GetPDGCharge();
  //G4StepPoint* prePoint      = step->GetPreStepPoint();
  //G4StepPoint* postPoint     = step->GetPostStepPoint();
  //const G4VProcess *proc = postPoint->GetProcessDefinedStep();
  G4String transportation = "Transportation";
  G4String scintillation = "Scintillation";
  G4String inelastic = "inelastic";
  G4String elastic = "hadElastic";
  G4String nCapture = "nCapture";

  G4String creator = "null";
  if(track->GetCreatorProcess() != 0)
    creator = track->GetCreatorProcess()->GetProcessName();

  

  if(name=="neutron" && ParentID==0){
    //if(proc->GetProcessName()==elastic) whichEmu = 1;
    //if(proc->GetProcessName()==inelastic) whichEmu = 2;
    //if(proc->GetProcessName()==nCapture) whichEmu = 3;
  }


  // G4cout <<"\n-------> I am executing POST_ UserTrackingAction <---------\n" 
  // 	 <<" stepl = " <<G4BestUnit(stepl,"Length")
  // 	 << " Volume in which the particle is : " 
  // 	 << step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
  // 	 << G4endl;

  //G4int Nsecondaries =0;
  if(TrackID == 1)
    {     /*
           G4cout<< "Primary particle: it is a " 
		 <<track->GetParticleDefinition()->GetParticleName() 
		 << " with a kinetic energy of " << Ekin/MeV << " MeV" 
		 << G4endl;
         */
      // fEventAction->GetPrimaryPDG(pdg);
     }

  if(TrackID !=1)
    {
      /*G4cout<< "Secondary particle: it is a " 
	    << track->GetParticleDefinition()->GetParticleName() 
	    << " with a kinetic energy of " << Ekin/MeV << " MeV; " 
	    << " Parent ID = " << ParentID
	    << G4endl;*/
    
  
      if(ParentID == 1 && creator==inelastic)
	{
	  //G4cout << Ekin << " ";
	  //G4cout << track->GetParticleDefinition()->GetParticleName()<<G4endl;
	  //fEventAction->AddSecondary();
	}
    }
  
  //  G4cout << "Number of 2ry particles = " << Nsecondaries << G4endl;
}

//void ExN04TrackingAction::PreUserTrackingAction(const G4Track* track)
//{}
