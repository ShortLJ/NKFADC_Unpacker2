

#include "TreeWriter.h"


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
	tree->SetBranch("ASGARD", event.ASGARD);
	tree->SetBranch("StarkJr", event.StarkJr);

}

void TreeWriter::Run()
{
	writerEnd=0;
	while()
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


