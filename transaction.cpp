#include <iostream>
#include "account.cpp"
#include <map>

class Transaction{

    public:
        int id;
        std::vector<Account> accounts;
        std::map<Account, double> change;
        bool prepared;
        bool done;
        bool success;
        int type;
};