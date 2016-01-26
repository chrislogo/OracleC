//============================================================================
// Name        : HardwoodSellerC.cpp
// Author      : Esteban Parra
// Version     : 1
// Copyright   : It's free
// Description : Main class for the Hardwood Seller
//============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include "WoodItem.cpp"

using namespace std;

const int NUM_INVENTORY = 6;    //items available to inventory
const int BASE0 = 1;            //the lower bounds before the eta has higher multiple
const int BASE1 = 101;
const int BASE2 = 201;
const int BASE3 = 301;
const int BASE4 = 401;
const int BASE5 = 501;
const int MULT0 = 1;            // multiples of delivery time taken
const int MULT1 = 2;
const int MULT2 = 3;
const int MULT3 = 4;
const int MULT4 = 5;
const double MULT5 = 5.5;
const int MAX_BF = 1000;

//Prototypes
string readInputFile(string);
void Split_Cust_Info(string&, string&, string&, string&);
double deliveryTime(double, double);
void Load_Inventory(WoodItem[]);
int Split_Order(string&, string&, double&);
double Calc_Total(double[], int);


int main() 
{
	string file_name;                          // cust file
	string cust;                               // cust name 
	string addr;                               // cust addr
	string date;                               // order date
	string text;                               // hold information yet to be parsed
    int order = 0;                             // keeps track of num items ordere
    int j = 0;                                 // index for totals
    double totals[NUM_INVENTORY];              // storage to later sum totals
	WoodItem inventory[NUM_INVENTORY];         // ds to hold inventory info         
    string ordered_wood[NUM_INVENTORY];        // assumed max size can only be max inventory size in largest case
    double ordered_quantity[NUM_INVENTORY];    // ""
    double total_price = 0.0;                  // total price of cust order
    int keep_going = 1;                        // control for gathering all items ordered by cust
    double del_eta = 0.0;                      // estimated delivery time
    double eta_arr[NUM_INVENTORY];             // stores all etas to  sum later
    double max = 0.0;


    // open inventory text file and load it 
	Load_Inventory(inventory);

    // init totals to 0 and eta_arr to 0
    for(int i = 0; i < NUM_INVENTORY; i++)
    {
        totals[i] = 0;
        eta_arr[i] = 0;
    }

    // prompt user for a file to check customer info
	cout << "Enter the file of the order you would like to view: ";
	cin >> file_name;
	cout << endl << endl;

    // load file
	text = readInputFile(file_name);
	
    // obtain cust info
    Split_Cust_Info(text, cust, addr, date);


    cout << "Name: " << cust << endl 
         << "Address: " << addr << endl
         << "Order Creation Date: " << date << endl
         << "Items:\tWood Ordered\t\t      Price" << endl
         << "------------------------------------------------" << endl;

    // obtain wood ordered
    // calculate individual wood totals
    // calculate delivery time of each piece of wood also
    while(keep_going)
    {
        keep_going = Split_Order(text, ordered_wood[order], ordered_quantity[order]);
        cout << "   " << setw(15)  << ordered_wood[order] << " ";

        for(int i = 0; i < NUM_INVENTORY; i++)
        {
            if(inventory[i].type == ordered_wood[order])
            {
                cout << setw(6) << inventory[i].price;
                totals[j] = inventory[i].price * ordered_quantity[order];
                eta_arr[j] = deliveryTime(inventory[i].baseDeliveryTime, ordered_quantity[order]);
                j++;
                break;
            }
        }

        cout << "  x  " << ordered_quantity[order] << "  =  $" << totals[j-1] << endl;
        order++;
    }

    // calc total price of order
    total_price = Calc_Total(totals, NUM_INVENTORY);

    // find the max eta, that's how long the order will take (in hours)
    for(int i = 0; i < NUM_INVENTORY; i++)
    {
        if(eta_arr[i] > max)
            max = eta_arr[i];
    }

    del_eta = max;

	cout << "------------------------------------------------" << endl
		 << "Estimated Delivery Time: " << del_eta << " hours" << endl
		 << "Total Price: $" << total_price << endl
		 << endl;
	
	return 0;
}


