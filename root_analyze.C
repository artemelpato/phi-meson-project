#include <TTree.h>
#include <TFile.h>
#include <cmath>
#include <TH1D.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>

int root_analyze()
{

	TFile *inFile  = new TFile("tree_out.root", "READ");
	TFile *outFile = new TFile("hist_out.root", "RECREATE");
	
	TTreeReader *KPlusTreeReader =  new TTreeReader("k_plus_tree" , inFile);
	TTreeReader *KMinusTreeReader = new TTreeReader("k_minus_tree", inFile);
	
	TH1D *massHist = new TH1D("mass_hist", "Mass Distribution",     80, 990, 1060);
	TH1D *bgHist   = new TH1D("bg_hist",   "Mass Distribution, bg", 80, 990, 1060);

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

	while (KPlusTreeReader->Next()){
		KMinusTreeReader->Restart();
		while (KMinusTreeReader->Next()){
			if (*KPlusNEvent == *KMinusNEvent){
				massHist->Fill(1000 * sqrt( (*KPlusE  + *KMinusE )*(*KPlusE   + *KMinusE)
							  - (*KPluspx + *KMinuspx)*(*KPluspx + *KMinuspx)
							  - (*KPluspy + *KMinuspy)*(*KPluspy + *KMinuspy)
							  - (*KPluspz + *KMinuspz)*(*KPluspz + *KMinuspz) ) );
				continue;
			}		

			bgHist->Fill(1000 * sqrt( (*KPlusE  + *KMinusE )*(*KPlusE   + *KMinusE )
					        - (*KPluspx + *KMinuspx)*(*KPluspx + *KMinuspx)
                              	                - (*KPluspy + *KMinuspy)*(*KPluspy + *KMinuspy)
					        - (*KPluspz + *KMinuspz)*(*KPluspz + *KMinuspz) ) );

		}
	}

		
	massHist->Write();	
	bgHist  ->Write();

	inFile ->Close();
	outFile->Close();
	
	new TBrowser;
	
	return 0;
}






