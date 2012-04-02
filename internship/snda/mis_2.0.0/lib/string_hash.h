#ifndef _STRING_HASH_H
#define _STRING_HASH_H

#include <string>
#include <ext/hash_set>
#include <ext/hash_map>
#include <algorithm>

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
        template<> struct hash< std::string >
        {
                size_t operator()( const std::string& x ) const
                {
                        return hash< const char* >()( x.c_str() );
                }
        };
}

typedef hash_set<string, hash<string> >  string_hash_set;
typedef hash_map<string, int, hash<string> >  string_hash_map;

#endif