// Loads inventory from file into array
// based on number of items in inventory
void Load_Inventory(WoodItem inventory[])
{
	ifstream inv_file;
	char type[100];
    double *price = new double;
    double *hours = new double;
    size_t delim;
    size_t start = 0;

	
	inv_file.open("inventory.txt");

	streampos size;

	inv_file.seekg(0,inv_file.end);

    size = inv_file.tellg();

	inv_file.seekg(0, inv_file.beg);

    char *buffer = new char[size];

    inv_file.read(buffer, size);

    // copy to string
    string s(buffer);

    delete[] buffer;

    inv_file.close();

    for(int i = 0; i < NUM_INVENTORY; i++)
    {
    	sscanf(s.c_str(), "%[^:]:%lf:%lf", type, &inventory[i].price, &inventory[i].baseDeliveryTime);

    	inventory[i].type.assign(type);

    	delim = s.find_first_of('\n');

    	s = s.substr(delim+1, string::npos - delim);
    }
}

/*
 * Method to read the input file
 */
string readInputFile(string inputFilePath) 
{
	ifstream ledger;
	
	ledger.open(inputFilePath.c_str());

	string word;
    streampos size;

    if(!ledger)
    {
        cout << "ERROR: Cannot open input file: " << inputFilePath << endl
             << "Terminating..." << endl << endl;

        exit(1);
    }

    ledger.seekg(0,ledger.end);

    size = ledger.tellg();

    ledger.seekg(0, ledger.beg);

    char *buffer = new char[size];

    ledger.read(buffer, size);

    string s(buffer);

    delete[] buffer;

    ledger.close();

    return s;
}


// method to split up customer info
// parses out the customer's name, address, and order creation date
void Split_Cust_Info(string &text, string &cust, string &addr, string &date)
{
    size_t first_semi = 0;  
    char name[100];             // assuming name, addr, and date wont be longer
    char address[100];          // than 100 chars
    char shipment[100];


	sscanf(text.c_str(), "%[^;];%[^;];%[^\n]\n", name, address, shipment);

	// copy over read in values to strings to display customer info.
	string name_holder(name);
	cust = name_holder;

	string address_holder(address);
	addr = address_holder;

	string shipment_holder(shipment);
	date = shipment_holder;

	//skip the length of the cust name,
	//address, and date plus the delimiters
	first_semi = cust.length() + addr.length() + date.length() + 3;

    // remove this line to have orders next
	text = text.substr(first_semi, string::npos - first_semi);
}


// parses out one ordered item and the quantity
// then removes the item from the list
// returns 1 if there are more items to parsed
// returns 0 if the last item was parsed
int Split_Order(string &text, string &ordered_wood, double &ordered_quantity)
{
    char *name = new char[100];
    size_t first_semi;

    sscanf(text.c_str(), "%[^:]:%lf", name, &ordered_quantity);
   
    //copy over read in values to display customer info.
    ordered_wood.assign(name);

    //skip the length of the cust name,
    //address, and date plus the delimiters
    first_semi = text.find_first_of(";");

    text = text.substr(first_semi+1, string::npos - first_semi);

    // lets the program know if there are any orders left to parse
    // if not it will either be an empty string or a string that
    // contains a newline so it is assumed the length would be 
    // absolutely < 2 in those cases and otherwise the orders
    // should be entered in precisely on the text file and will 
    // obviously be > 2 characters in length
    if(text.length() < 2)
        return 0;
    else
        return 1;
}

/*
 * Method to compute the deliveryTime
 */
double deliveryTime(double base_time, double total_ordered) 
{
	double deliveryETA = 0.0;

    // Uses comparisons to figure out what the delivery time 
    // multiplier should be

    if(total_ordered == 0)
        deliveryETA = 0.0;
    else if(total_ordered >= BASE0 && total_ordered < BASE1)
        deliveryETA = MULT0 * base_time;
    else if(total_ordered >= BASE1  && total_ordered < BASE2)
        deliveryETA = MULT1 * base_time;
    else if(total_ordered >= BASE2 && total_ordered < BASE3)
        deliveryETA = MULT2 * base_time;
    else if(total_ordered >= BASE3 && total_ordered < BASE4)
        deliveryETA = MULT3 * base_time;
    else if(total_ordered >= BASE4 && total_ordered < BASE5)
        deliveryETA = MULT4 * base_time;
    else if(total_ordered >= BASE5 && total_ordered <= MAX_BF)
        deliveryETA = MULT5 * base_time;
	
	return deliveryETA;
}


// compute total price
// sums the total the cust ordered
// anything from inventory not ordered for the cust will be 0
double Calc_Total(double totals[], int NUM_INVENTORY)
{
    double sum = 0.0;

    for(int i = 0; i < NUM_INVENTORY; i++)
    {
        sum += totals[i];
    }

    return sum;
}