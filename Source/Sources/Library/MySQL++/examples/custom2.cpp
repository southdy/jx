
#include <iostream>
#include <vector>
#include <sqlplus.hh>
#include <custom.hh>
#include "util.hh"
// util.hh/cc contains the print_stock_table function

sql_create_5 (stock, 1, 5, string, item, int, num, 
	     double, weight, double, price, Date, sdate)

int main() {
  try { // its in one big try block

    Connection con(use_exceptions);
    con.connect("mysql_cpp_data");
    Query query = con.query();

    stock row;
    // create an empty stock object
    
    /*    row.item = "Hot Dogs";
    row.num = 100;
    row.weight = 1.5;
    row.price = 1.75;
    row.sdate = "1998-09-25"; */
    row.set("Hot Dogs", 100, 1.5, 1.75, "1998-09-25");
    // populate stock

    query.insert(row);
    // form the query to insert the row
    // the table name is the name of the struct by default
    cout << "Query : " << query.preview() << endl;
    // show the query about to be executed
    query.execute();
    // execute a query that does not return a result set

    print_stock_table(query);
    // now print the new table;
    
    return 0;
    
  } catch (BadQuery er) {
    cerr << "Error: " << er.error << endl;
    return -1;
  } catch (BadConversion er) { 
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \"" 
	 << er.type_name << "\"." << endl;
    return -1;
  }
}
