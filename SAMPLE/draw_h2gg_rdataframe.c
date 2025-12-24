


{
	ROOT::EnableImplicitMT(10);
	ROOT::RDataFrame df("simpletree","simple/*.root");
	ROOT::RDF::Experimental::AddProgressBar(df);

	auto pairX = [&](vector<float> vdcabEgamma)-> vector<float>
	{
		vector<float> EgX;
		for (int i=0; i<vdcabEgamma.size(); i++) for (int j=0; j<vdcabEgamma.size(); j++) if (i!=j) EgX.push_back(vdcabEgamma[i]);
		return EgX;
	};
	auto pairY = [&](vector<float> vdcabEgamma)-> vector<float>
	{
		vector<float> EgY;
		for (int i=0; i<vdcabEgamma.size(); i++) for (int j=0; j<vdcabEgamma.size(); j++) if (i!=j) EgY.push_back(vdcabEgamma[j]);
		return EgY;
	};

	auto df2d = df.Define("EgX",pairX,{"vdcabEgamma"}).Define("EgY",pairY,{"vdcabEgamma"});

	auto h2_labtheta_Eparticle = df.Histo2D({"h2_labtheta_Eparticle","h2_labtheta_Eparticle",180,0,180, 2000,0,200e3},"labtheta","Eparticle");
	auto h1_dcabEgamma = df.Histo1D({"h1_dcabEgamma","h1_dcabEgamma",2000,0,4000},"vdcabEgamma");
	auto h2_dcabEgamma_dcabEgamma = df2d.Histo2D({"h2_dcabEgamma_dcabEgamma","h2_dcabEgamma_dcabEgamma",1000,0,2000,1000,0,2000},"EgX","EgY")

	//TCutG *CUTG=0; // define your CUTG
	//auto df2d_cut = df2d.Filter([&](double labtheta, double Eparticle){return CUTG->IsInside(labtheta,Eparticle);},{"labtheta","Eparticle"})
	//auto h2_dcabEgamma_dcabEgamma_cut = df2d_cut.Histo2D({"h2_dcabEgamma_dcabEgamma_cut","h2_dcabEgamma_dcabEgamma_cut",1000,0,2000,1000,0,2000},"EgX","EgY")

}
