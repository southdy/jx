#include <windows.h>
#include <iostream>
#include <sqlplus.hh>

int main (int argc, char *argv[]) {
  Connection connection(use_exceptions);
  try { // the entire main block is one big try block;

    if (argc == 1) connection.connect("");
    else if (argc == 2) connection.connect("",argv[1]);
    else if (argc == 3) connection.connect("",argv[1],argv[2]);
    else if (argc <= 4) connection.connect("",argv[1],argv[2],argv[3]);
    // create a new object and connect based on any (if any) arguments
    // passed to main();
    
    try {
      connection.select_db("mysql_cpp_data");
    } catch (BadQuery er) {
      // if it couldn't connect to the database assume that it doesn't exist
      // and try created it.  If that does not work exit with an error.
      connection.create_db("mysql_cpp_data");
      connection.select_db("mysql_cpp_data");
    }
    
    Query query = connection.query();  // create a new query object
    
    try { // ignore any errors here
          // I hope to make this simpler soon
      query.execute("drop table stock");
    } catch (BadQuery er) {}
    
    query << "create table stock  (item char(20) not null, num bigint,"
	  << "weight double, price double, sdate date)";
    query.execute(RESET_QUERY);
    // send the query to create the table and execute it.  The
    // RESET_QUERY tells the query object to reset it self after
    // execution
    
    query << "insert into %5:table values (%0q, %1q, %2, %3, %4q)";
    query.parse();
    // set up the template query I will use to insert the data.  The
    // parse method call is important as it is what lets the query
    // know that this is a template and not a literal string
    
    query.def["table"] = "stock";
    // This is setting the parameter named table to stock.
    
    query.execute ("Hamburger Buns", 56, 1.25, 1.1, "1998-04-26");
    query.execute ("Hotdogs' Buns"   ,65, 1.1 , 1.1, "1998-04-23");
    query.execute ("Dinner Roles"  , 75,  .95, .97, "1998-05-25");
    query.execute ("White Bread"   , 87, 1.5, 1.75, "1998-09-04");
    // The last parameter "table" is not specified here.  Thus
    // the default value for "table" is used which is "stock".

  } catch (BadQuery er) { // handle any errors that may come up
    cerr << "Error: " << er.error << endl;
    return -1;
  }
}




















