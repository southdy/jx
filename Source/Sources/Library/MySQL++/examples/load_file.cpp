#include <windows.h>
#include <sys/stat.h>
#include <fstream>
#include <sqlplus.hh>
extern int errno;
const char  MY_DATABASE[]="telcent";
const char  MY_TABLE[]="fax";
const char  MY_HOST[]="localhost";
const char  MY_USER[]="root";
const char  MY_PASSWORD[]="";
const char  MY_FIELD[]="fax"; // BLOB field

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "Usage : load_file full_file_path" << endl << endl;
		return -1;
	}
  Connection con(use_exceptions);
	try {
		con.real_connect (MY_DATABASE,MY_HOST,MY_USER,MY_PASSWORD,3306,(int)0,60,NULL);
		Query query = con.query(); ostrstream strbuf;
		ifstream In (argv[1],ios::in | ios::binary); struct stat for_len;
		if ((In.rdbuf())->is_open()) {
			if (stat (argv[1],&for_len) == -1) return -1;
			unsigned int blen = for_len.st_size;  if (!blen) return -1;
			char  *read_buffer = new char[blen];	In.read(read_buffer,blen); string fill(read_buffer,blen);
			strbuf  << "INSERT INTO " << MY_TABLE << " (" << MY_FIELD << ") VALUES(\""  << escape << fill  << "\")";
      query.exec(strbuf.str());
			delete[] read_buffer;
		}
		else 
			cerr << "Your binary file " << argv[1] << "could not be open, errno = " << errno;
		return 0;
  } catch (BadQuery er) { 
    cerr << "Error: " << er.error << " " << con.errnum() << endl;
    return -1;
	}
}
