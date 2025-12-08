
//#define SMALL_FILE 16


#include "NKFileReader.h"

NKFileReader::~NKFileReader()
{
	if (fp) fclose(fp);
}

uint8_t* NKFileReader::GetNextPacket()
{
	if (data_read+8192>file_size)
	//if (data_read>=file_size)
	{
		fprintf(stdout, "data_read+8192>file_size (reached file end). %ld MB abandoned.\n", file_size-data_read);
		//fprintf(stdout, "data_read >= file_size (reached file end)\n");
		localEnd=1;
		it_input++;
	}

	if (localEnd)
	{
		if (it_input != v_input.end())
		{
			fprintf(stdout, "Proceed to next file\n");
			InitFile(it_input);
		}
		else
		{
			fprintf(stdout, "All files in list are read. Last packet size %d\n",packet_size);
			readerEnd=1;
			fmutex.lock();
			getterEnd=1;
			fprintf(stdout,"getterEnd %d\n",(int)getterEnd);
			fmutex.unlock();

			packet_size=0;
			return 0;
		}
	}
	packet_size= fread(NKPacket, 1, 8192, fp);
	data_read += packet_size;

	if ((data_read & 0x3fffff)==0)
	{
		//fprintf(stdout, "\r");
		fprintf(stdout, "data_read to= %ld/%ld (%.02f%%)\t", data_read, file_size,float(data_read)/file_size*100);
		fprintf(stdout,"sig_processed %d\t", sig_processed);
		fprintf(stdout,"packet_size %d",packet_size);
		fprintf(stdout, "\n");
		fflush(stdout);
	}

	return NKPacket;
}
int NKFileReader::Interpret(uint8_t *&tmp, Sig &sig)
{
	int ret;
	uint8_t data_type = tmp[0] & 0x7F;
	uint16_t data_length=0;
	counts++; //fprintf(stdout, "counts: %ld\n",counts);
	//fprintf(stdout, "data_type: %u (%#x)\n", data_type,data_type);

	switch (data_type)
	{
		case 0x20:
		{
			//fprintf(stdout, "data_type %u: ADC data\n",data_type);
			data_length = 32;
			//print_binary(tmp,data_length);
			NKSig nksig(tmp);
			//nksig.Print();
			sig = nksig.GetSig();
			ret = 1;
			break;
		}
		case 0x40:
		{
			//fprintf(stdout, "data_type %u: TCB data\n",data_type);
			data_length = 64;
			//print_binary(tmp,data_length);
			ret = 2;
			break;
		}
		default:
		{
			fprintf(stderr, "NKFileReader::Interpret : data_type %u: unknown\n",data_type);
			data_length = packet_size;
			ret = -1;
			//exit(-61);
			break;
		}
	}
	tmp += data_length;
	return ret;
}

NKFileReader::NKFileReader(string input)
{
	ParseInput(input);
	nfile = v_input.size();
	it_input=v_input.begin();

	readerEnd=0;
	InitFile(v_input.begin());
}


void NKFileReader::ParseInput(string input)
{
	string input_type =  input.substr(input.size() - 3);
	if (input_type=="txt" || input_type=="list")
	{
		FILE *fr_list = fopen(input.c_str(),"r");
        if (fr_list==NULL)
        {
            fprintf(stdout, "cannot find %s\n",input.c_str());
            exit(-1);
        }
		char line[200];
        while (fgets(line, sizeof line, fr_list))
        {
            if (*line == '#') continue;
            string strtmp(line);
            if (!strtmp.empty() && strtmp[strtmp.length()-1] == '\n') strtmp.erase(strtmp.length()-1);
            v_input.push_back(strtmp);
        }
    }
	else if (input_type=="dat")
	{
		string strtmp(input);
		if (!strtmp.empty() && strtmp[strtmp.length()-1] == '\n') strtmp.erase(strtmp.length()-1);
        v_input.push_back(strtmp);
	}
	else
	{
		fprintf(stderr, "input file must be either *.txt *.list or *.dat\n");
		exit(-2);
	}
	for (it_input=v_input.begin(); it_input!=v_input.end(); it_input++)
	{
		fprintf(stdout,"NKFileReader: %s\n",it_input->c_str());
	}
}

int NKFileReader::InitFile(vector<string>::iterator iter)
{
	if (fp) fclose(fp);
	idx_file = iter - v_input.begin();
	fprintf(stdout, "Opening %d-th file\n",idx_file);
	fp = fopen(iter->c_str(),"rb");

	if (fp==NULL)
	{
		fprintf(stderr,"No file named \"%s\"\n",iter->c_str());
		exit(-3);
	}
	fseek(fp, 0L, SEEK_END);
	file_size = ftell(fp);
	fprintf(stdout, "Opened input File: \"%s\" (%ld Bytes)\n",iter->c_str(), file_size);
#ifdef SMALL_FILE
	fprintf(stderr,"\n\n\n File size set to be small\n\n\n");
	file_size = file_size /SMALL_FILE;
#endif //SMALL_FILE

	if (file_size<0)
	{
		fprintf(stderr, "file size is larger than 2GB\n");
		exit(-4);
	}
	fseek(fp, 0, SEEK_SET);
	data_read=0;
	localEnd=0;
	return file_size;
}



	
