
#ifndef __TREEWRITER__
#define __TREEWRITER__

using namespace std;

class TreeWriter
{
	public:
		TreeWriter();
		~TreeWriter();


		void Init();
		mutex fmutex;
		void Run();
		void Enque(Event *evt);	
		void Stop();

		void Write(){ tree->Write(); }
		void Close(){ file->Close(); }

	private:
		String OutputFileName;
		TFile *outfile=0;
		TTree *tree=0;
		Event event;

		queue<Event> q_event;
		bool writerEnd;

}

#endif // __TREEWRITER__
