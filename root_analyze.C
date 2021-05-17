#include <TTree.h>
#include <TFile.h>
#include <cmath>
#include <iostream>
#include <TH1D.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

struct FourVec {
	double E;
	double px;
	double py;
	double pz;
};	

struct Event {
	std::vector<FourVec> KPlusVec;
	std::vector<FourVec> KMinusVec;
};

int root_analyze() {

	TFile *inFile  = new TFile("tree_out.root", "READ");
	TFile *outFile = new TFile("hist_out.root", "RECREATE");
	
	TTreeReader *KPlusTreeReader =  new TTreeReader("k_plus_tree" , inFile);
	TTreeReader *KMinusTreeReader = new TTreeReader("k_minus_tree", inFile);
	
	TH1D *massHist = new TH1D("mass_hist", "Mass Distribution",     150, 990, 1060);
	TH1D *bgHist   = new TH1D("bg_hist",   "Mass Distribution, bg", 150, 990, 1060);

	TTreeReaderValue<double> KPlusE       (*KPlusTreeReader,  "ParticleData.E"     );
	TTreeReaderValue<double> KPluspx      (*KPlusTreeReader,  "ParticleData.p_x"   );
	TTreeReaderValue<double> KPluspy      (*KPlusTreeReader,  "ParticleData.p_y"   );
	TTreeReaderValue<double> KPluspz      (*KPlusTreeReader,  "ParticleData.p_z"   );
	TTreeReaderValue<int>    KPlusNEvent  (*KPlusTreeReader,  "ParticleData.nEvent");

	TTreeReaderValue<double> KMinusE      (*KMinusTreeReader, "ParticleData.E"     );
	TTreeReaderValue<double> KMinuspx     (*KMinusTreeReader, "ParticleData.p_x"   );
	TTreeReaderValue<double> KMinuspy     (*KMinusTreeReader, "ParticleData.p_y"   );
	TTreeReaderValue<double> KMinuspz     (*KMinusTreeReader, "ParticleData.p_z"   );
	TTreeReaderValue<int>    KMinusNEvent (*KMinusTreeReader, "ParticleData.nEvent");
	
	std::vector<Event> events;

	int nEvents1, nEvents2;

	while (KPlusTreeReader->Next()) {
		nEvents1 = *KPlusNEvent;
	}

	while (KMinusTreeReader->Next()) {
		nEvents2 = *KMinusNEvent;
	}

	if ( nEvents1 > nEvents2 ) {
		events.resize( nEvents1 + 1);
	} else {
		events.resize( nEvents2 + 1);
	}
	
	std::cout << events.size() << std::endl;

	KPlusTreeReader ->Restart();
	KMinusTreeReader->Restart();

	while (KPlusTreeReader->Next()) {
		int i = *KPlusNEvent;

		FourVec temp; 
		temp.E  = *KPlusE; 
		temp.px = *KPluspx; 
		temp.py = *KPluspy; 
		temp.pz = *KPluspz; 
	
		events[i].KPlusVec.push_back( temp );
	}

	while (KMinusTreeReader->Next()) {
		int i = *KMinusNEvent;

		FourVec temp; 
		temp.E  = *KMinusE; 
		temp.px = *KMinuspx; 
		temp.py = *KMinuspy; 
		temp.pz = *KMinuspz; 
	
		events[i].KMinusVec.push_back( temp );
	}

	for (int i = 0; i < events.size(); i++) {
		for (int i1 = 0; i1 < events[i].KPlusVec.size(); i1++) {
			for (int i2 = 0; i2 < events[i].KMinusVec.size(); i2++){
				double E1  = events[i].KPlusVec[i1].E;
				double px1 = events[i].KPlusVec[i1].px;
				double py1 = events[i].KPlusVec[i1].py;
				double pz1 = events[i].KPlusVec[i1].pz;

				double E2  = events[i].KMinusVec[i2].E;
				double px2 = events[i].KMinusVec[i2].px;
				double py2 = events[i].KMinusVec[i2].py;
				double pz2 = events[i].KMinusVec[i2].pz;

				massHist->Fill(1000 * sqrt( (E1  + E2 ) * (E1  + E2 )
							 -(px1 + px2) * (px1 + px2)     
							 -(py1 + py2) * (py1 + py2)     
							 -(pz1 + pz2) * (pz1 + pz2) ));
			}
		}
	}
	

	for (int i = 0; i < events.size()/100; i++) {
		for (int j = i + 1; j < events.size()/50; j++) {
			for (int i1 = 0; i1 < events[i].KPlusVec.size(); i1++) {
				for (int i2 = 0; i2 < events[j].KMinusVec.size(); i2++){
					double E1  = events[i].KPlusVec[i1].E;
					double px1 = events[i].KPlusVec[i1].px;
					double py1 = events[i].KPlusVec[i1].py;
					double pz1 = events[i].KPlusVec[i1].pz;

					double E2  = events[j].KMinusVec[i2].E;
					double px2 = events[j].KMinusVec[i2].px;
					double py2 = events[j].KMinusVec[i2].py;
					double pz2 = events[j].KMinusVec[i2].pz;

					bgHist->Fill(1000 * sqrt( (E1  + E2 ) * (E1  + E2 )
							         -(px1 + px2) * (px1 + px2)     
							         -(py1 + py2) * (py1 + py2)     
							         -(pz1 + pz2) * (pz1 + pz2) ));
				}
			}
		}

	}


	massHist->Write();	
	bgHist  ->Write();

	inFile ->Close();
	outFile->Close();

	
	
	new TBrowser;
	
	return 0;
}














