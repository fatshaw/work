#ifndef _BANK_
#define _BANK_
#include "Customer.h"
#include "List.h"
class Bank
{
private:
	unsigned int custmerCount;
	List<Customer> * customerList;
public:
	Bank():custmerCount(0){customerList = new List<Customer>();}
	~Bank(){if(customerList)delete customerList;}
	Customer *  CreateCustmer(const string & firstName, const string & lastName, const string & birthday);
};

#endif
