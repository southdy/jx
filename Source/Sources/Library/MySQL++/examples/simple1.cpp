#include <iostream>
#include <iomanip>
#include <mysql++>

int main() {
  // The full format for the Connection constructor is
  // Connection(cchar *db, cchar *host="", 
  //            cchar *user="", cchar *passwd="") 
  // You may need to specify some of them if the database is not on
  // the local machine or you database username is not the same as your
  // login name, etc..
  try {
		Connection con("mysql_cpp_data");
		Query query = con.query();
		// This creates a query object that is bound to con.

		query << "select * from stock";
		// You can write to the query object like you would any other ostrem

		Result res = query.store();
		// Query::store() executes the query and returns the results

		cout << "Query: " << query.preview() << endl;
		// Query::preview() simply returns a string with the current query
		// string in it.

		cout << "Records Found: " << res.size() << endl << endl;
  
		Row row;
		cout.setf(ios::left);
		cout << setw(17) << "Item" 
			<< setw(4)  << "Num"
			<< setw(7)  << "Weight"
			<< setw(7)  << "Price" 
			<< "Date" << endl
			<< endl;
  
		Result::iterator i;
		// The Result class has a read-only Random Access Iterator
		for (i = res.begin(); i != res.end(); i++) {
			row = *i;
			cout << setw(17) << row[0] 
				<< setw(4)  << row[1] 
				<< setw(7)  << row["weight"]
				// you can use either the index number or column name when
				// retrieving the colume data as demonstrated above.
				<< setw(7)  << row[3]
				<< row[4] << endl;
		}
  } catch (BadQuery er){ // handle any connection 
                         // or query errors that may come up
    cerr << "Error: " << er.error <<  endl;
    return -1;

  } catch (BadConversion er) {
    // we still need to cache bad conversions incase something goes 
    // wrong when the data is converted into stock
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
	 << er.type_name << "\"." << endl;
    return -1;
  }
	return 0;
}
