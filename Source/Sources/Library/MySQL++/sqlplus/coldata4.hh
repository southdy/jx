#ifndef __coldata4_h__
#define __coldata4_h__

#include "convert3.hh"

#include "coldata3.hh"

#include "string_util.hh"

// FIX: the implementation of the conv function has moved to coldata1.hh
/*
template <class Str, class Type>
Type mysql_ColData<Str>::conv<Type> (Type dummy) const {
	string strbuf(buf);
	strip_all_blanks(strbuf);
  size_t len = strbuf.size();
  const char *str = strbuf.c_str();
  const char *end = str;
  Type num = mysql_convert<Type>(str, end);
  if (*end == '.') {
    end++;
    for (;*end == '0'; end++);
  }
  if (*end != '\0' && end != NULL ) {
    throw BadConversion (typeid(Type).name(), c_str(), end - str, len);
  }
  return num;
}
*/

#endif
