/*
 * ConstVar.h
 *
 *  Created on: 2010-1-22
 *      Author: root
 */

#ifndef CONSTVAR_H_
#define CONSTVAR_H_
#include <string>
#include <vector>
#include <set>
using namespace std;
//some parameter which may be changed often,so defined as const variable
//does not use #define to avoid clean all objective files and build again
extern  double partitionNum;
extern  double decayFactor  ;
extern  double cm;
extern  double cl ;
extern  double beta;
extern  int gap ;
extern  double eps ;
extern  int minPts ;
extern  int readLine;
extern  int readTimes;
extern  double globalWarp;
extern double localWarp;
extern double borderWarp;
extern const char * fileName;
extern  double minValue;
extern int readPointNum;
extern int minInnerPts;
extern set<string>DOS;
extern set<string>R2L;
extern set<string>U2L;
extern set<string>PROBING;
extern int totalNormal;
extern int totalDos;
extern int totalR2l;
extern int totalU2l;
extern int totalProbing;
extern vector<double>totalmax;
extern vector<double>totalmin;
extern const int DIMENSION;
extern const int VECTORDIMENSION;
extern double src_bytes_factor;
extern double dst_bytes_factor;
extern double dst_factor;
extern double symbalFactor;
extern double serror_factor;
extern double borderPosMin1;
extern double borderPosMax1;
extern double hot_factor;
extern double fagment_factor;
extern double protocol_factor ;
extern double service_factor;
extern double flag_factor  ;
extern double dst_srv_factor ;
extern double count_factor ;
extern double srv_count_factor ;
//define some symbolic value which will not changed using #define
//#define DIMENSION 36
//#define VECTORDIMENSION 36 //DIMENSION + PROTOCOL_TYPE_LENGTH +SERVICE_LENGTH +FLAG_LENGTH - 3

#define UNCLASSIFIED -1	//clusterId begin from 0
#define NOISE -2

#define DENSE 1
#define TRANSITIONAL 2
#define SPARSE 3
#define UNSTATUS 4

#define NORMAL 0
#define SPORADIC 1


//used in function epsilonRangeQuery
#define EXPAND 1
#define FINDNEIGHBOR 2
#define DENSE_WITH_NOCLASS 3
#define DENSE_WITH_CLASS 4
#define TRANSITIONAL_WITH_CLASS 5
#define TRANSITIONAL_WITH_NO_CLASS 6
#define DELETE_SPARSE 7
#define FINDDENSE 8
#define MERGE 9
#define JOIN 10
#define INCREMENTALEXPAND 11
#define INCREMENTALFINDNEIGHBOR 12

#define PROTOCOL_TYPE_LENGTH 2
#define SERVICE_LENGTH 7
#define FLAG_LENGTH 4

#define DURATION 0
#define PROTOCOL_TYPE 1
#define SERVICE 2
#define FLAG 3
#define SRC_BYTES 4
#define DST_BYTES 5
#define LAND 6
#define WRONG_FRAGMENT 7
#define URGENT 8
#define HOT 9
#define NUM_FAILED_LOGINS 10
#define LOGGED_IN 11
#define NUM_COMPROMISED 12
#define ROOT_SHELL  13
#define SU_ATTEMPTED  14
#define NUM_ROOT 15
#define NUM_FILE_CREATIONS 16
#define NUM_SHELLS 17
#define NUM_ACCESS_FILES 18
#define NUM_OUTBOUND_CMDS 19
#define IS_HOST_LOGIN 20
#define IS_GUEST_LOGIN 21
#define COUNT 22
#define SRV_COUNT 23
#define SERROR_RATE 24
#define SRV_SERROR_RATE 25
#define RERROR_RATE 26
#define SRV_RERROR_RATE 27
#define SAME_SRV_RATE 28
#define DIFF_SRV_RATE 29
#define SRV_DIFF_HOST_RATE 30
#define DST_HOST_COUNT 31
#define DST_HOST_SRV_COUNT 32
#define DST_HOST_SAME_SRV_RATE 33
#define DST_HOST_DIFF_SRV_RATE 34
#define DST_HOST_SAME_SRC_PORT_RATE 35
#define DST_HOST_SRV_DIFF_HOST_RATE 36
#define DST_HOST_SERROR_RATE 37
#define DST_HOST_SRV_SERROR_RATE 38
#define DST_HOST_RERROR_RATE 39
#define DST_HOST_SRV_RERROR_RATE 40

#define DELIM ','

#define NORMAL_TYPE "normal."
#define DOS_TYPE "dos."
#define R2L_TYPE "r2l."
#define U2L_TYPE "u2l."
#define PROBING_TYPE "probing."


#define MAXTHREAD  512
#define EPS_CUDA 3
#define DIMENSION_CUDA 36
#endif /* CONSTVAR_H_ */
