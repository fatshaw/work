/*
 * ConstVar.cpp
 *
 *  Created on: 2010-1-31
 *      Author: root
 */

#include "ConstVar.h"

int readLine = 200;
int readTimes  =3000;

double decayFactor = 0.998;
double cm = 3;// 2;
double cl = 1.0;
double beta =0.3;
int gap = 20000;
double eps =1.5;//1.5
int minPts = 3;//3
double src_bytes_factor = 1;
double dst_bytes_factor = 1;
double dst_factor = 1;
double dst_srv_factor = 1;
double count_factor = 1;
double srv_count_factor =125;// 125;
double serror_factor = 1;
double protocol_factor  =3;//3
double service_factor  =3;//3
double flag_factor  =3;//3
double hot_factor =4000;//5000,4000;//10e-4//4000//2000
double fagment_factor =25;//30,//25
int readPointNum = 0;
int minInnerPts = 3;
double minValue = 0.00001;
const char * fileName ="./file/kddcup.data_10_percent_corrected";//"./file/DataSet4_origin.txt";	//"./file/kddcup.data_10_percent_corrected";//nor_smu_nep_bo.txt" ;//kddcup.data_10_percent_corrected";// "nor_smu_nep_bo.txt"; //; //"test_normal_neptune_smurf_back_ipsweep_guesspasswd";//
double partitionNum = 5;
const int DIMENSION  = 36;
const int VECTORDIMENSION = 36;
double globalWarp = 25;//50
double borderWarp = 11;
double localWarp = 2;

double borderPosMin1 = 0.44;//0.28; //0.36;//0.43
double borderPosMax1 = 0.56;//0.72; //0.64;//0.57

set<string>DOS;
set<string>R2L;
set<string>U2L;
set<string>PROBING;
vector<double>totalmax;
vector<double>totalmin;

int totalNormal = 0;
int totalDos = 0;
int totalR2l = 0;
int totalU2l = 0;
int totalProbing =0;

clock_t total_time = 0;
