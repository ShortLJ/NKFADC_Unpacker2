

#include "HistServer.h"

HistServer::HistServer()
{
}

HistServer::HistServer(uint16_t portnumber)
	: PortNumber(portnumber)
{
	if (portnumber)	flag_httpServer=1;
	else flag_httpServer=0;

}

HistServer::~HistServer()
{
	vector<TObject*>::iterator it_histo;
	for (it_histo=v_histograms.begin(); it_histo!=v_histograms.end(); it_histo++)
	{
		srv_http->Unregister(*it_histo);
		delete *it_histo;
	}
}


void HistServer::InitFile()
{
	if (!flag_histfile)
	{
		fprintf(stderr,"HistServer::Init(): if you want to save histogram in a file, SetHistFile(filename) first\n");
	}
	else // if ( flag_histfile)
	{
		if (outfile!=0)
		{
			fprintf(stdout,"HistServer::Init(): output file is already open: %s\n",outfile->GetName());
			fprintf(stdout,"HistServer::Init(): closing previous file\n");
			outfile->Close();
		}
		outfile = new TFile(OutputFileName.c_str(),"recreate");
	}
}
void HistServer::InitHttp()
{
	if ( flag_httpServer)
	{
		//// TODO : make ROOT http server
		fprintf(stderr,"\nhttp server not implemented yet\nWork! Labor! Jaehwan!\n");
		flag_httpServer=0;
	}
}

void HistServer::Run()
{
	histerEnd=0;
	while(1)
	{
		fmutex.lock();
		if (q_event.size()==0)
		{
			if (!histerEnd)	
			{
				fmutex.unlock();
				usleep(100000);
				continue;
			}
			else //if ( histerEnd)
			{
				fprintf(stdout,"HistServer::Run() : Finished\n");
				fmutex.unlock();
				break;
			}
		}

		event = q_event.front();
		q_event.pop();
		fmutex.unlock();

		ProcessToHistUser();
	}
}
void HistServer::Stop()
{
	fmutex.lock();
	histerEnd=1;
	fmutex.unlock();
}

void HistServer::Enque(Event *evt)
{
	fmutex.lock();
	q_event.push(*evt);
	fmutex.unlock();
}

TH1I* HistServer::MakeH1(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup)
{
	TH1I *ret = new TH1I(name,title, nbinsx,xlow,xup);
	if (flag_httpServer) srv_http->Register(/*SUBFOLDER*/"test", ret); // TODO: How to manage SUBFOLDER? 
	v_histograms.push_back(ret);
	return ret;
}
TH2I* HistServer::MakeH2(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup)
{
	TH2I *ret = new TH2I(name,title, nbinsx,xlow,xup, nbinsy,ylow,yup);
	if (flag_httpServer) srv_http->Register(/*SUBFOLDER*/"test", ret); // TODO: How to manage SUBFOLDER? 
	v_histograms.push_back(ret);
	return ret;
}

void HistServer::Write()
{
	if (!outfile->IsOpen())
	{
		fprintf(stdout,"Histogram file is not open\n");
		return;
	}
	outfile->cd();
	vector<TObject*>::iterator it;
	for (it=v_histograms.begin(); it!=v_histograms.end(); it++)
	{
		(*it)->Write();
	}

}


