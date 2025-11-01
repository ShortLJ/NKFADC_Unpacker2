
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
		HistServer(string opt_http="");
		~HistServer();

		void SetHistFile(string filename){ OutputFileName=filename; flag_histfile=1;}
		void Init();

		mutex fmutex;
		void Run();
		void Enque(Event *evt);	
		void Stop();

		void Write();
		void Close();

		virtual void InitRaw()=0;
		virtual void ProcessToHistRaw()=0;
		virtual void InitUser()=0;
		virtual void ProcessToHistUser()=0;

	protected: // shall be called in InitUser();
		void InitFile();
		//void InitHttp();
		TH1I* MakeH1(const char *foldername, const char *name, const char *title, int nbinsx, double xlow, double xup);
		TH2I* MakeH2(const char *foldername, const char *name, const char *title, int nbinsx, double xlow, double xup, int nbinsy, double ylow, double yup);
		TH1I* MakeH1(const char *name, const char *title, int nbinsx, double xlow, double xup);
		TH2I* MakeH2(const char *name, const char *title, int nbinsx, double xlow, double xup, int nbinsy, double ylow, double yup);
		Event event;

	private:
		vector<TObject*> v_histograms;

		string OutputFileName;
		bool flag_histfile=0;
		TFile *outfile=0;

		string opt_http=0;
		bool flag_httpServer=0;
		THttpServer *srv_http;
		bool FolderParser(const char *name, string &obj, string &folder);

		queue<Event> q_event;

		bool histerEnd;
};

#endif // __HISTSERVER__
