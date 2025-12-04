


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
void Config::ReadRefPartiFile(string filename)
{
	string fullpath = configdir+"/"+filename;
	fprintf(stdout, "Ref parti file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath.c_str(),"r");
	if(fr==NULL)
	{
		fprintf(stderr,"ref_parti file is not opened.\n");
		exit(-6);
	}
	char line[100];

	uint8_t param[3];
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		uint8_t chL, chU, granul, parti;
		switch (sscanf(line, "%hhu,%hhu,%hhu,%hhu,%hhu,%hhu",
			&isid,&ibrd,&icha,&param[0],&param[1],&param[2]))
		{
			case 4:
			{
				fprintf(stdout,"ref parti %u %u %u %u\n", isid,ibrd,icha, param[0]);
				chL=icha; chU=icha; granul=1;
				parti=param[0];
				break;
			}
			case 6:
			{
				fprintf(stdout,"ref parti %u %u %u %u %u %u\n", isid,ibrd,icha, param[0],param[1],param[2] );
				chL=icha, chU=param[0], granul=param[1];
				parti=param[2];
				break;
			}
			default:
			{
				fprintf(stderr,"failed to ref parti cal file\n");
				exit(-7);
			}
		}
		if (granul==0)
		{
				fprintf(stderr,"failed to ref parti cal file: granul==0\n");
				exit(-7);
		}
		for (icha=chL; icha<=chU; icha+=granul)
		{
			fprintf(stdout, "%u %u %u %u\n",isid,ibrd,icha,parti);
			for (int s = (isid == uint8_t(-1) ? 0 : isid); s < (isid == uint8_t(-1) ? N_SID : isid + 1); s++)
			for (int b = (ibrd == uint8_t(-1) ? 0 : ibrd); b < (ibrd == uint8_t(-1) ? N_BRD : ibrd + 1); b++)
			for (int c = (icha == uint8_t(-1) ? 0 : icha); c < (icha == uint8_t(-1) ? N_CHA : icha + 1); c++)
			{
				fprintf(stdout, "%d %d %d %u\n",s,b,c,parti);
				if (parti==1) participate_ref[s][b][c] = 1;
				else if (parti==0) participate_ref[s][b][c] = 0;
				else
				{
					fprintf(stderr,"failed to ref parti cal file. 0 or 1\n");
					exit(-7);
				}
			}
			if (icha == uint8_t(-1)) break;
		}
	}
}


