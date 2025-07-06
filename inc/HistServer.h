
#include <mutex>
#include <string>
#include <vector>

#include "Event.h"

#include "TFile.h"
#include "TH1I.h"
#include "TH2I.h"
#include "THttpServer.h"

#ifndef __HISTSERVER__
#define __HISTSERVER__

using namespace std;

class HistServer
{
	public:
		HistServer();
		HistServer(uint16_t portnumber);
		~HistServer();

		void SetHistFile(string filename){ OutputFileName=filename; flag_histfile=1;}

		mutex fmutex;
		void Run();
		void Enque(Event *evt);	
		void Stop();

		void Write();
		void Close(){ outfile->Close(); }

		virtual void ProcessToHistUser();
		virtual void InitUser();

	protected: // shall be called in InitUser();
		void InitFile();
		void InitHttp();
		TH1I* MakeH1(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup);
		TH2I* MakeH2(const char *name, const char *title, Int_t nbinsx, Double_t xlow, Double_t xup, Int_t nbinsy, Double_t ylow, Double_t yup);
		Event event;

	private:
		vector<TObject*> v_histograms;

		string OutputFileName=NULL;
		bool flag_histfile=0;
		TFile *outfile=0;

		uint16_t PortNumber;
		bool flag_httpServer=0;
		THttpServer *srv_http;

		queue<Event> q_event;

		bool histerEnd;
};

#endif // __HISTSERVER__
