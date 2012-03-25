#ifndef AC_UTIL_STREAM_H_
#define AC_UTIL_STREAM_H_


#include <stdlib.h>
#include <sys/types.h>


namespace ac
{

enum {
	TEXT_STREAM_PROTOCOL	= 0x01,
	BINARY_STREAM_PROTOCOL	= 0x02,
	TEXT_STREAM_PROTOCOL_2	= 0x03,
};

enum {
	COMPOSITE_FLAG = 0x10000000,
	COMPOSITE_TEXT_STREAM_PROTOCOL		= TEXT_STREAM_PROTOCOL | COMPOSITE_FLAG,
	COMPOSITE_BINARY_STREAM_PROTOCOL	= BINARY_STREAM_PROTOCOL | COMPOSITE_FLAG,
	COMPOSITE_TEXT_STREAM_PROTOCOL_2	= TEXT_STREAM_PROTOCOL_2 | COMPOSITE_FLAG
};


struct ReadStream
{
	virtual ~ReadStream(){}

	virtual size_t GetSize() const = 0;

	virtual bool IsEmpty() const = 0;	
	
	virtual bool Read(char* str, size_t strlen, /* out */ size_t & len) = 0;

	virtual bool Read(/* out */ int & i) = 0;

	virtual bool Read(/* out */ short & i) = 0;

	virtual bool Read(/* out */ char & c) = 0;

	virtual size_t ReadAll(char * szBuffer, size_t iLen) const = 0;

	virtual bool IsEnd() const = 0;
};

struct WriteStream
{
	virtual ~WriteStream(){}

	virtual bool Write(const char* str, size_t len) = 0;

	virtual bool Write(int i) = 0;

	virtual bool Write(short i) = 0;

	virtual bool Write(char c) = 0;

	virtual void Clear() = 0;

	virtual void Flush() = 0;
};


} // namespace ac

#endif // AC_UTIL_STREAM_H_

