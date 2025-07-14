
#include <mutex>
#include <queue>
#include <string>

#include "Event.h"

#include "TFile.h"
#include "TTree.h"


#ifndef __TREEWRITER__
#define __TREEWRITER__

using namespace std;

class TreeWriter
{
	public:
		TreeWriter();
		TreeWriter(string OutputFileName, string TreeName="tree");
		~TreeWriter();


		void Init();
		mutex fmutex;
		void Run();
		void Enque(Event *evt);	
		void Stop();

		void Write(){ outfile->cd(); tree->Write(); }
		void Close(){ outfile->Close(); }

	private:
		string OutputFileName;
		string TreeName;
		TFile *outfile=0;
		TTree *tree=0;
		Event event;

		queue<Event> q_event;
		bool writerEnd;

};

#endif // __TREEWRITER__
