



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
	}
	if (!flag_loop)
	{
		fprintf(stdout,"force quit\n");
		flag_force_quit=1;
	}

}




int main(int argc, char *argv[])
{



	Config config;
	config.ReadConfig(configdir);


	Timesorter timesorter; /// queue
	timesorter.SetTimeWindow(timewindow);
	thread thread_timesorter(&Timesorter::Run, &timesorter);

	vector<DataGetter*> v_datagetter
	v_datagetters.push_back(new DataGetter());

	vector<DataGetter>::iterator datagetter;
	for (datagetter=v_datagetter.begin(); datagetter!=v_datagetter.end(); datagetter++)
	{
		datagetter->RegisterTimeSorter(&timesorter);
		datagetter->RegisterConfig(&config); /// for time offset
	}
	vector<thread> v_thread_datagetter;
	for (datagetter=v_datagetter.begin(); datagetter!=v_datagetter.end(); datagetter++)
	{
		v_thread_datagetter.push_back(thread(&DataGetter::Loop, datagetter));
	}

	EventProcessor eventprocessor;
	eventprocessor.RegisterTimeSorter(&timesorter);
	eventprocessor.RegisterConfig(&config);

	HistServer histserver;
	TreeWriter treewriter(outputfilename);
	eventprocessor.RegisterHistServer(&histserver);
	eventprocessor.RegisterTreeWriter(&treewriter);

	thread thread_eventprocessor(&EventProcessor::Run, &eventprocessor);
	thread thread_histserver(&HistServer::Run, &histserver);
	thread thread_treewriter(&TreeWriter::Run, &treewriter);




	constexpr refresh_rate = 5000000;

	while ()
	{
		signal(SIGINT,sig_handler);
		if ( flag_force_quit) exit(0);
		if ( flag_loop)
		{
			fprintf(stdout,"looping %d\n",seq);
			usleep(refresh_rate);
			continue;
		}
		if (!flag_loop && !flag_closing)
		{
			for (int i=0; i<v_datagetter.size(); i++)
				v_datagetter.at(i).Stop();
			for (int i=0; i<v_datagetter.size(); i++)
				v_thread_datagetter.at(i).join();
			timesorter.Stop();
			thread_timesorter.join();

			fprintf(stdout,"stopped readers\n");
			flag_closing=1;
			continue;
		}
		if ( flag_closing)
		{
			if (!timesorter.AllEmpty())
			{
				fprintf(stdout,"emptying timesorter\n");
				usleep(refresh_rate);
				continue;
			}
			else
			{
				fprintf(stdout,"emptyed timesorter. Finalizing\n");
				eventprocessor.Stop();
				histserver.Stop();
				thread_eventprocessor.join();
				thread_histserver.join();
				eventprocessor.Write();
				fprintf(stdout,"Wrote to files\n");
			}
		}

	}

	return 0;




}
