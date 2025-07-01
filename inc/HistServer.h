
#ifndef __HISTSERVER__
#define __HISTSERVER__

using namespace std;

class HistServer
{
	public:
		HistServer();
		HistServer(uint16_t portnumber);
		~HistServer();

		void SetHistFile(String filename){ OutputFileName=filename; flag_histfile=1;}

		mutex fmutex;
		void Run();
		void Enque(Event *evt);	
		void Stop();

		void Write(){ tree->Write(); }
		void Close(){ file->Close(); }

		virtual void ProcessToHistUser();
		virtual void InitUser();

	protected: // shall be called in InitUser();
		void InitFile();
		void InitHttp();
		TH1* MakeH1();
		TH2* MakeH2();

	private:
		vector<TH1*> v_histograms;

		String OutputFileName=NULL;
		bool flag_histfile=0;
		TFile *outfile=0;

		uint16_t PortNumber;
		bool flag_httpServer=0;

		Event event;
		queue<Event> q_event;

		bool histerEnd;
}

#endif // __HISTSERVER__
