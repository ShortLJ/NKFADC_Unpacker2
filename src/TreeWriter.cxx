

#include "TreeWriter.h"

TreeWriter::TreeWriter()
{
}

TreeWriter::TreeWriter(string outfilename, string treename)
	: OutputFileName(outfilename), TreeName(treename)
{
	fprintf(stdout, "TreeWriter::TreeWriter(string outfilename, string treename)\n");
	Init();
}
TreeWriter::~TreeWriter()
{
	if ( outfile->IsOpen()) outfile->Close();
}

void TreeWriter::Init()
{
	if (outfile!=0)	if ( outfile->IsOpen())
	{
		fprintf(stdout,"TreeWriter::Init(): output file is already open: %s\n",outfile->GetName());
		fprintf(stdout,"TreeWriter::Init(): closing previous file\n");
		outfile->Close();
	}
	outfile = new TFile(OutputFileName.c_str(),"recreate");
	tree = new TTree(TreeName.c_str(),TreeName.c_str());
	tree->Branch("ASGARD", &(event.ASGARD));
	tree->Branch("StarkJr", &(event.StarkJr));

}

void TreeWriter::Run()
{
	writerEnd=0;
	while(1)
	{
		fmutex.lock();
		if (q_event.size()==0)
		{
			if (!writerEnd)	
			{
				fmutex.unlock();
				usleep(100000);
				continue;
			}
			else //if ( writerEnd)
			{
				fprintf(stdout,"TreeWriter::Run() : Finished\n");
				fmutex.unlock();
				break;
			}
		}

		event = q_event.front();
		q_event.pop();
		fmutex.unlock();

		tree->Fill();
	}
}
void TreeWriter::Stop()
{
	fmutex.lock();
	writerEnd=1;
	fmutex.unlock();
}

void TreeWriter::Enque(Event *evt)
{
	fmutex.lock();
	q_event.push(*evt);
	fmutex.unlock();
}


