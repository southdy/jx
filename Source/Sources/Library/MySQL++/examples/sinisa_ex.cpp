#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sqlplus.hh>

int main() {
  vector<string> yy;

  Connection con(use_exceptions);
	try {
	con.real_connect ("","localhost","root","",3306,(int)0,60,NULL);
					
	cout << con.clinet_info() << endl << endl;
  Query query = con.query();

  query << "show databases";

  Result res = query.store();

  //cout << "Query: " << query.preview() << endl;

  cout << "Records Found: " << res.size() << endl << endl;
  
  Row row;
  cout.setf(ios::left);
  cout << setw(17) << "Databases"  << endl << endl;
  
  Result::iterator i;
  for (i = res.begin(); i != res.end(); i++) {
    row = *i;
    cout << setw(17) << row[0]  << endl;
  }
	char database [] = "mysql";
  con.select_db (database);

  query << "show tables";

  res = query.store();

  //cout << "Query: " << query.preview() << endl;

  cout << "Records Found: " << res.size() << endl << endl;
  
  cout.setf(ios::left);
  cout << setw(17) << "Tables"  << endl << endl;
			
  for (i = res.begin(); i != res.end(); i++) {
    row = *i;
		string xx(row[0]);
    cout << setw(17) << row[0]  << endl;
		yy.insert(yy.end(),xx);
  }
	for (unsigned int j = 0; j < yy.size();j++) {
		query << "describe " << yy[j] << "";
		cout << query.preview() << endl << endl;
		res = query.store();
		unsigned int columns = res.num_fields(), counter;
		cout << setw(15) << endl;
		for (counter = 0; counter < columns; counter++) {
			cout << setw(15) << res.names(counter) << "  ";
		}
		cout << endl << endl;
		for (i = res.begin(); i!=res.end();i++) {
			row = *i;
			for (counter = 0; counter < columns; counter++)  {
				cout << row[counter] << "  ";
			}
			cout << endl;
		}
	}
		query << "select * from user";
		res  =  query.store(); int columns =  res.num_fields();
		cout << query.preview() << endl << endl;
		cout << "fields = " << res.num_fields() << "rows = " << res.size() << endl;
		volatile MYSQL_RES *ress= res.mysql_result(); 
		if (!ress) return -1;
		for (i = res.begin(); i!=res.end();i++) {
			row = *i;
			for (int counter = 0; counter < columns; counter++)  {
				cout << row[counter] << "  ";
			}
			cout << endl;
		}
  } catch (BadQuery er){ // handle any connection 
                         // or query errors that may come up
    cerr << "Error: " << er.error << " " << con.errnum() << endl;
    return -1;

  } catch (BadConversion er) {
    // we still need to cache bad conversions incase something goes 
    // wrong when the data is converted into stock
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
	 << er.type_name << "\"." << endl;
    return -1;
  }

}

