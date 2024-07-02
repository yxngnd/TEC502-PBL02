#include <iostream>
#include "account.cpp"

class Transaction{

    private:
        int id;
        std::vector<Account> accounts;
        bool done;
        bool success;
        int type;
};