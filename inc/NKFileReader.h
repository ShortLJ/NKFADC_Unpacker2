
#include "DataGetter.h"

#ifndef __NKFILEREADER__
#define __NKFILEREADER__

#include "NKSig.h"

#include <stdio.h>
#define _FILE_OFFSET_BITS 64

class NKFileReader : public DataGetter
{
	public:
		uint8_t *GetNextPacket() override;
		int Interpret(uint8_t *&tmp, Sig &sig) override;

	public:
		NKFileReader(string input);
		~NKFileReader();
	private:
		void ParseInput(string input);
		int InitFile(vector<string>::iterator it_input);
		vector<string> v_input;
		vector<string>::iterator it_input;

		FILE *fp;
		int idx_file, nfile;
		off_t data_read, file_size;

		uint8_t NKPacket[8192];

		bool localEnd;
		bool readerEnd;

		long counts=0;
};


#endif // __NKFILEREADER__
