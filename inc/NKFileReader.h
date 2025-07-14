
#include "DataGetter.h"

#ifndef __NKFILEREADER__
#define __NKFILEREADER__

#include "NKSig.h"

class NKFileReader : public DataGetter
{
	public:
		uint8_t *GetNextPacket() override;
		Sig Interpret(uint8_t *&tmp) override;

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
		int data_read, file_size;

		uint8_t NKPacket[8192];

		bool localEnd;
		bool readerEnd;


};


#endif // __NKFILEREADER__
