#include  <stdio.h>
#include  <stdlib.h>
#include  <stdint.h>
#include  <assert.h>
#include  <string.h>



//input:ab
//output:ba
//length of a = 6
//length of b = 2
int encode(const  void*  raw_in,  void*  raw_out,  uint32_t  password,  size_t  len)
{
	const  uint8_t*  in  =  (const  uint8_t*)raw_in;
	uint8_t*  out  =  (uint8_t*)raw_out;

	uint32_t  seed  =  password  ^  0xfb422cf2u;
	for  (size_t  i  =  0  ;  i  <  len;  ++i)  {
		uint8_t  a  =  (  in[i]  ^  seed  )  >>  2;		//it contains the information of the 6 low bits in in[i]
		uint8_t  b  =  (  (  ((uint32_t)in[i])  <<  15  )  ^  seed  )  >>  (15-6);//b contais the information of the 2 high bits in in[i]
		a  &=  63;		//a contains the last 6 bits
		b  &=  192;		//b contains the first 2 bits
		a  =  63  &  (  a  ^  (b  <<  3));		//the statement is bullshit, useless.
		out[i]  =  a  |  b;						//conbine a|b as the result
		seed  =  (((seed  <<  7)  ^  seed  ^  out[i])  +  3687989);	//change the seed
	}
}

//input:ba
//output:ab
//length of a = 6
//length of b = 2
int  decode(const  void*  raw_in,  void*  raw_out,  uint32_t  password,  size_t  len)
{
	const  uint8_t*  in  =  (const  uint8_t*)raw_in;
	uint8_t*  out  =  (uint8_t*)raw_out;

	uint32_t  seed  =  password  ^  0xfb422cf2u;
	for  (size_t  i  =  0  ;  i  <  len;  ++i)  {
		uint8_t a = in[i] & 63;		//in[i] is the  out[i] in the encode process. a is the last 6 bits and b is the first 2 bits
		uint8_t b = in[i] & 192;
		a = ((a <<2)^seed)&0xfc;	//reverse the process for a in the encode process. the last 2 bits in the seed should be cleaned.
		b = (((uint32_t(b)<<9)^seed)>>15)&3;//reverse the process for b in the encode process. only remain the last 2 bits. 
		out[i] = a|b;	//combine a|b as the result
		seed = (((seed  <<  7)  ^  seed  ^  in[i])  +  3687989);	//change the seed
	}
}
int  main()
{
	/*const  uint8_t  buf1[]  =  {0x8a,  0x77,  0x10,  0x05,  0x73,  0x58,  0xfa,  0x9e,  0xd3,  0x7f,  0x27,  0xdb,  0x08,  0x40,  0x62,  0xfb,  0x6b,  0x9f,  0x00,  0x54,  0xb6,  0x7a,  0x27,  0x3c,  0x01,  0x3e,  0x77,  0x1b,  0x10,  0xfd,  0x8e,  0x16,  0xe9,  0x93,  0x5a,  0xf9,  0xd5,  0x43,  0xe9,  0x7d,  0x0d,  0x14,  0x34,  0xb6,  0x88,  0x67,  0x2b,  0x9e,  0x16,  0x60,  };
	uint8_t  buf2[100]  =  {};*/
	char * input = "what funking are you doing";
	uint8_t buf1[100];
	uint8_t buf2[100];
	const  uint32_t  password  =  0x5545fa17u;
	const  size_t  len  =  sizeof(buf1);
	encode(input,buf1,password,len);
	decode(buf1,  buf2,  password,  len);
	printf("%s\n",  buf2);
}
