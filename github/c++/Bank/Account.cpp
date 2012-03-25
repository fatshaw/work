#include "Account.h"

unsigned long Account::accountCount  = 0;

ostream & operator<<(ostream & out,Account * account)
{
	out<<account->accountType<<" INTEREST RATE:"<<account->interestRate<<"%"<<endl;
	out<<"MAXIMUM BALANCE :"<<account->maxBalance<<" MINIMUM BALANCE :"<<account->minBalance<<" CURRENT BALANCE :"<<account->currentBalance<<endl;
	account->DisplayStatement();
	return out;
}

int Account::Deposit(double amount)
{
	if(this->currentBalance + amount > this->maxBalance)return FAILURE;
	this->currentBalance+=amount;
	return SUCCESS;
}

int Account::WithDrawl(double amount)
{
	if(this->currentBalance - this->minBalance < amount)
	{
		return -1;
		cout<<"You cannot withdrawl money with amount of "<<amount<<"the currentBalance will be less than the minBalance of your account"<<endl;
		return FAILURE;
	}

	this->currentBalance -= amount;
	return SUCCESS;
}

void Account::DisplayStatement()
{
	cout<<"STATEMENT"<<endl;
	List<char>::Iterator it = statement.begin();
	for(;it!=statement.end();it++)
	{
		char * operation = *it;
		cout<<operation<<endl;
	}
}