void Config::ReadTimeOffsetFile(string filename)
{
	string fullpath = configdir+"/"+filename;
	fprintf(stdout, "time offset file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath.c_str(),"r");
	if(fr==NULL)
	{
		fprintf(stderr,"time offset file is not opened.\n");
		exit(-6);
	}
	char line[100];

	int64_t param[0];
	int overwrite=0;
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		switch (sscanf(line, "%hhu,%hhu,%hhu,%ld,%d",
			&isid,&ibrd,&icha,&param[0],&overwrite))
		{
			case 4:
			{
				overwrite=0;
				[[fallthrough]];
			}
			case 5:
			{
				fprintf(stdout,"reading %s: %u %u %u %ld,%d\n",fullpath.c_str(),isid,ibrd,icha,param[0],overwrite);
				for (int s = (isid == uint8_t(-1) ? 0 : isid); s < (isid == uint8_t(-1) ? N_SID : isid + 1); s++)
				for (int b = (ibrd == uint8_t(-1) ? 0 : ibrd); b < (ibrd == uint8_t(-1) ? N_BRD : ibrd + 1); b++)
				for (int c = (icha == uint8_t(-1) ? 0 : icha); c < (icha == uint8_t(-1) ? N_CHA : icha + 1); c++)
				{
					fprintf(stdout,"%d %d %d %ld,%d\n",s,b,c,param[0],overwrite);
					if (overwrite)	time_offset[s][b][c] = param[0];
					else 			time_offset[s][b][c] += param[0];
				}
				break;
			}
			default:
			{
				fprintf(stderr,"failed to read time offset file\n");
				exit(-7);
			}
		}
	}
	for (isid=0; isid<N_SID; isid++) for (ibrd=0; ibrd<N_BRD; ibrd++) for (icha=0; icha<N_CHA; icha++) if (enabled[isid][ibrd][icha])
	{
		fprintf(stdout, "%u %u %u / time_offset %ld\n",isid,ibrd,icha,time_offset[isid][ibrd][icha]);
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

void Config::ReadSegPosFile(string filename)
{
	string fullpath = configdir+"/"+filename;
	fprintf(stdout, "Segment Position file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath.c_str(),"r");
	if(fr==NULL)
	{
		fprintf(stderr,"Segment Position file is not opened.\n");
		exit(-6);
	}
	char line[100];

	uint8_t iclov,icrys,iseg;
	float param[3];
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		switch (sscanf(line, "%hhu,%hhu,%hhu,%f,%f,%f",
			&iclov,&icrys,&iseg,&param[0],&param[1],&param[2]))
		{
			case 6:
			{
				fprintf(stdout,"seg pos %u %u %u %f %f %f\n", iclov,icrys,iseg, param[0],param[1],param[2] );
				seg_pos_cart	[iclov][icrys][iseg][0] = param[0];
				seg_pos_cart	[iclov][icrys][iseg][1] = param[1];
				seg_pos_cart	[iclov][icrys][iseg][2] = param[2];
				break;
			}
			default:
			{
				fprintf(stderr,"failed to read seg pos file\n");
				exit(-7);
			}
		}
	}
}

void Config::ReadStripPosFile(string filename)
{
	string fullpath = configdir+"/"+filename;
	fprintf(stdout, "Strip Position file: %s\n",fullpath.c_str());

	FILE *fr;
	fr = fopen(fullpath.c_str(),"r");
	if(fr==NULL)
	{
		fprintf(stderr,"Strip Position file is not opened.\n");
		exit(-6);
	}
	char line[100];

	uint8_t ix6,istrip;
	float param[3];
	while (fgets(line, sizeof line, fr))
	{
		if (*line == '#') continue;
		switch (sscanf(line, "%hhu,%hhu,%f,%f,%f",
			&ix6,&istrip,&param[0],&param[1],&param[2]))
		{
			case 5:
			{
				fprintf(stdout,"strip pos %u %u %f %f %f\n", ix6,istrip, param[0],param[1],param[2] );
				strip_pos_cart	[ix6][istrip][0] = param[0];
				strip_pos_cart	[ix6][istrip][1] = param[1];
				strip_pos_cart	[ix6][istrip][2] = param[2];
				break;
			}
			default:
			{
				fprintf(stderr,"failed to read strip pos file\n");
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
		participate_ref[isid][ibrd][icha] = 0;
		time_offset[isid][ibrd][icha] = 0;
		map_type[isid][ibrd][icha] = 0xFF;
		map_det [isid][ibrd][icha] = 0xFF;
		map_idx [isid][ibrd][icha] = 0xFF;
		Ecal_par0 [isid][ibrd][icha] = 0;
		Ecal_par1 [isid][ibrd][icha] = 0;
	}
	for (uint8_t iclov=0; iclov<Nclover; iclov++) for (uint8_t icrys=0; icrys<Ncrystal; icrys++) for (uint8_t iseg=0; iseg<Nseg; iseg++)
	{
		seg_pos_cart	[iclov][icrys][iseg][0] = 0;
		seg_pos_cart	[iclov][icrys][iseg][1] = 0;
		seg_pos_cart	[iclov][icrys][iseg][2] = 0;
	}
	for (uint8_t ix6=0; ix6<Nx6; ix6++) for (uint8_t istrip=0; istrip<Nstrip; istrip++)
	{
		strip_pos_cart[ix6][istrip][0] = 0;
		strip_pos_cart[ix6][istrip][1] = 0;
		strip_pos_cart[ix6][istrip][2] = 0;
	}


}


