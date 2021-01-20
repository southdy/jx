#ifndef __convert1_hh__
#define __convert1_hh__

using namespace std ;

#include <stdlib.h>

#include "defs.h"

template <class Type> class mysql_convert;

#define mysql__convert(TYPE, FUNC) \
  template <> \
  class mysql_convert<TYPE> { \
  private:\
    TYPE num;\
  public:\
    mysql_convert(const char* str, const char *& end) { \
      num = FUNC(str, const_cast<char **>(&end));}\
    operator TYPE () {return num;}\
  } \

//extern double strtod (const char *, char **);

mysql__convert(float, strtod);
mysql__convert(double, strtod);
//-----------------------------------------------------------------
#undef mysql__convert
#define mysql__convert(TYPE, FUNC) \
  template <> \
  class mysql_convert<TYPE> {\
  private:\
    TYPE num;\
  public:\
    mysql_convert(const char* str, const char *& end) { \
      num = FUNC(str, const_cast<char **>(&end),10);}\
    operator TYPE () {return num;}\
  }\

extern long strtol(const char *str, char **ptr, int base);
extern unsigned long strtoul(const char *str, char **ptr, int base);

mysql__convert(char, strtol);
mysql__convert(signed char, strtol);
mysql__convert(int, strtol);
mysql__convert(short int, strtol);
mysql__convert(long int, strtol);

mysql__convert(unsigned char, strtoul);
mysql__convert(unsigned int, strtoul);
mysql__convert(unsigned short int, strtoul);
mysql__convert(unsigned long int, strtoul);


#ifndef NO_LONG_LONGS
#ifdef strtoull
#undef strtoull
#endif
#ifdef strtoll
#undef strtoll
#endif
extern "C" {
extern char *longlong2str(longlong val,char *dst,int radix);
extern char *longlong10_to_str(longlong val,char *dst,int radix);
extern longlong strtoll(const char *nptr,char **endptr,int base);
extern ulonglong strtoull(const char *nptr,char **endptr,int base);
}
mysql__convert(longlong, strtoll);
mysql__convert(ulonglong, strtoull);

#endif

#endif



