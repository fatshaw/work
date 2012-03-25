#ifndef _ACCOUNT_
#define _ACCOUNT_
#include "Define.h"
#include "List.h"
#include <iostream>
using namespace std;

class Account
{
private:
	static unsigned long accountCount;
private:
	unsigned long custmerId;
	unsigned long accountId;
	char *  accountType;
	double interestRate;
	double currentBalance;
	double maxBalance;
	double minBalance;
	List<char>statement;
private:
	void SetAccountCharacteristic(int accountType);
public:
	Account(char *  accountType,double currentBalance,unsigned long custmerId,double maxBalance = 100000,double minBalance  = 300,double interestRate = 1.25)
		:accountType(accountType),currentBalance(currentBalance),custmerId(custmerId),maxBalance(maxBalance),minBalance(minBalance),interestRate(interestRate){accountId = accountCount++;}
	~Account(){}
	friend ostream & operator<<(ostream & out,Account * account);
	unsigned long GetAccountId(){return accountId;};
	int Deposit(double amount);
	int WithDrawl(double amount);
	void AddStatement(char * operation)
	{
		if(statement.Size()>ACCOUNT_STATEMENT_MAXCOUNT)
			statement.PopFront();
		statement.Append(operation);
	}
	unsigned long GetId(){return accountId;};
	void DisplayStatement();
	char * GetAccountType(){return accountType;}
};

ostream & operator<<(ostream & out,const Account * account);

#endif