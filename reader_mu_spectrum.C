// V. GENTILE 2019
//
// Macros for the muons production in a particluar volume

#define reader_mu_spectrum_cxx
#include "reader_mu_spectrum.h"
#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <fstream>
#include <TH1.h>
#include <TProfile.h>
#include "Riostream.h"
#include <vector>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <stdio.h>
#include <TF1.h>
#include <TStyle.h>
#include <fstream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TSystem.h>
#include <TLine.h>
#include <TGraph.h>
#include <TLegend.h>

reader_mu_spectrum vv;

using namespace std;

void reader_mu_spectrum::Loop()
{
  const int nevfile=10000000;
  cout <<"hello"<<endl;

  TString dir = gSystem->UnixPathName(__FILE__);
  dir.ReplaceAll("reader_mu_spectrum.C","");
  dir.ReplaceAll("/./","/");

  Int_t ev,id;
  Float_t ene,x,y,z,cx,cy,cz,xrot,yrot;
  Long64_t nev=0;
  Float_t mass_mu = 0.1056583745; // GeV

  Float_t cosphi = 0.997; // rad (angolo di rotazione rispetto Z)
  Float_t sinphi = 0.074; // rad (angolo di rotazione rispetto Z)

  TH2F * h = new TH2F("h","h",100,-3000,-2000,100,-1000,1000);
  TH2F * hrot = new TH2F("hrot","hrot",100,-3000,-2000,100,-1000,1000);

  TRandom3 *r= new TRandom3();
  r->SetSeed(0);
    
  Int_t pdg=0;
  Float_t pmod=0;
  Float_t px=0;
  Float_t py=0;
  Float_t pz=0;
  Int_t nfile=0;

  Float_t tmp_max_x=-1000000;
  Float_t tmp_min_x=1000000;

  Float_t tmp_max_y=-1000000;
  Float_t tmp_min_y=1000000;

  Float_t emu_x0;
  Float_t emu_y0;   
  Float_t emu_z;
  Float_t id_pos;
  Int_t nmuons;

  cout << "Insert position number"<< endl;
  cin >> id_pos;
  cout << endl;
  cout << "Coordinates of emulsion film (x,y,z) in mm"<<endl;
  cin >> emu_x0;
  cout << " ";
  cin >> emu_y0;
  cout << " ";
  cin >> emu_z;
  cout << endl;
  cout << "Insert number of produced muons"<< endl;
  cin >> nmuons;
  
  Long64_t nentries = fChain->GetEntries();
    
    
    for (Long64_t in=0; in<nmuons/*nentries*/;in++) { // su tutte le entries
      GetEntry(in);
      Long64_t ientry = LoadTree(in);
      FILE *log;
      
      float t_cut = 55*(TMath::Pi()/180.);
      float p_cut = 0.300; //GeV
      if(t < t_cut && p > p_cut){

	if(nev%nevfile==0){
	  int nmuons_in_M = nmuons/1000000;
	  log = fopen(Form("hepmuon_sea_level_%dM_%d_pos_%.1f.txt",nmuons_in_M,nfile,id_pos),"w+");
	  fprintf(log,"HepMC::Version 2.06.09\n");
	  fprintf(log,"HepMC::IO_GenEvent-START_EVENT_LISTING\n");
	  nev=0;
	  nfile++;
	}

	int charge = r->Uniform(0,2);  // per garantire mu+ e mu-
	
	// DIMENSIONI EMULSIONI
	x = r->Uniform(-150,150); 
	y = r->Uniform(-125,125);  
	// trasformazione su z
	xrot = emu_x0 +  (x*cosphi - y*sinphi);
	yrot = emu_y0 +  (x*sinphi + y*cosphi);
	//cout << x << " " << xrot << " " << y << " " << yrot << endl;

	// calcolo dei margini xy dell'emulsione ruotata
	if(xrot>tmp_max_x)tmp_max_x=xrot;
	if(xrot<tmp_min_x)tmp_min_x=xrot;
	if(yrot>tmp_max_y)tmp_max_y=yrot;	
	if(yrot<tmp_min_y)tmp_min_y=yrot;
	h->Fill(x+emu_x0,y+emu_y0);
	hrot->Fill(xrot,yrot);
	//cout << in << " " << t << " " << phi << " " << p << " " << charge <<  endl;

	// assegno la carica
	if(charge==0)pdg=-13;
	if(charge==1)pdg=13;
	// cout << ev << " " << pdg << endl;
	//if (nlines < 5) printf("x=%8f, y=%8f, z=%8f\n",x,y,z);

	// PARAMETRI DINAMICI
	//pmod= TMath::Sqrt(ene*ene + mass_mu*mass_mu);
	ene = TMath::Sqrt(p*p - mass_mu*mass_mu);
	px = p*TMath::Cos(phi)*TMath::Sin(t);
	py = p*TMath::Sin(phi)*TMath::Sin(t);
	pz = p*TMath::Cos(t);
	pmod = TMath::Sqrt(px*px + py*py + pz*pz);
	
	//cout << nev << " " << p << " " << pmod << " " << xrot << " " << yrot << " " << z << endl;
	
	fprintf(log,"E %lld  -1 -1.0 -1.0 -1.0 0 0 1 1 2 0 0\n",nev);
	fprintf(log,"U GEV MM\n");
	fprintf(log,"V -1 0 %f %f %f 0 0 1 0\n",xrot,yrot,emu_z); //mm
	//fprintf(log,"P 1 %d %f %f %f %f %f 1 0 0 0 0\n",pdg,ene,ene,cx,cy,cz);
	fprintf(log,"P 1 %d %f %f %f %f %f 1 0 0 0 0\n",pdg,px,py,pz,ene,mass_mu);
	
	nev++;
	if(nev%nevfile==0){
	  printf(" found %lld points in the %d file\n",nev,nfile);
	  
	  fprintf(log,"HepMC::IO_GenEvent-END_EVENT_LISTING\n");
	  fclose(log);
	}
      }
    }
    //cout << tmp_max_x << " " << tmp_min_x << " " << tmp_max_y << " " << tmp_min_y << endl;
    
}

int mytest(){
  vv.Loop();
  //cout << "hello" << endl;
}

