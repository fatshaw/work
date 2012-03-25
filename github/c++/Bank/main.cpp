#include <iostream>
#include "Bank.h"
using namespace std;

char currentTime[20];

int main()
{
	Bank * bank = new Bank();
	Customer * customer = bank->CreateCustmer("Json","Whright","2010-10-10");
	cout<<"CUSTOMER"<<endl;
	cout<<customer<<endl;
	unsigned long accountId1 = customer->OpenAccount(ACOUNT_TYPE_CHEQUE,1000);
	unsigned long accountId2 = customer->OpenAccount(ACOUNT_TYPE_CHEQUE,1000);
	customer->Deposit(accountId1,10000);
	customer->WithDrawl(accountId1,1000);
	customer->Transfer(1000,accountId1,accountId2);
	customer->DisplayStatement(accountId1);
	delete bank;
	system("pause");
}