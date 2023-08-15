#include <iostream>
#include <bits/stdc++.h>
#include <map>
#include <fstream>
using namespace std;

#define MIN_BALANCE 500
class InsufficientFunds{};

class Account{
private:
    long accountNumber;
    string firstName;
    string lastName;
    float balance;
    static long NextAccountNumber;
public:
    Account(){}
    Account(string fname, string lname, float balance);
    long getAccNo(){return accountNumber;}
    string getfirstName(){return firstName;}
    string getLastName(){return lastName;}
    float getBalance(){return balance;}

    void Deposit(float amount);
    void withdraw(float amount);
    static void setLastAccNo(long accountNumber);
    static long getLastAccNo();
    friend ofstream & operator<<(ofstream &save, Account &acc);
    friend ifstream & operator>>(ifstream &read, Account &acc);
    friend ostream & operator>>(ostream &out, Account &acc);
};

long Account::NextAccountNumber = 0;

class Bank{
private:
    map<long,Account> accounts;
public:
    Bank();
    Account OpenAccounts(string fname, string lname, float balance);
    Account  BalanceEnquiry(long accountnumber);
    Account  Deposit(long accountnumber, float amount);
    Account withdraw(long accountnumber, float amount);
    void CloseAccounts(long accountnumber);
    void ShowAllAccounts();
    ~Bank();
};

Account::Account(string fname, string lname, float balance){
    NextAccountNumber++;
    accountNumber = NextAccountNumber;
    firstName = fname;
    lastName = lname;
    this->balance = balance;
}
void Account::Deposit(float amount){
    balance += amount;
}
void Account::withdraw(float amount){
    if((balance-amount)< MIN_BALANCE) {
        throw InsufficientFunds();
    }else{
        balance -= amount;
    }
}
void Account::setLastAccNo(long accountNumber){
    NextAccountNumber= accountNumber;
}
long Account::getLastAccNo(){
    return NextAccountNumber;
}
ofstream & operator<<(ofstream &save, Account &acc){
    save<<acc.accountNumber<<endl;
    save<<acc.firstName<<endl;
    save<<acc.lastName<<endl;
    save<<acc.balance<<endl;
    return save;
}
ifstream & operator>>(ifstream &read, Account &acc){
    read>>acc.accountNumber;
    read>>acc.firstName;
    read>>acc.lastName;
    read>>acc.balance;
    return read;
}
ostream & operator<<(ostream &out, Account &acc){
    out<<"first Name: "<<acc.getfirstName()<<endl;
    out<<"Last Name: "<<acc.getLastName()<<endl;
    out<<"AccountNumber: "<<acc.getAccNo()<<endl;
    out<<"Balance: "<<acc.getBalance()<<endl;
    return out;
}

Bank::Bank(){
    Account account;
    ifstream read;
    read.open("Bank.data");
    if(!read){
        cout<<"Error in opening!"<<endl;
        cout<<"File not found!"<<endl;
        return;
    }
    while(!read.eof()){
        read>>account;
        accounts.insert(pair<long,Account>(account.getAccNo(),account));
    }
    Account::setLastAccNo(account.getAccNo());
    read.close();
}

