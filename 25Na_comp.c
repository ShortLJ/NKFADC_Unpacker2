
{
TFile *_file0 = TFile::Open("hists_bg.root");
TFile *_file1 = TFile::Open("hists_blank2.root");
TFile *_file2 = TFile::Open("hists_stopper14.root");
TH2I *h2;
_file0->cd("Clover"); h2 = (TH2I*) gDirectory->Get("h2_Time_Energy_fv_hemi"); TH1D *h1_bg = h2->ProjectionY("E_bg",1,60);
_file1->cd("Clover"); h2 = (TH2I*) gDirectory->Get("h2_Time_Energy_fv_hemi"); TH1D *h1_bl = h2->ProjectionY("E_bl",1,60);
_file2->cd("Clover"); h2 = (TH2I*) gDirectory->Get("h2_Time_Energy_fv_hemi"); TH1D *h1_st = h2->ProjectionY("E_st",1,60);
THStack *hs = new THStack();

h1_bl->Add(h1_bg,-1);
h1_st->Add(h1_bg,-1);

//hs->Add(h1_bg); h1_bg->SetLineColor(kRed);
hs->Add(h1_bl); h1_bl->SetLineColor(kGreen);
hs->Add(h1_st); h1_st->SetLineColor(kBlue);
hs->Draw("nostack hist");

TLegend *lgd = new TLegend(0.8,0.8,1,1);
//lgd->AddEntry(h1_bg,"BG 0");
lgd->AddEntry(h1_bl,"Blank 6");
lgd->AddEntry(h1_st,"Stopper 2");

lgd->Draw();
}
