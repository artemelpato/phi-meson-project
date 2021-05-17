int root_fit()
{
	TFile *inFile = new TFile("hist_out.root", "READ");
	TFile *outFile = new TFile("peak_hist_out.root", "RECREATE");

	TH1D *peakHist = (TH1D*) inFile->Get("mass_hist");
	TH1D *bgHist   = (TH1D*) inFile->Get("bg_hist"  );

	TF1 *gamma = new TF1("gamma", "sqrt([0]^2*([0]^2 + [1]^2))", 990, 1060);
	TF1 *k     = new TF1("k", "2*sqrt(2)*[0]*[1]*gamma([0], [1])/(pi*sqrt([0]^2 + gamma([0], [1])))", 990, 1060);
	TF1 *bw    = new TF1("bw", "k([0], [1])/((x^2 - [0]^2)^2 + [0]^2*[1]^2)", 990, 1060);

	bw->SetParameters(1020, 5);

	peakHist->Scale(1/peakHist->Integral(130, 150));
	bgHist  ->Scale(1/bgHist  ->Integral(130, 150));

	peakHist->Add(bgHist, -1);
	peakHist->Scale(1/bw->Integral(990, 1060));
	peakHist->SetName("peak_hist");
	peakHist->Fit(bw);
	peakHist->Write();	

	inFile ->Close();
	outFile->Close();

	new TBrowser;

	return 0;
}