Account Bank::OpenAccounts(string fname, string lname, float balance){
    ofstream save;
    Account account(fname,lname,balance);
    accounts.insert(pair<long,Account>(account.getAccNo(),account));
    save.open("Bank.data",ios::trunc);
    map<long,Account>::iterator itr;
    for(itr= accounts.begin();itr != accounts.end();itr++){
        save<<itr->second;
    }
    save.close();
    return account;
}
Account Bank::BalanceEnquiry(long accountnumber){
    map<long,Account>::iterator itr=accounts.find(accountnumber);
    return itr->second;
}
Account Bank::withdraw(long accountnumber, float amount){
    map<long,Account>::iterator itr = accounts.find(accountnumber);
    itr-> second.withdraw(amount);
    return itr->second;
}
Account Bank::Deposit(long accountnumber, float amount){
    map<long, Account>::iterator itr = accounts.find(accountnumber);
    itr->second.Deposit(amount);
    return itr->second;
}
void Bank::CloseAccounts(long accountnumber){
    map<long,Account>::iterator itr = accounts.find(accountnumber);
    cout<<"*******Account deleted*********"<<endl;
    cout<<itr->second;
    accounts.erase(accountnumber);
}
void Bank::ShowAllAccounts(){
    map<long,Account>::iterator itr;
    for(itr = accounts.begin();itr != accounts.end();itr++){
        cout<<itr->first<<endl<<itr->second<<endl;
    }    
}
Bank::~Bank(){
    ofstream read;
    read.open("Bank.data",ios::trunc);
    map<long,Account>::iterator itr;
    for(itr = accounts.begin();itr != accounts.end();itr++){
        read<<itr->second;
    }
    read.close();
}

int main(){
    Bank bk;
    Account acc;
    int choice;
    string FirstName, LastName;
    float Balance,amount;
    long accountNumber;
    cout<<"******************Banking System*******************"<<endl;
    do{
    cout<<"        "<<"Choose a option below"<<endl;
    cout<<"        "<<"(1) Open an Account "<<endl;
    cout<<"        "<<"(2) Balance enquiry "<<endl;
    cout<<"        "<<"(3) Deposit "<<endl;;
    cout<<"        "<<"(4) Withdrawal "<<endl;
    cout<<"        "<<"(5) Close an Accounts"<<endl;
    cout<<"        "<<"(6) Show all Accounts "<<endl;
    cout<<"        "<<"(7) Quit "<<endl;
    cout<<"Enter your choice: ";
    cin>>choice;
    switch(choice){
        case 1:
        cout<<"***********Account Creation***********"<<endl;
        cout<<"First Name: ";
        cin>>FirstName;
        cout<<"Last Name: ";
        cin>>LastName;
        cout<<"Initial Balance: ";
        cin>>Balance;
        acc = bk.OpenAccounts(FirstName,LastName,Balance);
        cout<<endl<<"           "<<"Congragulation"<<endl;
        cout<<"***********Account Created*************"<<endl;
        cout<<acc;
        break;
        case 2:
        cout<<"***********Balance Enquiry***********"<<endl;
        cout<<"Enter your account number: ";
        cin>>accountNumber;
        cout<<"Available Balance: "<<endl;
        acc = bk.BalanceEnquiry(accountNumber);
        cout<<acc;
        break;
        case 3:
        cout<<"***********Deposit***********"<<endl;
        cout<<"Enter your account number: ";
        cin>>accountNumber;
        cout<<"Enter the amount to deposit: ";
        cin>>amount;
        cout<<"***********Deposit Successfull***********"<<endl;
        cout<<"Available Balance: ";
        acc = bk.Deposit(accountNumber,amount);
        cout<<acc;
        break;
        case 4:
        cout<<"***********Withdrawal***********"<<endl;
        cout<<"Enter your account number: ";
        cin>>accountNumber;
        cout<<"Enter the amount to withdraw: ";
        cin>>amount;
        cout<<"***********Withdrawal Successfull***********"<<endl;
        cout<<"Available Balance: ";
        acc = bk.withdraw(accountNumber,amount);
        cout<<acc;
        break;
        case 5:
        cout<<"***********Close Account***********"<<endl;
        cout<<"Enter your account number: ";
        cin>>accountNumber;
        bk.CloseAccounts(accountNumber);
        cout<<"***********Account closed successfully***********"<<endl;
        cout<<acc;
        break;
        case 6:
        cout<<"***********Show all Account***********"<<endl;
        bk.ShowAllAccounts();
        break;
        case 7:
        break;
        default:
        cout<<"***********Enter the correct choice***********"<<endl;
        exit(0);
    }
    }while(choice != 7);
    return 0;
}


