#include "Customer.h"

ostream & operator<<(ostream & out,const Customer * customer)
{
	out<<customer->firstName<<" "<<customer->lastName<<" "<<customer->birthday;
	return out;
}

unsigned long Customer::OpenAccount(char * accountType,double initialAmount)
{
	char * statement = new char[100];
	Account * account = new Account(accountType,initialAmount,custmerId);
	accountList->Append(account);
	sprintf(statement,"%s -- account opening %f",GetCurrentTime(),initialAmount);
	account->AddStatement(statement);
	return account->GetAccountId();
}

Account * Customer::FindAccount(unsigned long accountId)
{
	return accountList->Find(accountId);
}

int Customer::Deposit(unsigned long accountId,double amount)
{
	Account * account = FindAccount(accountId);
	if(account == NULL)
	{
		return FAILURE;
	}
	if(account->Deposit(amount) == FAILURE)
	{
		return FAILURE;
	}
	char * statement = new char[100];
	sprintf(statement,"%s -- deposit %f",GetCurrentTime(),amount);
	account->AddStatement(statement);
	return SUCCESS;
}

int Customer::WithDrawl(unsigned long accountId,double amount)
{
	Account * account = FindAccount(accountId);
	if(account == NULL)
	{
		return FAILURE;
	}
	if(account->WithDrawl(amount) == FAILURE)
	{
		return FAILURE;
	}
	char * statement = new char[100];
	sprintf(statement,"%s -- withdrawl %f",GetCurrentTime(),amount);
	account->AddStatement(statement);
	return SUCCESS;
}

int Customer::Transfer(double amount,unsigned long fromAccountId,unsigned long toAccountId)
{
	Account * fromAccount = NULL;
	Account * toAccount = NULL;
	if((fromAccount = FindAccount(fromAccountId)) == NULL || (toAccount = FindAccount(toAccountId)) == NULL )
		return FAILURE;
	if(fromAccount->WithDrawl(amount)==FAILURE)return FAILURE;
	if(toAccount->Deposit(amount)==FAILURE)
	{
		fromAccount->Deposit(amount);
		return FAILURE;
	}
	char * statement  =  new char[100];
	sprintf(statement,"%s -- transfer %f to %s",GetCurrentTime(),amount,toAccount->GetAccountType());
	cout<<"statement"<<statement<<endl;
	fromAccount->AddStatement(statement);
	return SUCCESS;
	
}

void Customer::DisplayStatement(unsigned long accountId)
{
	Account * account = FindAccount(accountId);
	if(account == NULL)return;
	cout<<account<<endl;
}