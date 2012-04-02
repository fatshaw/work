#ifndef _MIS_MD5_H_
#define _MIS_MD5_H_

#include <string>
using namespace std;

typedef unsigned int UINT;
typedef unsigned char UCHAR;

class MD5 {
private:
	/* MD5 context. */
	typedef struct {
		UINT  state[4];        /* state (ABCD) */
		UINT  count[2];        /* number of bits, modulo 2^64 (lsb first) */
		UCHAR buffer[64];      /* input buffer */
	} MD5_CTX;

private:
	MD5() {};

protected:
	void static init(MD5_CTX& context);
	void static update(MD5_CTX& context, unsigned char *input, unsigned int inputLen);
	void static final(MD5_CTX& context, unsigned char digest[16]);

public:
	void static md5(const string& data, string& result);

private:
	void static encode(unsigned char *output, unsigned int *input, unsigned int len);
	void static decode(unsigned int *output, unsigned char *input, unsigned int len);
	void static transform(unsigned int state[4], unsigned char block[64]);
};

#endif
