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

const int NUM_INVENTORY = 6;
const int BASE0 = 1;
const int BASE1 = 101;
const int BASE2 = 201;
const int BASE3 = 301;
const int BASE4 = 401;
const int BASE5 = 501;
const int MULT0 = 1;
const int MULT1 = 2;
const int MULT2 = 3;
const int MULT3 = 4;
const int MULT4 = 5;
const int MULT5 = 5.5;

//Prototypes
string readInputFile(string);
void Split_Cust_Info(string&, string&, string&, string&);
double deliveryTime();
void Load_Inventory(WoodItem[]);
int Split_Order(string&, string&, int&);

double Calc_Total(int[], int);


int main() 
{
	string file_name;
	string cust;
	string addr;
	string date;
	string text;
    int order = 0;
    int j = 0;
    int totals[NUM_INVENTORY];
	WoodItem inventory[NUM_INVENTORY];
    string ordered_wood[NUM_INVENTORY];         // assumed max size can only be max inventory size in largest case
    int ordered_quantity[NUM_INVENTORY];        // ""
    double total_price;
    int keep_going = 1;


    // open inventory text file and load it 
	Load_Inventory(inventory);

    for(int i = 0; i < NUM_INVENTORY; i++)
    {
        totals[i] = 0;
    }

    // promt user for a file to check customer info
	cout << "Enter the file of the order you would like to view: ";
	cin >> file_name;
	cout << endl << endl;

    // load file
	text = readInputFile(file_name);
	
    // obtain cust info
    Split_Cust_Info(text, cust, addr, date);

    cout << "Name: " << cust << endl 
         << "Address: " << addr << endl
         << "Items: Wood Ordered\t\tPrice" << endl;

    // obtain wood ordered
    while(keep_going)
    {
        keep_going = Split_Order(text, ordered_wood[order], ordered_quantity[order]);
        cout << "\t" << setw(10)  << ordered_wood[order] << "  x   " << ordered_quantity[order] << "  =  $";

        for(int i = 0; i < NUM_INVENTORY; i++)
        {
            if(inventory[i].type == ordered_wood[order])
            {
                totals[j] = inventory[i].price * ordered_quantity[order];
                j++;
                break;
            }
        }

        cout << totals[j-1] << endl;
        order++;
        //break;
    }

    total_price = Calc_Total(totals, NUM_INVENTORY);

	cout << "-------------------------------------" << endl
		 << "Estimated Delivery Time: " << date << endl
		 << "Total Price: $" << total_price << endl
		 << endl;
	
	return 0;
}


// Loads inventory into array
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

    string s(buffer);

    delete[] buffer;

    inv_file.close();

    for(int i = 0; i < NUM_INVENTORY; i++)
    {
    	sscanf(s.c_str(), "%[^:]:%lf:%lf", type, &inventory[i].price, &inventory[i].baseDeliveryTime);

    	inventory[i].type.assign(type);
    	//inventory[i].price = price;
    	//inventory[i].baseDeliveryTime = hours;

    	delim = s.find_first_of('\n');

    	s = s.substr(delim+1, string::npos - delim);

    	//cout << inventory[i].type <<  " " << inventory[i].price << " " << inventory[i].baseDeliveryTime << endl;
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
void Split_Cust_Info(string &text, string &cust, string &addr, string &date)
{
	size_t last_semi = 0;
    size_t first_semi = 0;
    string temp;
    char name[100];
    char address[100];
    char shippment[100];


	sscanf(text.c_str(), "%[^;];%[^;];%[^\n]\n", name, address, shippment);

	//copy over read in values to display customer info.
	string name_holder(name);
	cust = name_holder;

	string address_holder(address);
	addr = address_holder;

	string shippment_holder(shippment);
	date = shippment_holder;

	//skip the length of the cust name,
	//address, and date plus the delimiters
	first_semi = cust.length() + addr.length() + date.length() + 3;

	text = text.substr(first_semi, string::npos - first_semi);

	//cout << text << endl << endl;
}


int Split_Order(string &text, string &ordered_wood, int &ordered_quantity)
{
    char *name = new char[100];
    size_t first_semi;

    sscanf(text.c_str(), "%[^:]:%d", name, &ordered_quantity);
   
    //copy over read in values to display customer info.
    ordered_wood.assign(name);

    //skip the length of the cust name,
    //address, and date plus the delimiters
    first_semi = text.find_first_of(";");

    text = text.substr(first_semi+1, string::npos - first_semi);

   // cout << text << endl << endl;

    if(text.length() < 2)
        return 0;
    else
        return 1;
}

/*
 * Method to compute the deliveryTime
 */
double deliveryTime() 
{
	double deliveryETA = 0.0;
	
	return deliveryETA;
}


//compute total price
double Calc_Total(int totals[], int NUM_INVENTORY)
{
    double sum = 0.0;

    for(int i = 0; i < NUM_INVENTORY; i++)
    {
        sum += totals[i];
    }

    return sum;
}