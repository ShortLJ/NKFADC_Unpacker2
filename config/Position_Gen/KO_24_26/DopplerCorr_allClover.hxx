#include <iostream>
#include <vector>
#include <cmath>
#include "TMath.h"

struct Coord {
    double x, y, z;
};

// ---- 32개 세그먼트 기본 좌표 (절대 수정 금지) ----
vector<Coord> base = {
    // CRYSTAL_A

 {128.9613 , -37.1833  , -37.4119 },
 {128.6257 , -36.8860  , -13.1381 },
 {129.0913 , -13.7584  , -13.9100 },
 {128.7654 , -13.0546  , -37.1095 },

 {172.3650 , -40.5584 , -40.5599},
 {172.3650 , -40.8078 , -15.0203},
 {172.3650 , -14.7779 , -14.7784},
 {172.3650 , -15.0189 , -40.8084},


//CRYSTAL_B
 { 128.9613 , -37.2119 , 37.3833},
 { 128.6257 , -12.9381 , 37.0860},
 { 129.0914 , -13.7100 , 13.9584},
 { 128.7654 , -36.9095 , 13.2546},

 { 172.3650 , -40.3599 , 40.7584},
 { 172.3650 , -14.8203 , 41.0078},
 { 172.3650 , -14.5784 , 14.9779},
 { 172.3650 , -40.6084 , 15.2189},

//CRYSTAL_C
 { 128.9613 , 37.1833 , 37.4119},
 { 128.6257 , 36.8860 , 13.1381},
 { 129.0913 , 13.7584 , 13.9100},
 { 128.7654 , 13.0546 , 37.1095},

 { 172.4071 , 40.5584 , 40.5600},
 { 172.3650 , 40.8078 , 15.0203},
 { 172.3650 , 14.7779 , 14.7784},
 { 172.3650 , 15.0189 , 40.8084},

//CRYSTAL_D
 { 128.9613 , 37.2119 , -37.3833},
 { 128.6257 , 12.9381 , -37.0860},
 { 129.0913 , 13.7100 , -13.9584},
 { 128.7654 , 36.9095 , -13.2546},

 { 172.3650 , 40.3599 , -40.7584},
 { 172.3650 , 14.8203 , -41.0078},
 { 172.3650 , 14.5784 , -14.9979},
 { 172.3650 , 40.6084 , -15.2189}

};



// ------------------------ 변환 함수 ------------------------
//Coord TransformSpherical(const Coord& p, double r, double theta_deg, double phi_deg) {
Coord TransformSpherical(const Coord& p, double r, double theta_deg, double phi_deg) {
	double theta = TMath::DegToRad() * theta_deg;  // polar
	double phi   = TMath::DegToRad() * phi_deg;    // azimuth


	double new_px = p.x + r - 100 ;
	double r_base_seg = sqrt(new_px*new_px + p.y*p.y + p.z*p.z);
    double theta_seg  = TMath::ACos(p.z / r_base_seg);
    double phi_seg    = TMath::ATan2(p.y, new_px);

	theta_seg += theta;
    phi_seg   += phi;

	Coord newpos;
    newpos.x = r_base_seg * sin(theta_seg) * cos(phi_seg);
    newpos.y = r_base_seg * sin(theta_seg) * sin(phi_seg);
    newpos.z = r_base_seg * cos(theta_seg);
    return newpos;

}

Coord GetSegmentPosition(int det_num, int Crystal, int Segment) {
    // 구면 좌표 불러오기
    vector<double> Clover_theta = {135, 135, 90, 90, 45, 135, 90, 90, 45, 45};
    vector<double> Clover_phi   = {67.5, -22.5, 22.5, -22.5, 22.5, 157.5, 157.5, 202.5, 112.5, 202.5};
    //vector<double> Clover_dis   = {132.5, 132.5, 132.5, 132.5, 132.5, 132.5, 132.5, 132.5, 132.5, 132.5};//ARUNITA EXP
    //vector<double> Clover_dis   = {195, 195, 195, 195, 195 , 195, 195 , 195, 195, 195};//SUSO EXP
    vector<double> Clover_dis   = {158,156,106,106,163,154,105,105,158,161};//Watanabe EXP, Jason EXP
    //vector<double> Clover_dis   = {167.78032, 165.99542, 87.459954, 86.567506, 169.56522, 182.05950, 83.890160, 83.890160, 180.27460, 182.05950};//Watanabe EXP - wrong


    double theta = Clover_theta[det_num];
    double phi   = Clover_phi[det_num];
    double r     = Clover_dis[det_num];

	if( phi > 90 )
	{
		if( Crystal == 0 ) Crystal = 2;
		else if( Crystal == 1 ) Crystal = 3;
		else if( Crystal == 2 ) Crystal = 0;
		else if( Crystal == 3 ) Crystal = 1;
	}

    // 로컬 세그먼트 좌표
    Coord p = base[Crystal * 8 + Segment];


    // 변환
    return TransformSpherical(p, r, theta-90, phi);
}


double GetTheta(int det_num, int Crystal, int Segment)
{
    Coord pos = GetSegmentPosition(det_num, Crystal, Segment);
	double x = pos.x;
    double y = pos.y;
    double z = pos.z;

    double r = sqrt(x*x + y*y + z*z);
	double theta_rad = TMath::ACos(z / r);
	double theta_deg = TMath::RadToDeg() * theta_rad;
	return theta_deg;
}

double GetPhi(int det_num, int Crystal, int Segment)
{
    Coord pos = GetSegmentPosition(det_num, Crystal, Segment);
	double x = pos.x;
    double y = pos.y;
    double z = pos.z;

    double r = sqrt(x*x + y*y + z*z);
	double phi_rad= TMath::ATan2(y , x);
	double phi_deg = TMath::RadToDeg() * phi_rad;
	return phi_deg;
}

double Doppler_Corr_erg(double erg, int det_num, int Crystal, int Segment)
{

	//double beta = 0.3;
	double beta = 0.0973; //0.442 MeV/u Ne20+ Arunita EXP
	double theta = GetTheta(det_num, Crystal, Segment);
	//double erg_corr = erg*(1-beta*TMath::Cos(theta*(TMath::Pi()/180)))/sqrt(1-pow(beta,2));
	double erg_corr = erg/(1-beta*TMath::Cos(theta*TMath::DegToRad()))*sqrt(1-pow(beta,2));

	return erg_corr;	

}







