#include "DopplerCorr_allClover.hxx"

void StarkJr_StripPos_to_CSV(){

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

	double width = 70; // idk
	double length = 75;
	double distance_face = 93.5;
	double distance_z = 20+2+length/2;



	Coor base_x6[8];
	for (int istrip=0; istrip<8; istrip++)
	{
		base_x6[istrip] = {0+(0.5+istrip-4)*width/8 , distance_face/2, distance_z};
		fprintf(stdout,"base: %d %d %d\n",base_x6[istrip].x,base_x6[istrip].y,base_x6[istrip].z);
	}

	Coor strip[12][8];
	for (int ix6=0; ix6=12; ix6++)
	{



	const char *filename = "strip_pos.csv";
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
		fout << "#X6,Strip,X,Y,Z\n";
	}


	for(int i = 0 ; i < 6 ; i ++)
	{
		for(int j = 0 ; j < 8 ; j ++)
		{
			fout << i
				<< "," << j
				<< "," << GetSegmentPosition(i,j,k).x 
				<< "," << GetSegmentPosition(i,j,k).y 
				<< "," << GetSegmentPosition(i,j,k).z 
				<<"\n";
		}
	}

	fout.close();
}
