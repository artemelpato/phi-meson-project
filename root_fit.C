int root_fit()
{
	TFile *file = new TFile("hist_out.root", "UPDATE");

	TH1D *peakHist = (TH1D*) file->Get("peak_hist");
	TH1D *massHist = (TH1D*) file->Get("mass_hist");

	TF1 *gamma = new TF1("gamma", "sqrt([0]^2*([0]^2 + [1]^2))", 990, 1060);
	TF1 *k     = new TF1("k", "sqrt2*[0]*[1]*gamma([0], [1])/(pi*sqrt([0]^2 + gamma([0], [1])))", 990, 1060);
	TF1 *bw    = new TF1("bw", "k([0], [1])/((x^2 - [0]^2)^2 + [0]^2*[1]^2)", 990, 1060);
	TF1 *bw2   = new TF1("bw2", "bw([0], [1]) + [2] + [3]*x + [4]*x^2", 990, 1060);

	bw->SetParameters(1019, 5);
	peakHist->Fit(bw);

	peakHist->Write("", TObject::kOverwrite);	
	file->Close();

	new TBrowser;

	return 0;
}
