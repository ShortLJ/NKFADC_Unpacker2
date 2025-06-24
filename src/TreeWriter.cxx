



void TreeWriter::Init()
{
	if (OutputFileName==NULL)
	{
		fprintf(stderr,"TreeWriter::Init(): SetFileName(filename) first\n");
		exit(-20);
	}
	if (outfile!=0)
	{
		fprintf(stdout,"TreeWriter::Init(): output file is already open: %s\n",outfile->GetName());
		fprintf(stdout,"TreeWriter::Init(): closing previous file\n");
		outfile->Close();
	}
	outfile = new TFile(OutputFileName,"recreate");
	tree = new TTree("tree","tree");
	tree->SetBranch("ASGARD", Event->ASGARD);
	tree->SetBranch("StarkJr", Event->StarkJr);

}


