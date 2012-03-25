#define _STR(x)	#x
#define STR(x)	_STR(x)

#ifndef BUILDTIME
#define BUILDTIME unknown
#endif

#ifndef VERSION
#define VERSION unknown
#endif

char *version_buildtime = STR(BUILDTIME);
char *version_release = STR(VERSION);
