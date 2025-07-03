


#include "Config.h"

Config::Config()
{
	InitializeGlobalVariables();
}
Config::Config(string configdir_) : Config()
	: configdir(configdir_)
{


}

Config::~Config();
{
}

void Config::ReadDetMapFile(string filename="det_map.csv")
{
	string fullpath = configdir+filename;
	fprintf(stdout, "Detector Map file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath,"r");
	if(fr==NULL)
	{
		fprintf(stderr,"map file is not opened.\n");
		exit(-6);
	}
	char line[100];

	uint8_t param[2];
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		switch (sscanf(line, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu",
			&itype,&idet,&iidx,&isid,&imid,&icha,&param[0],&param[1])	)
		{
			case 8:
			{
				uint8_t chL=icha, chU=param[0], granul=param[1];
				fprintf(stdout,"program %u %u %u %u %u %u %u %u\n",itype,idet,iidx,isid,imid,chL,chU,incre);
				for (icha=chL; ich<=chU; ich+=granul)
				{
					map_type	[isid][imid][ich] = itype;
					map_det		[isid][imid][ich] = idet;
					map_idx		[isid][imid][ich] = iidx++;
				}
				break;
			}
			case 6:
			{
				//uint8_t asdf = param[0], aaa=param[1];
				fprintf(stdout,"brute %u %u %u %u %u %u\n", itype,idet,iidx, isid,imid,icha);
				map_type	[isid][imid][ich] = itype;
				map_det		[isid][imid][ich] = idet;
				map_idx		[isid][imid][ich] = iidx;
				break;
			}
			default: 
			{
				fprintf(stderr,"failed to read det map file\n");
				exit(-7);
			}
		}
	}
}

void Config::InitializeGlobalVariables()
{
	for (isid=0; isid<N_SID; isid++) for (ibrd=0; ibrd<N_BRD; ibrd++) for (icha=0; icha<N_CHA; icha++)
	{
		map_type[isid][ibrd][icha] = 0xFF;
		map_det [isid][ibrd][icha] = 0xFF;
		map_idx [isid][ibrd][icha] = 0xFF;
	}
}


