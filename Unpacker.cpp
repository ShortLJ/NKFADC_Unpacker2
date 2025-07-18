


#include <cstdio>
#include <csignal>
#include <cstring>
#include <string>
#include <vector>
#include <thread>
using namespace std;

//#include "Global.h"
#include "Config.h"
#include "DataGetter.h"
#include "NKFileReader.h"
#include "TimeSorter.h"
#include "EventProcessor.h"
#include "HistServer.h"
#include "HistServerUser.h"
#include "TreeWriter.h"


bool flag_loop=1;
bool flag_closing=0;
bool flag_force_quit=0;

void sig_handler(int signal)
{
	fprintf(stdout, "signal received: ");
	if ( flag_loop)
	{
		fprintf(stdout,"start Closing\n");
		flag_loop=0;
		return;
	}
	if (!flag_loop)
	{
		fprintf(stdout,"force quit\n");
		flag_force_quit=1;
		return;
	}
	if (flag_force_quit)
	{
		exit(-100);
	}

}

void print_usage()
{
	fprintf(stdout,"Usage\n");
	fprintf(stdout,"--input,-i <file.dat> or 'online'\\\n");
	fprintf(stdout,"--treeout,-to <tree.root>\\\n");
	fprintf(stdout,"--histout,-to <hists.root>\\\n");
	fprintf(stdout,"--config,-c <config>\\\n");
	fprintf(stdout,"--timewindow,-tw <timewindow=0> ## 8 ns for NKfadc\\\n");


}


int main(int argc, char *argv[])
{

	if (argc<2)
	{
		print_usage();
		return 0;
	}

	string inputfilename;
	string outputfilename = "tree.root";
	string histfilename = "hists.root";
	string configdir = "config";
	int64_t timewindow = 0;


	bool flag_online=1;

	for (int i=1; i<argc; i++)
	{
		if ((strcmp(argv[i],"--input")==0 || strcmp(argv[i],"-i")==0) && (argv[i+1]))
		{
			if (strcmp(argv[i+1],"online")==0)
			{
				flag_online=1;
			}
			else
			{
				flag_online=0;
				inputfilename = string(argv[i+1]);
			}
			i++;
		}
		else if ((strcmp(argv[i],"--treeout")==0 || strcmp(argv[i],"-to")==0) && (argv[i+1]))
		{
			outputfilename = string(argv[++i]);
		}
		else if ((strcmp(argv[i],"--histout")==0 || strcmp(argv[i],"-ho")==0) && (argv[i+1]))
			histfilename = string(argv[++i]);
		else if ((strcmp(argv[i],"--config")==0 || strcmp(argv[i],"-c")==0) && (argv[i+1]))
		{
			configdir = string(argv[++i]);
		}
		else if ((strcmp(argv[i],"--timewindow")==0 || strcmp(argv[i],"-tw")==0) && (argv[i+1]))
		{
			timewindow = atoll(argv[++i]);
		}
		else if (strcmp(argv[i],"-h")==0)
		{
			print_usage();
			return 0;
		}
		else
		{
			fprintf(stderr,"invalid opt\n");
			print_usage();
			return -1;
		}
	}



	Config config(configdir);
	config.ReadDetMapFile();

	TimeSorter timesorter; /// queue
	timesorter.SetTimeWindow(timewindow);
	thread thread_timesorter(&TimeSorter::Run, &timesorter);

	vector<DataGetter*> v_datagetter;
	if (!flag_online) 
		v_datagetter.push_back(new NKFileReader(inputfilename));
	//else // if ( flag_online)
	//	v_datagetter.push_back(new NKSharedMemory);

	vector<DataGetter*>::iterator datagetter;
	for (datagetter=v_datagetter.begin(); datagetter!=v_datagetter.end(); datagetter++)
	{
		(*datagetter)->RegisterTimeSorter(&timesorter);
		//(*datagetter)->RegisterConfig(&config); /// for time offset
	}
	vector<thread> v_thread_datagetter;
	for (datagetter=v_datagetter.begin(); datagetter!=v_datagetter.end(); datagetter++)
	{
		v_thread_datagetter.push_back(thread(&DataGetter::Loop, *datagetter));
		//fprintf(stdout,"DataGetter::Loop\n");
	}

	EventProcessor eventprocessor;
	eventprocessor.RegisterTimeSorter(&timesorter);
	eventprocessor.RegisterConfig(&config);
#define WriteTree 1
#ifdef WriteTree
	TreeWriter treewriter(outputfilename);
	eventprocessor.RegisterTreeWriter(&treewriter);
#endif

	HistServerUser histserver("http:127.0.0.1:8181?thrds=2");
	//HistServerUser histserver;
	histserver.SetHistFile(histfilename);
	histserver.InitUser();
	eventprocessor.RegisterHistServer(&histserver);
	thread thread_eventprocessor(&EventProcessor::Run, &eventprocessor);
	thread thread_histserver(&HistServer::Run, &histserver);
#ifdef WriteTree
	thread thread_treewriter(&TreeWriter::Run, &treewriter);
#endif


	fprintf(stdout,"Start while loop\n");

	constexpr useconds_t refresh_rate = 5000000;

	while (1)
	{
		signal(SIGINT,sig_handler);
		if ( flag_force_quit)
		{
			break;
		}
		if ( flag_loop)
		{
			fprintf(stdout,"looping\n");
			timesorter.fmutex_output.lock(); fprintf(stdout,"%lu\n",timesorter.GetNSorted()); timesorter.fmutex_output.unlock();
			usleep(refresh_rate);
			continue;
		}
		if (!flag_loop && !flag_closing)
		{
			for (int i=0; i<v_datagetter.size(); i++)
			{
				v_datagetter.at(i)->Stop();
			}
			for (int i=0; i<v_datagetter.size(); i++)
			{
				v_thread_datagetter.at(i).join();
			}
			timesorter.Stop();
			thread_timesorter.join();

			fprintf(stdout,"stopped readers\n");
			flag_closing=1;
			eventprocessor.Stop();
			continue;
		}
		if ( flag_closing)
		{
			if ( timesorter.GetNSorted())
			{
				fprintf(stdout,"emptying timesorter. %lu\n",timesorter.GetNSorted());
				usleep(refresh_rate);
				continue;
			}
			else
			{
				fprintf(stdout,"emptyed timesorter. Finalizing\n");
				eventprocessor.Stop();
				histserver.Stop();
#ifdef WriteTree
				treewriter.Stop();
#endif
				thread_eventprocessor.join();
				thread_histserver.join();
#ifdef WriteTree
				thread_treewriter.join();
#endif
				histserver.Write();
				histserver.Close();
#ifdef WriteTree
				treewriter.Write();
				treewriter.Close();
#endif
				fprintf(stdout,"Wrote to files and file closed\n");
				break;
			}
		}

	}
	usleep(1000000);
	fprintf(stdout,"close!\n");

	return 0;




}
