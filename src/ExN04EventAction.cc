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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04EventAction.cc
/// \brief Implementation of the ExN04EventAction class
//
// $Id: ExN04EventAction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4VHitsCollection.hh"
#include "ExN04EventAction.hh"

#include "ExN04Analysis.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "Randomize.hh"
#include <iomanip>
#include "ExN04AnalysisManager.hh"
#include "ExN04RunAction.hh"

#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04EventAction::ExN04EventAction()
  : G4UserEventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04EventAction::~ExN04EventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04EventAction::BeginOfEventAction(const G4Event*)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04EventAction::EndOfEventAction(const G4Event* event)
{
  // Numero evento
   Event = event->GetEventID();
   //G4cout << "Evento " << event_id << G4endl;   // utile per print inf
   
   
   for(G4int i=0; i<1000; i++){
     if(Event == i*10000) G4cout << Event << G4endl;    // utile per run
   }
   // get analysis manager
   //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
   //analysisManager->FillNtupleIColumn(1,0,Event);
   //analysisManager->AddNtupleRow();
}
