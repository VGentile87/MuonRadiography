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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04DetectorConstruction.cc
/// \brief Implementation of the ExN04DetectorConstruction class
//
// $Id: ExN04DetectorConstruction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4FieldManager.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "ExN04CalorimeterParametrisation.hh"
#include "ExN04CalorimeterROGeometry.hh"
#include "ExN04CalorimeterSD.hh"
#include "ExN04DetectorConstruction.hh"
#include "ExN04Field.hh"
#include "ExN04MuonSD.hh"
#include "ExN04TrackerParametrisation.hh"
#include "ExN04TrackerSD.hh"


#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4ProductionCuts.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include "Riostream.h"
#include <algorithm>


using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04DetectorConstruction::ExN04DetectorConstruction()
//:fShieldCuts(0)
{

//#include "ExN04DetectorParameterDef.icc"
DefineMaterials();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ExN04DetectorConstruction::~ExN04DetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ExN04DetectorConstruction::DefineMaterials()
{
  //-------------------------------------------------------------------------
  // Materials
  //-------------------------------------------------------------------------

   //
  // Database Material
  //
  
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->SetVerbose(1);
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE"); // plates 
  nistManager->FindOrBuildMaterial("G4_WATER");   // shielding
  nistManager->FindOrBuildMaterial("G4_POLYSTYRENE");  // base emulsion
  nistManager->FindOrBuildMaterial("G4_Pb");  //  lead
  nistManager->FindOrBuildMaterial("G4_Fe");
  nistManager->FindOrBuildMaterial("G4_Cu");  
  nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");
  nistManager->FindOrBuildMaterial("G4_AIR");
  nistManager->FindOrBuildMaterial("G4_CONCRETE");  

  //
  // Material Properties
  //
  
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density, opera_density, opera_base_density;
  G4double fractionmass;
  G4String name, symbol; 
  G4int ncomponents, natoms;
  G4int base_ncomponents;

  //
  // Emulsion Component  
  //
  
  a = 126.9*g/mole;
  G4Element* elI  = new G4Element(name="Iodium", symbol="I", z=53., a);
  
  a = 107.9*g/mole;
  G4Element* elAg  = new G4Element(name="Silver", symbol="Ag", z=47., a);

  a = 79.9*g/mole;
  G4Element* elBr  = new G4Element(name="Bromine", symbol="Br", z=35., a);
  
  a = 32.1*g/mole;
  G4Element* elS  = new G4Element(name="Sulphur", symbol="S", z=16., a);
  
  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen", symbol="O", z=8., a);

  a = 14.01*g/mole;
  G4Element* elN  = new G4Element(name="Nitrogen", symbol="N", z= 7., a);
  
  a = 12.00*g/mole;
  G4Element* elC  = new G4Element(name="Carbon", symbol="C", z= 6., a);

  a = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

  //
  // NIT EMULSION MATERIAL
  //
  
  density = 3.43*g/cm3; // NIT Emulsion
  G4Material* Emulsion = new G4Material(name="Emulsion", density, ncomponents=8);

  //
  // Fraction Mass NIT
  //
  
  Emulsion->AddElement(elAg,fractionmass=0.44065);
  Emulsion->AddElement(elBr,fractionmass=0.32190);
  Emulsion->AddElement(elI, fractionmass=0.01875);
  Emulsion->AddElement(elC, fractionmass=0.1012);
  Emulsion->AddElement(elO, fractionmass=0.0741);
  Emulsion->AddElement(elN, fractionmass=0.0268);
  Emulsion->AddElement(elH, fractionmass=0.0163);
  Emulsion->AddElement(elS, fractionmass=0.0003);

 //
  // Rock Component  
  //
  
  a = 28.09*g/mole;
  G4Element* elSi = new G4Element(name="Silicon", symbol="Si", z=14.,a);
  
  a = 26.98*g/mole;
  G4Element* elAl = new G4Element(name="Aluminum", symbol="Al", z=13.,a);

  a = 24.305*g/mole;
  G4Element* elMg = new G4Element(name="Magnesium",symbol="Mg",z=12.,a);
  
  a = 40.08*g/mole;
  G4Element* elCa = new G4Element(name="Calcium",symbol="Ca",z=20.,a);
  
  //a = 16.00*g/mole;
  //G4Element* elO  = new G4Element(name="Oxygen", symbol="O", z=8., a);

  a = 39.098*g/mole;
  G4Element* elK = new G4Element(name="Potassium",symbol="K",z=19.,a);
  
  //a = 12.00*g/mole;
  //G4Element* elC  = new G4Element(name="Carbon", symbol="C", z=6., a);


   //
  // ROCK MATERIAL
  //
  
  G4double density_rock = 2.2*g/cm3; // ROCK IPOGEI
  G4Material* rock_material = new G4Material(name="rockMaterial", density_rock, ncomponents=7);

  //
  // Fraction Mass NIT
  //
  
  rock_material->AddElement(elC,fractionmass=0.06001);
  rock_material->AddElement(elO,fractionmass=0.32508);
  rock_material->AddElement(elMg, fractionmass=0.05710);
  rock_material->AddElement(elAl, fractionmass=0.01129);
  rock_material->AddElement(elSi, fractionmass=0.01464);
  rock_material->AddElement(elK, fractionmass=0.01660);
  rock_material->AddElement(elCa, fractionmass=0.51520);

  

  //
  // World Material
  //
  
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
		 kStateGas, 2.73*kelvin, 3.e-18*pascal);
 

  //PMMA C5H8O2 ( Acrylic )
  // -------------
  G4double ACRdensity = 1.19*g/cm3;
  G4Material* Acrylic = new G4Material(name="Acrylic", ACRdensity, base_ncomponents=3);
  Acrylic->AddElement(elC, 5);
  Acrylic->AddElement(elH, 8);
  Acrylic->AddElement(elO, 2);
  

  G4Material* Polystyrene = new G4Material("Polystyrene", density= 1.050*g/cm3, ncomponents=2);
  Polystyrene->AddElement(elC, natoms=8); 
  Polystyrene->AddElement(elH, natoms=8);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VPhysicalVolume* ExN04DetectorConstruction::Construct()
{

  ///// Physical Parameters /////

  //
  // Get materials
  //
  
  G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
  G4Material* airMaterial = G4Material::GetMaterial("G4_AIR");           
  G4Material* emulsionMaterial = G4Material::GetMaterial("Emulsion");            
  //G4Material* baseMaterial = G4Material::GetMaterial("G4_POLYSTYRENE");
  G4Material* baseMaterial = G4Material::GetMaterial("Acrylic");
  //G4Material* plateMaterial = G4Material::GetMaterial("G4_WATER");
  G4Material* plateMaterial = G4Material::GetMaterial("G4_POLYETHYLENE");
  G4Material* leadMaterial = G4Material::GetMaterial("G4_Pb");
  G4Material* supportMaterial = G4Material::GetMaterial("G4_Fe");
  G4Material* rockMaterial = G4Material::GetMaterial("rockMaterial");
  G4Material* wallMaterial = G4Material::GetMaterial("G4_CONCRETE");
  G4Material* nitrogen_box_Material = G4Material::GetMaterial("G4_PLEXIGLASS");
  
  
  

  if ( ! defaultMaterial ) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("B4ExN04DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  

 
  /////  Geometrical Parameters /////

  //
  //Dimensioni del mondo
  //
  
  G4double worldSizeX = 200 *m;
  G4double worldSizeY = 200 *m;
  G4double worldSizeZ = 200 *m;

  G4double myworldR = 90*m;

  //
  // Detector
  //
  
  //G4double baseThickness  = 175*um; // PLASTIC
  G4double baseThickness  = 1*mm;  //PMMA  --- SLIDE GLASS
  G4double layerThickness = 50*um;
  G4double factor_source = 1.5;
  G4double emuSizeX       = 30.0*cm*factor_source;//100*cm;//30.0*cm;//+gap_source; //12.0*cm;  
  G4double emuSizeY       = 25.0*cm*factor_source;//100*cm;//+gap_source; //10.0*cm;
  G4double emuSizeZ       = 50.0*um;//+1*cm;       //12.0*cm;    
  G4int nFilm             = 150;//45;//45;//150;   //envir //cosmog
  G4double distance = 0;//5*mm;
  G4double filmThickness   = baseThickness+layerThickness+distance; //*2 se double coated
  G4double detThickness   = filmThickness*nFilm;


  // VARIE POSIZIONI DELLE EMULSIONI  

  //pos1
  /*
  G4double emuPosX = -2.470*m;
  G4double emuPosY =  0.545*m;
  G4double emuPosZ = -18.25*m;
  
  //pos5
  G4double emuPosX = -2.434*m;
  G4double emuPosY =  4.220*m;
  G4double emuPosZ = -18.25*m;
  
  //pos3.1
  G4double emuPosX = -4.24*m;
  G4double emuPosY =  1.88*m;
  G4double emuPosZ = -18.2*m;
  
  //pos3.2
  G4double emuPosX = -4.28*m;
  G4double emuPosY =  2.24*m;
  G4double emuPosZ = -18.2*m;
  
  //pos4.1
  G4double emuPosX = -1.69*m;
  G4double emuPosY =  2.85*m;
  G4double emuPosZ = -18.2*m;
  
  //pos5.1
  G4double emuPosX = 1.64*m;
  G4double emuPosY = 2.33*m;
  G4double emuPosZ = -18.2*m;
  */
  //pos5.2
  G4double emuPosX = 1.45*m;
  G4double emuPosY = 2.73*m;
  G4double emuPosZ = -18.2*m;
  
  
  //
  // Shielding Bottom plate
  //

  G4double soilX = 100*m;
  G4double soilY = 100*m;
  G4double soilZ = 20*m;

  G4double surfX = 100*m;
  G4double surfY = 100*m;
  G4double surfZ = 40*m;

 


  vector <int> overlap_cube;
  vector <double> x;
  vector <double> y;
  vector <double> z;
  vector <double> x_up;
  vector <double> y_up;
  vector <double> z_up;
  G4double  xfile,yfile,zfile,xdim,ydim,zdim;
  G4int nlines=0;
  G4int npoints_unde=0;
  G4int npoints_surf=0;
  G4int npoints_middle=0;
  G4double tmp_zfile=0;

  ifstream in;
  in.open("../out.txt");
  std::cout << "hello" << std::endl;
  while (1) {
    //for(int i=0;i<10;i++){
    in >> xfile >> yfile >> zfile;
      if(in.eof() ) break; // da mettere dopo il cin
      if (!in.good()) break;
      xfile=xfile*m;
      yfile=yfile*m;
      zfile=zfile*m;
      if (nlines < 5) std::cout << xfile << " " << yfile << " " << zfile << std::endl;
      if(nlines>0){
	if((zfile+zdim/2.)<0 && zfile<0){
	  x.push_back(xfile);
	  y.push_back(yfile);
	  z.push_back(zfile);
	  if(abs(zfile)>tmp_zfile){
	    tmp_zfile=abs(zfile);
	    //cout<< nlines << " " << npoints_unde <<" " << tmp_zfile << endl;
	  }

	  // overlap con emulsione
	  if(abs(xfile - emuPosX)<(emuSizeX/2. + xdim/2.) && abs(yfile - emuPosY)<(emuSizeY/2. + ydim/2.) && abs(zfile - emuPosZ)<(emuSizeZ/2. + zdim/2.)) overlap_cube.push_back(npoints_unde);  //cout << "overlap "<<npoints_unde << endl;
	  
	  npoints_unde++;
	 	  
      }
	if((zfile-zdim/2.)>0 && zfile>0){
	  x_up.push_back(xfile);
	  y_up.push_back(yfile);
	  z_up.push_back(zfile);
	  npoints_surf++;
	}
	if(zfile<0 && (zfile+zdim/2.)>0){
	  //std::cout << xfile << " " << yfile << " " << zfile << " " << -zfile+zdim/2. << std::endl;
	  npoints_middle++;
	}
	if(zfile>0 && (zfile-zdim/2.)<0){
	  //std::cout << xfile << " " << yfile << " " << zfile << " " << zfile-zdim/2. << std::endl;
	  npoints_middle++;
	}
     }
      else{
	xdim = xfile;
	ydim = yfile;
	zdim = zfile;
      }
      
      nlines++;
   }
  std::cout << "nlines "<< nlines << " "<< npoints_unde << " " << npoints_surf << " " <<npoints_middle << std::endl;
  std::cout << "cube dim "<< xdim << " " << ydim << " " << zdim << " " << tmp_zfile << std::endl;
  in.close();    
  //     
  // World
  //
  
  G4VSolid* worldS 
    = new G4Box("World", worldSizeX/2, worldSizeY/2, worldSizeZ/2);                      
  G4LogicalVolume* worldLV
    = new G4LogicalVolume(worldS, defaultMaterial,"World");
                                   
  G4VPhysicalVolume* worldPV
    = new G4PVPlacement(0, G4ThreeVector(),worldLV,                         
                 "World", 0, false, 0, fCheckOverlaps);

  G4VSolid *myworldS
    = new G4Sphere("myWorld", myworldR-1*cm,
		   myworldR,    // Outer radius
		   0.*deg,         // Starting phi
		   360.*deg,       // Delta phi
		   0.*deg,         // Starting theta
		   180.*deg);      // Delta theta

  G4LogicalVolume* myworldLV
    = new G4LogicalVolume(myworldS, defaultMaterial,"myWorld");

  G4VPhysicalVolume* myworldPV
    = new G4PVPlacement(0, G4ThreeVector(),myworldLV,                         
                 "myWorld", worldLV, false, 0, fCheckOverlaps);
  

  G4VSolid* soilS 
    = new G4Box("Soil", soilX/2, soilY/2, soilZ/2);                      
  G4LogicalVolume* soilLV
    = new G4LogicalVolume(soilS, rockMaterial,"Soil");
  
  G4VPhysicalVolume* soilPV
    = new G4PVPlacement(0, G4ThreeVector(0.,0.,-soilZ/2.),soilLV,                         
                 "Soil", worldLV, false, 0, fCheckOverlaps);

  
  G4VSolid* surfS 
    = new G4Box("Surface", surfX/2, surfY/2, surfZ/2);                      
  G4LogicalVolume* surfLV
    = new G4LogicalVolume(surfS, airMaterial,"Surface");
  
  G4VPhysicalVolume* surfacePV
    = new G4PVPlacement(0, G4ThreeVector(0.,0.,surfZ/2.),surfLV,                         
                 "Surface", worldLV, false, 0, fCheckOverlaps);

  G4VSolid* emulsionS 
    = new G4Box("Emulsion", emuSizeX/2, emuSizeY/2, emuSizeZ/2);                      
  G4LogicalVolume* emulsionLV
    = new G4LogicalVolume(emulsionS, defaultMaterial,"Emulsion");


  G4double outerwallXmax = 22.73*m;
  G4double outerwallXmin = 20.43*m;
  G4double outerwallY = 24*m;
  G4double outerwallZ = 25*m;

  G4double thickwall = 30*cm;
  G4double xygap= 10*cm;
    
  G4double int_outerwallXmax = outerwallXmax - thickwall;
  G4double int_outerwallXmin = outerwallXmin - thickwall;
  G4double int_outerwallY = outerwallY - thickwall;
  G4double int_outerwallZ = outerwallZ - thickwall;

  G4double innerspaceX = 5.10*m;
  G4double innerspaceY = 6.30*m;
  G4double innerspaceZ = 25.*m;

  G4double innerwallX = innerspaceX + thickwall + xygap;
  G4double innerwallY = innerspaceY + thickwall + xygap;
  G4double innerwallZ = innerspaceZ;

  G4double innerwallX2 = innerspaceX + thickwall;
  G4double innerwallY2 = innerspaceY + thickwall;
  G4double innerwallZ2 = innerspaceZ - thickwall;

  G4double nowallX = 6.5*m;
  G4double nowallY = 5.7*m;
  G4double nowallZ = 8.*m;

  G4double bld_thickness=60*cm;
  G4double bld_gap=1*cm;

  G4double bld1X = 10*m;
  G4double bld1Y = 47*m;
  G4double bld1Z = 20*m;

  G4double ibld1X = bld1X - bld_thickness;
  G4double ibld1Y = bld1Y - bld_thickness;
  G4double ibld1Z = bld1Z - bld_thickness;

  G4double bld1_dist = 9.5*m;

  G4double bld2X = 40*m;
  G4double bld2Y = 10*m;
  G4double bld2Z = 20*m;

  G4double ibld2X = bld2X - bld_thickness;
  G4double ibld2Y = bld2Y - bld_thickness;
  G4double ibld2Z = bld2Z - bld_thickness;

  G4double bld2_dist = 7.3*m;

  G4double bld3X = 42.5*m;
  G4double bld3Y = 10.5*m;
  G4double bld3Z = 20*m;

  G4double ibld3X = bld3X - bld_thickness;
  G4double ibld3Y = bld3Y - bld_thickness;
  G4double ibld3Z = bld3Z - bld_thickness;

  G4double bld4X = 10.5*m;
  G4double bld4Y = 24*m;
  G4double bld4Z = 20*m;

  G4double ibld4X = bld4X - bld_thickness;
  G4double ibld4Y = bld4Y - bld_thickness;
  G4double ibld4Z = bld4Z - bld_thickness;

  G4double bld5X = 11.0*m;
  G4double bld5Y = 15.0*m;
  G4double bld5Z = 20*m;

  G4double ibld5X = bld5X - bld_thickness;
  G4double ibld5Y = bld5Y - bld_thickness;
  G4double ibld5Z = bld5Z - bld_thickness;
  
  
  G4VSolid * int_outerwallS
    = new G4Trap("int_outerWall",int_outerwallZ,int_outerwallY,int_outerwallXmax,int_outerwallXmin);

  G4VSolid * outerwallS
    = new G4Trap("Wall",outerwallZ,outerwallY,outerwallXmax,outerwallXmin);

  G4VSolid* out_subtrwallS1 =
    new G4SubtractionSolid("out_SubtrWall1", outerwallS, int_outerwallS);
  

  G4VSolid* innerwallS 
    = new G4Box("Inside Wall", innerwallX/2, innerwallY/2, innerwallZ/2);

  G4VSolid* innerwall2S 
    = new G4Box("Inside Wall2", innerwallX2/2, innerwallY2/2, innerwallZ2/2);

  G4VSolid* innerspaceS 
    = new G4Box("Inner space", innerspaceX/2, innerspaceY/2, innerspaceZ/2);

  G4VSolid* nowallS 
    = new G4Box("No Wall", nowallX/2, nowallY/2, nowallZ/2);
  

  G4ThreeVector zTrans(1.0*m,2.1*m,0.);
  G4RotationMatrix* zRot = new G4RotationMatrix; // Rotates X and Z axes only
  zRot->rotateZ(-3*deg); // Rotates 45 degrees
  G4Transform3D rot_tr(*zRot, zTrans);

  G4VSolid* subtrwallS =
    new G4SubtractionSolid("SubtrWall", int_outerwallS, innerwallS,
			   rot_tr);

  G4VSolid* int_subtrwallS =
    new G4SubtractionSolid("int_SubtrWall", innerwall2S, innerspaceS);

  G4VSolid* wallS =
    new G4SubtractionSolid("Wall", subtrwallS, nowallS,
			   0,G4ThreeVector(-((outerwallXmin+outerwallXmax)/4.-nowallX/2.),outerwallY/2.- nowallY/2.,outerwallZ/2.- nowallZ/2.));


  G4VSolid* OutwallS =
    new G4IntersectionSolid("OutWalls", wallS,out_subtrwallS1 , 0, G4ThreeVector(0.,0.,0.));
  
  
  G4LogicalVolume* out_wallLV
    = new G4LogicalVolume(out_subtrwallS1, wallMaterial,"outWall");

  G4LogicalVolume* int_wallLV
    = new G4LogicalVolume(int_subtrwallS, wallMaterial,"intWall");

  G4LogicalVolume* wallLV
    = new G4LogicalVolume(wallS, wallMaterial,"Wall");

  G4RotationMatrix* zRot2 = new G4RotationMatrix; // Rotates X and Z axes only
  zRot2->rotateZ(-20*deg); // Rotates  degrees
  //zRot2->rotateZ(0*deg); // for check
  G4RotationMatrix* zRot4 = new G4RotationMatrix; // Rotates X and Z axes only
  zRot4->rotateZ(-200.0*deg); // Rotates  degrees

  //G4RotationMatrix* zRot5 = new G4RotationMatrix; // Rotates X and Z axes only
  //zRot5->rotateZ(-205.5*deg); // Rotates  degrees

  G4RotationMatrix* zRot3 = new G4RotationMatrix; // Rotates X and Z axes only
  zRot3->rotateZ(-17*deg); // Rotates  degrees
  //zRot3->rotateZ(0*deg); // for check
  
  G4VPhysicalVolume* wallPV
    = new G4PVPlacement(zRot2, G4ThreeVector(-1.5*m,-2*m,-(surfZ/2.-outerwallZ/2.)),wallLV,                         
                 "Wall", surfLV, false, 0, fCheckOverlaps);

  G4VPhysicalVolume* out_wallPV
    = new G4PVPlacement(zRot2, G4ThreeVector(-1.5*m,-2*m,-(surfZ/2.-outerwallZ/2.)),out_wallLV,                         
                 "outWall", surfLV, false, 0, fCheckOverlaps);

  G4VPhysicalVolume* int_wallPV
    = new G4PVPlacement(zRot3, G4ThreeVector(-1.28*m, 0.32*m,-(surfZ/2.-outerwallZ/2.)),int_wallLV,                         
                 "intWall", surfLV, false, 0, fCheckOverlaps);


  //////////////////////////////////////////////////////////////////////////////////////////////////

   G4VSolid* bld1S 
    = new G4Box("Bld1", bld1X/2, bld1Y/2, bld1Z/2);

   G4VSolid* ibld1S 
    = new G4Box("iBld1", ibld1X/2, ibld1Y/2, ibld1Z/2);

   G4VSolid* sbld1S =
    new G4SubtractionSolid("sBld1",bld1S, ibld1S);
   
   G4VSolid* bld2S 
    = new G4Box("Bld2", bld2X/2, bld2Y/2, bld2Z/2);

   G4VSolid* ibld2S 
    = new G4Box("iBld2", ibld2X/2, ibld2Y/2, ibld2Z/2);

   G4VSolid* sbld2S =
    new G4SubtractionSolid("sBld2",bld2S, ibld2S);
   
   G4VSolid* bld3S 
    = new G4Box("Bld3", bld3X/2, bld3Y/2, bld3Z/2);

   G4VSolid* ibld3S 
    = new G4Box("iBld3", ibld3X/2, ibld3Y/2, ibld3Z/2);

   G4VSolid* sbld3S =
    new G4SubtractionSolid("sBld3",bld3S, ibld3S);

   G4VSolid * bld4S
    = new G4Trap("Bld4",bld4Z,bld4Y,bld4X + outerwallXmax - outerwallXmin,bld4X);

   G4VSolid * ibld4S
    = new G4Trap("iBld4",ibld4Z,ibld4Y,ibld4X + outerwallXmax - outerwallXmin,ibld4X);
   
   //G4VSolid* bld4S 
   // = new G4Box("Bld4", bld4X/2, bld4Y/2, bld4Z/2);

   //G4VSolid* ibld4S 
   //= new G4Box("iBld4", ibld4X/2, ibld4Y/2, ibld4Z/2);

   G4VSolid* sbld4S =
    new G4SubtractionSolid("sBld4",bld4S, ibld4S);

   G4VSolid* bld5S 
    = new G4Box("Bld5", bld5X/2, bld5Y/2, bld5Z/2);

   G4VSolid* ibld5S 
    = new G4Box("iBld5", ibld5X/2, ibld5Y/2, ibld5Z/2);

   G4VSolid* sbld5S =
    new G4SubtractionSolid("sBld5",bld5S, ibld5S);
   

   /*
   G4VSolid* bld4aS 
    = new G4Box("Bld4a", bld4aX/2, bld4aY/2, bld4aZ/2);

   G4VSolid* ibld4aS 
    = new G4Box("iBld4a", ibld4aX/2, ibld4aY/2, ibld4aZ/2);
   
   G4VSolid* bld4bS 
    = new G4Box("Bld4b", bld4bX/2, bld4bY/2, bld4bZ/2);

    G4VSolid* ibld4bS 
    = new G4Box("iBld4b", ibld4bX/2, ibld4bY/2, ibld4bZ/2);

   G4VSolid* bld4S =
    new G4SubtractionSolid("Bld4",bld4aS, bld4bS,
			   0,G4ThreeVector((bld4aX-bld4bX)/2.,(bld4aY-bld4bY)/2.,0));

   G4VSolid* ibld4S =
    new G4SubtractionSolid("iBld4",ibld4aS, ibld4bS,
			   0,G4ThreeVector((ibld4aX-ibld4bX)/2.,(ibld4aY-ibld4bY)/2.,0));

   G4VSolid* sbld4S =
     new G4SubtractionSolid("sBld4",bld4S, ibld4S);
   */
   
   G4LogicalVolume* sbld1LV
     = new G4LogicalVolume(sbld1S, wallMaterial,"sBld1");
   
   G4LogicalVolume* sbld2LV
     = new G4LogicalVolume(sbld2S, wallMaterial,"sBld2");
   
   G4LogicalVolume* sbld3LV
     = new G4LogicalVolume(sbld3S, wallMaterial,"sBld3");
   
   G4LogicalVolume* sbld4LV
     = new G4LogicalVolume(sbld4S, wallMaterial,"sBld4");

   G4LogicalVolume* sbld5LV
     = new G4LogicalVolume(sbld5S, wallMaterial,"sBld5");

   G4LogicalVolume* bld1LV
     = new G4LogicalVolume(bld1S, wallMaterial,"Bld1");
   
   G4LogicalVolume* bld2LV
     = new G4LogicalVolume(bld2S, wallMaterial,"Bld2");
   
   G4LogicalVolume* bld3LV
     = new G4LogicalVolume(bld3S, wallMaterial,"Bld3");
   
   G4LogicalVolume* bld4LV
     = new G4LogicalVolume(bld4S, wallMaterial,"Bld4");

   G4LogicalVolume* bld5LV
     = new G4LogicalVolume(bld5S, wallMaterial,"Bld5");

   G4VPhysicalVolume* bld1PV
     = new G4PVPlacement(zRot2, G4ThreeVector((-1.5*m-outerwallXmin/2.-bld1X/2.-bld1_dist),(-10*m+bld3Y+outerwallY/2.-bld1Y/2.),-(surfZ/2.-bld1Z/2.)),bld1LV,
			 "Bld1", surfLV, false, 0, fCheckOverlaps);
   
   G4VPhysicalVolume* sbld1PV
     = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),sbld1LV,                         
			 "sBld1", bld1LV, false, 0, fCheckOverlaps);
      
  G4VPhysicalVolume* bld2PV
    = new G4PVPlacement(zRot2, G4ThreeVector(17.5*m,3.25*m-outerwallY/2.- bld2Y/2.-bld2_dist,-(surfZ/2.-bld2Z/2.)),bld2LV,     
			"Bld2", surfLV, false, 0, fCheckOverlaps);

  G4VPhysicalVolume* sbld2PV
    = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),sbld2LV,     
			"sBld2", bld2LV, false, 0, fCheckOverlaps);
   
  G4VPhysicalVolume* bld3PV
    = new G4PVPlacement(zRot2, G4ThreeVector(2.5*m,0.75*m+outerwallY/2.+ bld3Y/2.,-(surfZ/2.-bld3Z/2.)),bld3LV,
			"Bld3", surfLV, false, 0, fCheckOverlaps);

  G4VPhysicalVolume* sbld3PV
    = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),sbld3LV,
			"sBld3", bld3LV, false, 0, fCheckOverlaps);

    
  G4VPhysicalVolume* bld4PV
    = new G4PVPlacement(zRot4, G4ThreeVector(-2.25*m + outerwallXmax/2. +bld4X/2.,3.8*m,-(surfZ/2.-bld4Z/2.)),bld4LV,
			"Bld4", surfLV, false, 0, fCheckOverlaps);

  G4VPhysicalVolume* sbld4PV
    = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),sbld4LV,
			"sBld4", bld4LV, false, 0, fCheckOverlaps);
  
  G4VPhysicalVolume* bld5PV
  = new G4PVPlacement(zRot4, G4ThreeVector(-0.74*m + outerwallXmax/2. + bld4X +bld5X/2.,8.05*m -(bld4Y-bld5Y)/2.,-(surfZ/2.-bld5Z/2.)),bld5LV,"Bld5", surfLV, false, 0, fCheckOverlaps);

  G4VPhysicalVolume* sbld5PV
  = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),sbld5LV,"sBld5", bld5LV, false, 0, fCheckOverlaps);

  
  
  /////////////////////////////////////////////////////////////////////////////
  
  G4RotationMatrix* emuRot = new G4RotationMatrix; // Rotates X and Z axes only
  emuRot->rotateZ(0.074*rad); // Rotates 45 degrees
  G4VPhysicalVolume* emulsionPV
    = new G4PVPlacement(emuRot, G4ThreeVector(emuPosX,emuPosY,emuPosZ+soilZ/2.),emulsionLV,                         
                 "Emulsion", soilLV, false, 0, fCheckOverlaps);
  
  
  G4VSolid* cubeS 
    = new G4Box("Cube", xdim/2.-0.25*mm, ydim/2.-0.25*mm, zdim/2.-0.25*mm);

  std::cout<<"cube " << xdim/2. << " " << ydim/2. << " " << zdim/2. << std::endl;
  std::cout<<"soil " << soilX/2. << " " << soilY/2. << " " << soilZ/2. << std::endl;

  G4String tName1("Cube");        // Allow all target physicals to share
  G4LogicalVolume *cubeLV
	= new G4LogicalVolume(cubeS, airMaterial, "Cube");



  G4int copyNo=0;
  G4int xpos=0, ypos=0,zpos=0;
  
  for (G4int i = 0; i < npoints_unde; i++) {
    if(std::find(overlap_cube.begin(), overlap_cube.end(), i) != overlap_cube.end())continue;
    else{ 
      //if(i%100000==0){
      xpos = x.at(i);
      ypos = y.at(i);
      zpos = z.at(i);
      //cout << i << " " << xpos << " " << ypos << " " << zpos << endl;
      new G4PVPlacement(0,G4ThreeVector(xpos,ypos,zpos+soilZ/2.),
			cubeLV,
			tName1,
			soilLV,false,copyNo++);
      //}
  }
  }
  
   ///// Draw Experimental Setup /////
  
   //                                        
   // Visualization attributes
   //
   
   worldLV->SetVisAttributes (G4VisAttributes::Invisible);

   G4VisAttributes* myworldVisAtt= new G4VisAttributes();
   myworldVisAtt->SetVisibility(true);
   myworldVisAtt->SetColour(G4Colour::White());
   myworldVisAtt->SetForceAuxEdgeVisible(true);
   myworldLV->SetVisAttributes(myworldVisAtt);
   
   G4VisAttributes* cubeVisAtt= new G4VisAttributes();
   cubeVisAtt->SetVisibility(true);
   cubeVisAtt->SetColour(G4Colour::Red());
   cubeVisAtt->SetForceAuxEdgeVisible(true);
   cubeLV->SetVisAttributes(cubeVisAtt);

   /*
   G4VisAttributes* cubeVisAtt2= new G4VisAttributes();
   cubeVisAtt2->SetVisibility(true);
   cubeVisAtt2->SetColour(G4Colour::Green());
   cube2LV->SetVisAttributes(cubeVisAtt2);
   */
   G4VisAttributes* wallVisAtt= new G4VisAttributes();
   wallVisAtt->SetVisibility(true);
   wallVisAtt->SetColour(G4Colour::Blue());
   //wallVisAtt->SetForceSolid(true);
   wallLV->SetVisAttributes(wallVisAtt);

   G4VisAttributes* bld1VisAtt= new G4VisAttributes();
   bld1VisAtt->SetVisibility(true);
   bld1VisAtt->SetColour(G4Colour::Magenta());
   //wallVisAtt->SetForceSolid(true);
   sbld1LV->SetVisAttributes(bld1VisAtt);
   sbld2LV->SetVisAttributes(bld1VisAtt);
   sbld3LV->SetVisAttributes(bld1VisAtt);
   sbld4LV->SetVisAttributes(bld1VisAtt);
   sbld5LV->SetVisAttributes(bld1VisAtt);
   bld1LV->SetVisAttributes(bld1VisAtt);
   bld2LV->SetVisAttributes(bld1VisAtt);
   bld3LV->SetVisAttributes(bld1VisAtt);
   bld4LV->SetVisAttributes(bld1VisAtt);
   bld5LV->SetVisAttributes(bld1VisAtt);

   G4VisAttributes* outwallVisAtt= new G4VisAttributes();
   outwallVisAtt->SetVisibility(true);
   outwallVisAtt->SetColour(G4Colour::Green());
   //wallVisAtt->SetForceSolid(true);
   out_wallLV->SetVisAttributes(outwallVisAtt);

   G4VisAttributes* intwallVisAtt= new G4VisAttributes();
   intwallVisAtt->SetVisibility(true);
   intwallVisAtt->SetColour(G4Colour::Yellow());
   //wallVisAtt->SetForceSolid(true);
   int_wallLV->SetVisAttributes(intwallVisAtt);

   
   G4VisAttributes* emulsionVisAtt= new G4VisAttributes();
   emulsionVisAtt->SetVisibility(true);
   emulsionVisAtt->SetColour(G4Colour::Yellow());
   emulsionVisAtt->SetForceSolid(true);
   emulsionLV->SetVisAttributes(emulsionVisAtt);
   
   //// ONLY FOR EXTERNAL NEUTRONS
   /*
   G4VisAttributes* baseVisAtt= new G4VisAttributes();
   baseVisAtt->SetVisibility(true);
   baseVisAtt->SetColour(G4Colour::White());
   baseVisAtt->SetForceSolid(true);
   baseLV->SetVisAttributes(baseVisAtt);
   */
 
   return worldPV;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN04DetectorConstruction::PrintDetectorParameters()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
