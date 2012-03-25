#ifndef _CUSTOMER_
#define _CUSTOMER_
#include "Account.h"
#include "List.h"
#include <string>
using namespace std;

class Customer
{
private:
	const string firstName;
	const string lastName;
	const string birthday;
	List<Account> * accountList;
	unsigned long custmerId;
private:
	Account * FindAccount(unsigned long accountId);
public:
	friend 	ostream & operator<<(ostream & out,const Customer * customer);
	Customer(const string & firstName, const string & lastName,const string & birthday,unsigned long custmerId):firstName(firstName),lastName(lastName),birthday(birthday),custmerId(custmerId)
	{
		accountList = new List<Account>();
	}
	~Customer(){if(accountList)delete accountList;}
	unsigned long OpenAccount(char *  accountType,double initialAmount);
	int Deposit(unsigned long accountId,double amount);
	int WithDrawl(unsigned long accountId,double amount);
	int Transfer(double amount,unsigned long fromAccountId,unsigned long toAccountId);
	int GetId(){return custmerId;}
	void DisplayStatement(unsigned long accountId);
};

ostream & operator<<(ostream & out,const Customer * customer);

#endif