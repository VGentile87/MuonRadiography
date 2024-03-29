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
/// \file eventgenerator/HepMC/HepMCEx01/include/ExN04TrackingAction.hh
/// \brief Definition of the ExN04TrackingAction class
//
// $Id: ExN04TrackingAction.hh 77801 2013-11-28 13:33:20Z gcosmo $
//

#ifndef ExN04TrackingAction_h
#define ExN04TrackingAction_h 1

#include "G4UserTrackingAction.hh"

#include "globals.hh"


class ExN04PrimaryGeneratorAction;
class ExN04DetectorConstruction;
class ExN04RunAction;
class ExN04EventAction;

class ExN04TrackingAction : public G4UserTrackingAction {
public:
  ExN04TrackingAction(){};
  virtual ~ExN04TrackingAction(){};

  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

  double StepInSoil(G4double stepsoil);
  double Edep(G4double edep, G4double density);
  double GetStepInSoil();
  double GetTotEdep();
  void StepInSoilInitialize();

  //double myStoppingPower(G4double m, G4double q, G4double E, G4double ro, G4double Z, G4double A, G4double I);


private:
  ExN04DetectorConstruction* fDetConstruction;
  ExN04RunAction*          fRunAction;  
  ExN04EventAction*          fEventAction;
  G4double stepl=0;
  G4double tot_edep=0;
};

inline void ExN04TrackingAction::StepInSoilInitialize(){ 
  stepl=0;
  tot_edep=0;
  }

inline double ExN04TrackingAction::StepInSoil(G4double stepsoil){
  stepl += stepsoil;
  return stepl;
  }

inline double ExN04TrackingAction::Edep(G4double steplength, G4double density){ 
  tot_edep += 0.15*density*steplength;  // energy loss GeV * g/cm^3 * m  (mu at 100 GeV --> edep 2MeV / cm)
  return tot_edep;
}

inline double ExN04TrackingAction::GetStepInSoil(){
  return stepl;
}

inline double ExN04TrackingAction::GetTotEdep(){
  return tot_edep;
}

/*
inline double ExN04SteppingAction::myStoppingPower(G4double m, G4double q, G4double E, G4double ro, G4double Z, G4double A, G4double I){

  double k = 0.307075;
  double g, b2, T, SP;
  
  printf("\n|******** Stopping-Power values for this material ********|\n");
  
  g = 1 + (E/m);
  printf("\n Gamma  =  %lf", g);
  
  b2 = 1 - (1/(g*g));
  printf("\n Beta squared  =  %lf", b2);
  
  T = (1.022*b2*g*g) / (1 + 2*g*(0.511/m) + pow(0.511/m,2) );
  printf("\n Maximum kinetic energy transferred  =  %lf MeV ", T);
  
  SP = ((ro*k*q*q*Z)/(A*b2))*(0.5*log( (1.022*b2*g*g*T) / (I*I) ) - b2);
  printf("\n Stopping Power  =  %lf MeV/cm \n", SP);
  
  return SP;

}*/

#endif
