#include "Bank.h"


Customer *  Bank::CreateCustmer(const string & firstName, const string & lastName, const string & birthday)
{
	Customer * newCustomer = new Customer(firstName,lastName,birthday,custmerCount++);
	customerList->Append(newCustomer);
	return newCustomer;
}

