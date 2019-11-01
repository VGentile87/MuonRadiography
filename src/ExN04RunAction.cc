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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04RunAction.cc
/// \brief Implementation of the ExN04RunAction class
//
// $Id: ExN04RunAction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "ExN04RunAction.hh"
#include "ExN04Analysis.hh"
#include "G4Run.hh"
#include "ExN04EventAction.hh"
#include "G4UnitsTable.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "TFile.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04RunAction::ExN04RunAction()
   : G4UserRunAction()
{
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(0);       // MALEDETTO
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  eTree=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04RunAction::~ExN04RunAction()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04RunAction::BeginOfRunAction(const G4Run* /*aRun*/)
{
  //-----------DYNAMIC SEED FOR EACH RUN--------------------------//
  // aggiunto io
  long seeds[2];
  time_t systime = time(NULL);
  seeds[0] = (long) systime;
  seeds[1] = (long) (systime*G4UniformRand());
  //seeds[0] = 0;  // settare per controlli tra pacchetti
  //seeds[1] = 1;
  G4Random::setTheSeeds(seeds);  
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //G4String fileName = "Root_data_G4_sim";
  //analysisManager->OpenFile(fileName);

  eFileMC = new TFile("tmc.root","RECREATE");
  //ExN04RunAction tmc;
  MakeTree();

}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN04RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // print histogram statistics
  //
  //TFile *f = new TFile("tmc.root");
  //TTree *tree = (TTree*)f->Get("treemc");
  //TFile f;
  //ExN04RunAction tmc;
  //tree->Write();
  // save histograms & ntuple
  //
  eFileMC->cd();
  eTree->Write();
  eFileMC->Close();
  //Analysis::GetInstance()->EndOfRun(aRun);
  //analysisManager->Write();
  //analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

