#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include "account.cpp"

using json = nlohmann::json;

enum class Vote { COMMIT, ABORT };
enum class Decision { COMMIT, ABORT };

class Bank{
    private:
        std::string name;
        std::map<std::string, Account> accounts;  
        std::vector<json> transfers;
    public:
        std::string viewAccounts();
        std::string viewTransfers();
};
