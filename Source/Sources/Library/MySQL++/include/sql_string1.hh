#ifndef __sql_string1_hh__
#define __sql_string1_hh__

#include <string>
using namespace std ;

#include <stdio.h>
#include "defs.h"
#include "define_short.h"

//: A special string that will convert from anything.
// A class subclassed from string that has the additional ability to
// convert from any valid mysql type.

class SQLString : public string {
public:
  bool is_string;
  bool dont_escape;
  bool processed;

  SQLString() : is_string(false) , processed(false) {}
  SQLString (const string& str) :string (str)
    {is_string=true;processed=false;dont_escape=false;}
  SQLString (const char *str) :string (str) 
    {is_string=true;processed=false;dont_escape=false;}
  SQLString& operator = (const char *str) {
    string::operator = (str); return *this;
  }
  SQLString& operator = (const string& str) {
    string::operator = (str); return *this;
  }
  SQLString (char i) : is_string(false), processed(false)
    {char s[6]; sprintf(s,"%dh",(short int)i); *this=s;}
  SQLString (unsigned char i) : is_string(false), processed(false)
    {char s[6]; sprintf(s,"%uh",(short int)i); *this=s;}
  SQLString (short int i) : is_string(false), processed(false)
    {char s[6]; sprintf(s,"%dh",i); *this=s;}
  SQLString (unsigned short int i) : is_string(false), processed(false)
    {char s[6]; sprintf(s,"%uh",i); *this=s;}
  SQLString (int i) : is_string(false), processed(false)
    {char s[11]; sprintf(s,"%d",i); *this=s;}
  SQLString (unsigned int i) : is_string(false), processed(false)
    {char s[11]; sprintf(s,"%u",i); *this=s;}
/*SQLString (longlong i) : is_string(false), processed(false)
    {char s[22]; sprintf(s,"%dL",i); *this=s;}
  SQLString (ulonglong i) : is_string(false), processed(false) 
    {char s[22]; sprintf(s,"%uL",i); *this=s;}
*/
  SQLString (float i) : is_string(false), processed(false)
    {char s[40]; sprintf(s,"%g",i); *this=s;}
  SQLString (double i) : is_string(false), processed(false)
    {char s[40]; sprintf(s,"%g",i); *this=s;}
};

#endif





