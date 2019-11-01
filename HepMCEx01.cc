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
/// \file eventgenerator/HepMC/HepMCEx01/HepMCEx01.cc
/// \brief Main program of the eventgenerator/HepMC/HepMCEx01 example
//
// $Id: HepMCEx01.cc 77801 2013-11-28 13:33:20Z gcosmo $
//
// --------------------------------------------------------------
//      GEANT 4 - example derived from novice exampleN04
// --------------------------------------------------------------

#include "Shielding.hh"
#include "FTFP_BERT.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh"
#include "ExN04DetectorConstruction.hh"
#include "ExN04EventAction.hh"
#include "ExN04PrimaryGeneratorAction.hh"
#include "ExN04RunAction.hh"
#include "ExN04StackingAction.hh"
#include "ExN04SteppingAction.hh"
#include "ExN04SteppingVerbose.hh"
#include "ExN04TrackingAction.hh"
#include "ExN04PhysicsList.hh"
#include "ExN04ActionInitialization.hh"


//g4 writer
#include "G4GDMLParser.hh"
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleB4a [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}


int main(int argc,char** argv)
{

  if ( argc > 7 ) {
    PrintUsage();
    return 1;
  }

  G4GDMLParser parser;
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif
  for ( G4int i=1; i<argc; i=i+2 ) {
    if      ( G4String(argv[i]) == "-m" ) macro = argv[i+1];
    else if ( G4String(argv[i]) == "-u" ) session = argv[i+1];
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
    else if ( G4String(argv[i]) == "-g" )parser.Read(argv[i+1]); 
#endif
    else {
      PrintUsage();
      return 1;
    }
  }
  

  // Choose the Random engine
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  nThreads = 4;
  G4MTRunManager * runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  
  // User Verbose output class
  //
  //G4VSteppingVerbose* verbosity = new ExN04SteppingVerbose;
  //G4VSteppingVerbose::SetInstance(verbosity);

  // Set mandatory initialization classes
  //

  /// Reading the GDML file


  
  /// Run manager
  
  //runManager->SetUserInitialization ( new TGDML_DetectorConstruction(parser.GetWorldVolume()) ) ;
  
  ExN04DetectorConstruction* detConstruction = new ExN04DetectorConstruction;
  runManager->SetUserInitialization(detConstruction);
  //G4VUserPhysicsList* physics = new Shielding;    // for neutron and muon (for muon try cut region in the rock: 5cm)
  //G4VUserPhysicsList* physics = new QGSP_BERT_HP;    // for neutron and muon (for muon try cut region in the rock: 5cm)
  //runManager->SetUserInitialization(physics);        // for neutron and muon 
  runManager->SetUserInitialization(new ExN04PhysicsList);  // Livermore (or Penelope) for electrons
  ////runManager->SetUserInitialization(new Shielding);  
  runManager->SetUserInitialization(new ExN04ActionInitialization(detConstruction));
  
  

   
  
 //interactive mode
#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  //Pointer to user Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  // if(argc!=1) //batch mode
  //   {
  //     G4String command = "/control/execute ";
  //     G4String fileName = argv[1];
  //     UImanager->ApplyCommand(command+fileName);
  //   }
 if ( macro.size() ) {
    // batch mode
    UImanager->ApplyCommand("/control/execute init.mac");
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else
    { 

 // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init_vis.mac");
#else
      UImanager->ApplyCommand("/control/execute init.mac");
#endif
      if (ui->IsGUI())
         UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
    }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
  
