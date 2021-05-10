#include "Pythia8/Pythia.h"
#include <TTree.h> 
#include <TFile.h> 
#include <TH1F.h> 
#include <vector> 
#include <cmath> 
#include <fstream> 

using namespace Pythia8;

struct particleData 
{
	double E;
	double p_x;
	double p_y;
	double p_z;
	int    nEvent;
};

int main()
{
	int  nCollisions;

	fstream fCollision ("ncollisions.txt");;
	fCollision >> nCollisions;

	particleData particle;

	TFile *outFile = new TFile("tree_out.root", "RECREATE");

	TTree *KPlusTree = new TTree("k_plus_tree", "Tree kek");
	TTree *KMinusTree = new TTree("k_minus_tree", "Tree kek");

	KPlusTree ->Branch("ParticleData", &particle, "E/D:p_x/D:p_y/D:p_z/D:nEvent/I");
	KMinusTree->Branch("ParticleData", &particle, "E/D:p_x/D:p_y/D:p_z/D:nEvent/I");

	Pythia pythia;
	pythia.readString("Beams:eCM = 8000.");
	pythia.readString("HardQCD:all = on");
	pythia.readString("PhaseSpace:pTHatMin = 20.");

	pythia.init();

	for (int iEvent = 0; iEvent < nCollisions; ++iEvent)
	{
		if (!pythia.next()) continue;

		for (int i = 0; i < pythia.event.size(); ++i)
		{
	 		if ((pythia.event[i].isFinal()) && (pythia.event[i].id() == 321)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.nEvent = iEvent;
				KPlusTree->Fill();
			}		

	 		if ((pythia.event[i].isFinal()) && (pythia.event[i].id() == -321)) 
			{
				particle.E   = pythia.event[i].e();
				particle.p_x = pythia.event[i].px();
				particle.p_y = pythia.event[i].py();
				particle.p_z = pythia.event[i].pz();
				particle.nEvent = iEvent;
				KMinusTree->Fill();
			}		

		}
	}

	KMinusTree->Write();
	KPlusTree ->Write();

	KMinusTree->Print();
	KPlusTree->Print();

	outFile->Close();
}


















