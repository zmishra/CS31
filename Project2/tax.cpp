#include <iostream>
#include <string>
using namespace std;

int main()
{
	const double TAX_RATE_200 = .041; //All constants that could be used
	const double TAX_RATE_500 = .053;
	const double TAX_RATE_500NYNJ = .057;
	const double TAX_RATE_OVER500 = .06;

	double marketValue; //All variables declared
	string id;
	string state;
	double tax;
	double tr500; //Only tax rate that  could differ between inputs
	
	cout << "Market value (in millions): ";
	cin >> marketValue;
	cin.ignore(10000, '\n'); //Gets rid of the newline character left over
	cout << "Identification: ";
	getline(cin, id);
	cout << "State: ";
	getline(cin, state);
	cout << "---" << endl;

	if (state == "NY" || state == "NJ")	//Determines which tax rate to use based on state
		tr500 = TAX_RATE_500NYNJ;
	else
		tr500 = TAX_RATE_500;

	if (marketValue <= 200)				  //If market value is 200 million or less,
		tax = TAX_RATE_200 * marketValue; //the following formula is used
	else if (marketValue <= 500)								
		tax = TAX_RATE_200 * 200 + tr500*(marketValue-200);	//The next tax rate  only applies for the market value above 200 million
	else
		tax = TAX_RATE_200 * 200 + tr500 * 300 + TAX_RATE_OVER500 * (marketValue-500); //The next tax rate only applies to the market value above 500 million 

	cout.setf(ios::fixed); //Set the output to cut off at 3 decimal places
	cout.precision(3);

	if (marketValue < 0)
		cout << "The market value must be nonnegative." << endl; //Error message if market value entered is negative
	else if (id == "")
		cout << "You must enter a property identification." << endl; //Error message if nothing is entered for identification
	else if (state == "")
		cout << "You must enter a state." << endl; //Error message if nothing is entered for state
	else
		cout << "The tax for " << id << " is $" << tax << " million." << endl; //Message if there are no errors
}