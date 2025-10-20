


#include "Config.h"

Config::Config()
{
	InitializeGlobalVariables();
}
Config::Config(string configdir_)
	: configdir(configdir_)
{
	InitializeGlobalVariables();

}

Config::~Config()
{
}

void Config::ReadDetMapFile(string filename)
{
	string fullpath = configdir+"/"+filename;
	fprintf(stdout, "Detector Map file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath.c_str(),"r");
	if(fr==NULL)
	{
		fprintf(stderr,"map file is not opened.\n");
		exit(-6);
	}
	char line[100];

	uint8_t param[2];
	uint8_t itype, idet, iidx;
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		switch (sscanf(line, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu",
			&itype,&idet,&iidx,&isid,&ibrd,&icha,&param[0],&param[1])	)
		{
			case 8:
			{
				uint8_t chL=icha, chU=param[0], granul=param[1];
				fprintf(stdout,"det map program %u %u %u %u %u %u %u %u\n",itype,idet,iidx,isid,ibrd,chL,chU,granul);
				for (icha=chL; icha<=chU; icha+=granul)
				{
					enabled		[isid][ibrd][icha] = true;
					map_type	[isid][ibrd][icha] = itype;
					map_det		[isid][ibrd][icha] = idet;
					map_idx		[isid][ibrd][icha] = iidx++;
					//fprintf(stdout,"%u %u %u / type %u det %u idx %u\n",isid,ibrd,icha, map_type[isid][ibrd][icha],map_det[isid][ibrd][icha],map_idx[isid][ibrd][icha]);
				}
				break;
			}
			case 6:
			{
				//uint8_t asdf = param[0], aaa=param[1];
				fprintf(stdout,"det map brute %u %u %u %u %u %u\n", itype,idet,iidx, isid,ibrd,icha);
				enabled		[isid][ibrd][icha] = true;
				map_type	[isid][ibrd][icha] = itype;
				map_det		[isid][ibrd][icha] = idet;
				map_idx		[isid][ibrd][icha] = iidx;
				break;
			}
			default: 
			{
				fprintf(stderr,"failed to read det map file\n");
				exit(-7);
			}
		}
	}
	
	for (isid=0; isid<N_SID; isid++) for (ibrd=0; ibrd<N_BRD; ibrd++) for (icha=0; icha<N_CHA; icha++) if (enabled[isid][ibrd][icha])
	{
		fprintf(stdout,"%u %u %u / type %u det %u idx %u\n",isid,ibrd,icha, map_type[isid][ibrd][icha],map_det[isid][ibrd][icha],map_idx[isid][ibrd][icha]);
	}

}
void Config::ReadErgCalFile(string filename)
{
	string fullpath = configdir+"/"+filename;
	fprintf(stdout, "Energy Calibration file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath.c_str(),"r");
	if(fr==NULL)
	{
		fprintf(stderr,"Ecal file is not opened.\n");
		exit(-6);
	}
	char line[100];

	float param[3];
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		switch (sscanf(line, "%hhu,%hhu,%hhu,%f,%f,%f",
			&isid,&ibrd,&icha,&param[0],&param[1],&param[2]))
		{
			case 6:
			{
				fprintf(stdout,"erg cal %u %u %u %f %f %f\n", isid,ibrd,icha, param[0],param[1],param[2] );
				Ecal_par0	[isid][ibrd][icha] = param[0];
				Ecal_par1	[isid][ibrd][icha] = param[1];
				Ecal_par2	[isid][ibrd][icha] = param[2];
				break;
			}
			default: 
			{
				fprintf(stderr,"failed to read erg cal file\n");
				exit(-7);
			}
		}
	}

}



void Config::InitializeGlobalVariables()
{
	for (isid=0; isid<N_SID; isid++) for (ibrd=0; ibrd<N_BRD; ibrd++) for (icha=0; icha<N_CHA; icha++)
	{
		enabled [isid][ibrd][icha] = 0;
		map_type[isid][ibrd][icha] = 0xFF;
		map_det [isid][ibrd][icha] = 0xFF;
		map_idx [isid][ibrd][icha] = 0xFF;
		Ecal_par0 [isid][ibrd][icha] = 0;
		Ecal_par1 [isid][ibrd][icha] = 0;
	}
}


