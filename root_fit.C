int root_fit()
{
	TFile *file = new TFile("hist_out.root", "UPDATE");

	TH1D *massHist = (TH1D*) file->Get("mass_hist");
	TH1D *bgHist   = (TH1D*) file->Get("bg_hist"  );
	TH1D *peakHist = (TH1D*) massHist->Clone();

	TF1 *gamma = new TF1("gamma", "sqrt([0]^2*([0]^2 + [1]^2))", 990, 1060);
	TF1 *k     = new TF1("k", "sqrt2*[0]*[1]*gamma([0], [1])/(pi*sqrt([0]^2 + gamma([0], [1])))", 990, 1060);
	TF1 *bw    = new TF1("bw", "k([0], [1])/((x^2 - [0]^2)^2 + [0]^2*[1]^2)", 990, 1060);

	bw->SetParameters(1019, 5);

	massHist->Scale(2/massHist->Integral());
	bgHist  ->Scale(1/bgHist  ->Integral());

	peakHist->Add(bgHist, -1);
	peakHist->SetName("peak_hist");
	peakHist->Fit(bw);
	peakHist->Write("", TObject::kOverwrite);	

	file->Close();

	new TBrowser;

	return 0;
}
