
#include <iostream>
#include <iomanip>
#include <vector>
#include <sqlplus.hh>
#include <custom.hh>

sql_create_5 (stock,		// struct name, 
	      1, 5,		// I'll explain these latter
	      string, item,	// type, id
	      int, num,
	      double, weight,
	      double, price,
	      Date, sdate)

// this is calling a very complex macro which will create a custom
// struct "stock" which has the variables:
//   string item
//    int num
//    ...
//    Date sdate
// defined as well methods to help populate the class from a mysql row
// among other things that I'll get too in a latter example

int main () {
  try {				// its in one big try block
    Connection con (use_exceptions);
    con.connect ("mysql_cpp_data");
    Query query = con.query ();
    query << "select * from stock";

    vector < stock > res;
    query.storein (res);
    // this is storing the results into a vector of the custom struct
    // "stock" which was created my the macro above.

    cout.setf (ios::left);
    cout << setw (17) << "Item"
	 << setw (4) << "Num"
	 << setw (7) << "Weight"
	 << setw (7) << "Price"
	 << "Date" << endl
	 << endl;

    // Now we we iterate through the vector using an iterator and
    // produce output similar to that using Row
    // Notice how we call the actual variables in i and not an index
    // offset.  This is because the macro at the begging of the file
    // set up an *actual* struct of type stock which contains the 
    // variables item, num, weight, price, and data.

    cout.precision(3);
    vector <stock>::iterator i;
    for (i = res.begin (); i != res.end (); i++) {
      cout << setw (17) << i->item.c_str ()
	// unfortunally the gnu string class does not respond to format
	// modifers so I have to convert it to a conat char *.
	   << setw (4) << i->num
	   << setw (7) << i->weight
	   << setw (7) << i->price
	   << i->sdate
	   << endl;
    }
    return 0;
    
  } catch (BadQuery er){ // handle any connection 
                         // or query errors that may come up
    cerr << "Error: " << er.error << endl;
    return -1;

  } catch (BadConversion er) {
    // we still need to cache bad conversions incase something goes 
    // wrong when the data is converted into stock
    cerr << "Error: Tried to convert \"" << er.data << "\" to a \""
	 << er.type_name << "\"." << endl;
    return -1;
  }
}
