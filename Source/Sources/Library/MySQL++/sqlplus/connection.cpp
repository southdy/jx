#include <windows.h>

#include "connection3.hh"
#include "result3.hh"

//--------------------------------------------------------------------------
Connection::Connection (const char *db, const char *host, const char *user,
			const char *passwd, bool te)
  : throw_exceptions(te), locked(false)
{
	mysql_init(&mysql);
  if (connect (db, host, user, passwd))
	{
    locked = false;
    Success = is_connected = true;
  }
  else
  {
    locked = false; Success = is_connected = false;
    if (throw_exceptions) throw BadQuery(error());
  }
}
//--------------------------------------------------------------------------
Connection::Connection (const char *db, const char *host, const char *user,
			const char *passwd, uint port, my_bool compress,
			unsigned int connect_timeout, bool te,
			const char *socket_name)
  : throw_exceptions(te), locked(false)
{
	mysql_init(&mysql);
  if (real_connect (db, host, user, passwd, port, compress,		connect_timeout,socket_name))
  {
    locked = false;
    Success = is_connected = true;
  }
  else
  {
    locked = false; Success = is_connected = false;
    if (throw_exceptions) throw BadQuery(error());
  }
}
//--------------------------------------------------------------------------
bool Connection::real_connect (cchar *db, cchar *host, cchar *user,
			       cchar *passwd, uint port, my_bool compress,
			       unsigned int connect_timeout,
			       const char *socket_name)
{
  if (socket_name && socket_name[0])
    mysql.options.unix_socket = (char *)socket_name;
  else
    mysql.options.unix_socket=NULL;
  mysql.options.port = port;
  mysql.options.compress = compress;
  mysql.options.connect_timeout=connect_timeout;
  locked = true;
  if (mysql_connect(&mysql, host, user, passwd))
  {
    locked = false;
    Success = is_connected = true;
  }
  else
  {
    locked = false; Success = is_connected = false;
    if (throw_exceptions) throw BadQuery(error());
  }
  if (!Success) return Success;
  if (db[0]) // if db is not empty
    Success = select_db(db);
  return Success;
}
//--------------------------------------------------------------------------
Connection::~Connection () 
{
	mysql_close(&mysql);
}
//--------------------------------------------------------------------------
bool Connection::select_db (const char *db) 
{
  bool suc = !(mysql_select_db(&mysql, db));
  if (throw_exceptions && !suc) throw MysqlBadQuery(error());
  else return suc;
}
//--------------------------------------------------------------------------
bool Connection::reload() 
{
  bool suc = !mysql_reload(&mysql);
  if (throw_exceptions && !suc) throw MysqlBadQuery(error());
  else return suc;
}
//--------------------------------------------------------------------------
bool Connection::shutdown () 
{
  bool suc = !(mysql_shutdown(&mysql));
  if (throw_exceptions && !suc) throw MysqlBadQuery(error());
  else return suc;
}
//--------------------------------------------------------------------------
bool Connection::connect (cchar *db, cchar *host, cchar *user, cchar *passwd)
{
  locked = true;
  if (mysql_connect(&mysql, host, user, passwd)) {
    locked = false;
    Success = is_connected = true;
  } else {
    locked = false;
    if (throw_exceptions) throw BadQuery(error());
    Success = is_connected = false;
  }
  if (!Success) return Success;
  if (db[0]) // if db is not empty
    Success = select_db(db);
  return Success;
}
//--------------------------------------------------------------------------
string Connection::info ()
{
  char *i = mysql_info(&mysql);
  if (!i)
    return string();
  else
    return string(i);
}
//--------------------------------------------------------------------------
ResNSel Connection::execute(const string &str, bool throw_excptns) 
{
  Success = false;
  if (lock())
    if (throw_excptns) throw BadQuery(error());
    else return ResNSel();
  Success = !mysql_query(&mysql, str.c_str());
  unlock();
  if (!Success)
    if (throw_excptns) throw BadQuery(error());
    else return ResNSel();
  return ResNSel(this);
}
//--------------------------------------------------------------------------
bool Connection::exec(const string &str) 
{
	Success = !mysql_query(&mysql,str.c_str());
	if (!Success && throw_exceptions) throw BadQuery(error());
	return Success;
}
//--------------------------------------------------------------------------
Result Connection::store(const string &str, bool throw_excptns) 
{
  Success = false;
  if (lock())
    if (throw_excptns) throw BadQuery(error());
    else return Result();
  Success = !mysql_query(&mysql, str.c_str());
  unlock();
  if (!Success)
    if (throw_excptns) throw BadQuery(error());
    else return Result();
  return Result(mysql_store_result(&mysql));
}
//--------------------------------------------------------------------------
ResUse Connection::use(const string &str, bool throw_excptns) 
{
  Success = false;
  if (lock())
    if (throw_excptns) throw BadQuery(error());
    else return ResUse();
  Success = !mysql_query(&mysql, str.c_str());
  if (!Success)
    if (throw_excptns) throw BadQuery(error());
    else return ResUse();
  return ResUse(mysql_use_result(&mysql), this);
}
//end of file ===============================================================


