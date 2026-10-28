#include <bits/stdc++.h>
using namespace std;

enum class TransactionType {
    WITHDRAWAL,
    DEPOSIT
};

struct Transaction {
    int amount;
    TransactionType type;
    int balanceAfter;
};

class TransactionHistory{
    private:
      vector<Transaction> history;

    public: 

        void addTransaction(int amount, TransactionType type, int balanceAfter) {
            history.push_back({amount, type, balanceAfter});
      }

      vector<Transaction> getHistory() const {
    return history;
}



};

class Account {
    private:
        int PIN;
        int Balance;
        int counter = 0;
        int dailLimit;
        int moneydebited = 0;
        TransactionHistory t;
protected:
        virtual int minimumAllowedBalance() const {
            return 0;
        }

    public:

        Account(int pin, int balance, int dailyLimit){
            PIN = pin;
            Balance = balance;
            dailLimit = dailyLimit;

        }

        


        bool isBlocked() const { return counter >= 3; }

        bool verifyPin(int pin) {
            if (isBlocked()) return false;   
            if (pin != PIN) {
                counter++;
                return false;
            }
            counter = 0;                     
            return true;
        }

        bool debit(int amount){
            if (amount <= 0) return false;
            if (Balance - amount < minimumAllowedBalance()) return false;
            if(moneydebited + amount > dailLimit) return false;
                Balance -= amount;
                moneydebited += amount;
                t.addTransaction(amount, TransactionType::WITHDRAWAL, Balance);
                return true;
        
        }

        bool depositamount(int amount){
            if (amount <= 0) return false;
            Balance += amount;
            t.addTransaction(amount, TransactionType::DEPOSIT, Balance);
            return true;
        }

        int getBalance(){
            return Balance;
        }

        virtual ~Account() {}
        
        void startNewDay() {
            moneydebited = 0;
        }


        vector<Transaction> transactions (){
            return  t.getHistory();
        }
};

class OverdraftAccount : public Account {
    private:
        int overdraftLimit;

    protected:
        int minimumAllowedBalance() const override {
            return -overdraftLimit;        
        }

    public:
        OverdraftAccount(int pin, int balance, int dailyLimit, int limit)
            : Account(pin, balance, dailyLimit), overdraftLimit(limit) {}
};




class ATMmachine{
    private:
        int cash;

    public:
        ATMmachine(int c){
            cash = c;
        }


        bool isPinValid(int PIN,  Account& a){
                return a.verifyPin(PIN);         
        
        }

        bool withdraw(int pin, int amount, Account& a){
            if (!a.verifyPin(pin)) return false;               // PIN first
            if (amount <= 0 || amount % 100 != 0) return false;
            if (amount > cash) return false;
            if(a.debit(amount)){ 
                cash -= amount;
                return true;
            }
            return false;
        }

        bool deposit(int pin, int amount, Account& a){
            if (!a.verifyPin(pin)) return false;               // PIN first
            if(a.depositamount(amount)){ 
                cash += amount;
                return true;
            }
            return false;
        }

        int getCash(){
            return cash;
        }
        
};