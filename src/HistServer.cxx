

#include "HistServer.h"

HistServer::HistServer()
{
	fprintf(stdout,"HistServer::HistServer()\n");
}

HistServer::HistServer(string opt_)
	: opt_http(opt_)
{
	fprintf(stderr,"HistServer::HistServer(%s)\n",opt_.c_str());
	if (opt_http=="")
	{
		flag_httpServer=0;
	}
	else
	{
		flag_httpServer=1;
		fprintf(stdout,"THttpServer : %s\n",opt_http.c_str());
		srv_http = new THttpServer(opt_http.c_str());
	}


}

HistServer::~HistServer()
{
}


void HistServer::InitFile()
{
	if (!flag_histfile)
	{
		fprintf(stderr,"HistServer::Init(): if you want to save histogram in a file, SetHistFile(filename) first\n");
	}
	else // if ( flag_histfile)
	{
		if (outfile!=0) if ( outfile->IsOpen())
		{
			fprintf(stdout,"HistServer::Init(): output file is already open: %s\n",outfile->GetName());
			fprintf(stdout,"HistServer::Init(): closing previous file\n");
			outfile->Close();
		}
		outfile = new TFile(OutputFileName.c_str(),"recreate");
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
		if (flag_httpServer) srv_http->ProcessRequests();
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
	//fmutex.lock();
	q_event.push(*evt);
	//fmutex.unlock();
}

bool HistServer::FolderParser(const char *fullname, string &objectname, string &foldername)
{
	if (!fullname || (strlen(fullname)==0))
	{
		return false;
	}
	const char *separ = strrchr(fullname,'/');
	if ( separ)
	{
		objectname = separ + 1;
		foldername.append(fullname, separ - fullname);
	}
	else // (!separ)
	{
		objectname = fullname;
	}
	return true;
}

TH1I* HistServer::MakeH1(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup)
{
	string foldername, objectname;
	if (!FolderParser(name, objectname, foldername))
	{
		fprintf(stderr,"TH1 name must be specified!\nname %s\ntitle %s\n",name,title);
		exit(-61);
	}
	TH1I *h1 = new TH1I(name,title, nbinsx,xlow,xup);
	if (flag_httpServer) srv_http->Register(foldername.c_str(), h1);
	v_histograms.push_back(h1);
	return h1;
}

TH2I* HistServer::MakeH2(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup)
{
	string foldername, objectname;
	if (!FolderParser(name, objectname, foldername))
	{
		fprintf(stderr,"TH2 name must be specified!\nname %s\ntitle %s\n",name,title);
		exit(-61);
	}
	TH2I *h2 = new TH2I(name,title, nbinsx,xlow,xup, nbinsy,ylow,yup);
	if (flag_httpServer) srv_http->Register(foldername.c_str(), h2); 
	v_histograms.push_back(h2);
	return h2;
}

void HistServer::Write()
{
	if (outfile==0 || !outfile->IsOpen())
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

void HistServer::Close()
{
	vector<TObject*>::iterator it_histo;
	if (flag_httpServer) for (it_histo=v_histograms.begin(); it_histo!=v_histograms.end(); it_histo++)
	{
		srv_http->Unregister(*it_histo);
		//delete *it_histo;
	}
	outfile->Close();
}

