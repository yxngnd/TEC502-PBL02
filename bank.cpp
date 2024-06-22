#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include "account.cpp"

using json = nlohmann::json;

class Bank{
    private:
        std::string name;
        std::map<std::string, Account> accounts;  
        std::vector<json> transfers;
    public:
        std::string viewAccounts();
        std::string viewTransfers();
};
