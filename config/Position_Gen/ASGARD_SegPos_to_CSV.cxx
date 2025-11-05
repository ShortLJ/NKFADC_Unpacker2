#include "DopplerCorr_allClover.hxx"

void ASGARD_SegPos_to_CSV(){

	//GetSegmentPosition(Clover_num, Crystal_num, Segment_num);

	int Clover=8;
	int Crystal=3;
	int Sega=2;

	cout<<"x : "<<GetSegmentPosition(Clover,Crystal,Sega).x<<endl;
	cout<<"y : "<<GetSegmentPosition(Clover,Crystal,Sega).y<<endl;
	cout<<"z : "<<GetSegmentPosition(Clover,Crystal,Sega).z<<endl;

	cout<<"theta : "<<(GetTheta(Clover,Crystal,Sega)+GetTheta(Clover,Crystal+2,Sega))/2<<endl;
	cout<<"phi : "  <<(GetPhi(Clover,Crystal,Sega)+GetPhi(Clover,Crystal+2,Sega))/2<<endl;
	cout<<"Corrected energy : "<<Doppler_Corr_erg(454,Clover,Crystal,Sega)<<endl;





	const char *filename = "seg_pos.csv";
	ifstream checkFile(filename);
	if (checkFile.is_open())
	{
		std::cout << "File '" << filename << "' exists." << std::endl;
		std::cout << "Move existing file for backup." << std::endl;
		checkFile.close();
		return;
	}
	checkFile.close();



	// CSV 파일 저장
	ofstream fout(filename);
	if (fout.tellp() == 0) {
		fout << "#Clover,Crystal,Segment,X,Y,Z\n";
		//fout << "Clover,Crystal,Segment,X,Y,Z,Theta,Phi\n";
	}


	for(int i = 0 ; i < 10 ; i ++)
	{
		for(int j = 0 ; j < 4 ; j ++)
		{
			for(int k = 0 ; k < 8 ; k ++)
			{
				fout << i<< 
					"," << j
					<< "," << k
					<< "," << GetSegmentPosition(i,j,k).x 
					<< "," << GetSegmentPosition(i,j,k).y 
					<< "," << GetSegmentPosition(i,j,k).z 
//					<< "," << GetTheta(i,j,k) 
///					<< "," << GetPhi(i,j,k) 
					<<"\n";

			}
		}
	}

	fout.close();
}
