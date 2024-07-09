#include <iostream>
#include <map>
#include "nlohmann/json.hpp"
#include <vector>
#include "account.cpp"

using json = nlohmann::json;

class Bank{
    private:
        std::mutex mtx;
        std::map<std::string, Account> accounts;
    public:
        //std::string viewAccounts();

        bool login(const std::string& cpf, const std::string& password, Account& account) {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = accounts.find(cpf);
            if (it != accounts.end() && it->second.getPassword() == password) {
                account = it->second;
                return true;
            }
            return false;
        }

        bool registerAccount(const std::string& name, const std::string& cpf, bool type, const std::string& password) {
            std::lock_guard<std::mutex> lock(mtx);
            
            auto it = accounts.find(cpf);
            if (it != accounts.end()) {
                // Se a conta já existe e o tipo não permite duplicatas, retorne false
                    return false;
            }
            
            // Insira a nova conta ou atualize a existente
            accounts[cpf] = Account(name, cpf, type, password, 0);
            return true;
        }

        bool getAccount(std::string& cpf, Account& account){
            std::lock_guard<std::mutex> lock(mtx);
            auto it = accounts.find(cpf);
            if (it != accounts.end()) {
                account = it->second;
                return true;
            }
            return false;
        }

        json getAccountsByCpf(const std::string& cpf) {
            std::lock_guard<std::mutex> lock(mtx);
            json result = json::array();

            for (auto& pair : accounts) {
                if (pair.first.find(cpf) != std::string::npos) {
                    json accountJson;
                    toJson(accountJson, pair.second);
                    result.push_back(accountJson);
                }
            }

            return result;
        }

        void toJson(json& j, Account& account) {
            j = json{
                {"name", account.getName()},
                {"cpf", account.getCpf()},
                {"type", account.getType()},
                {"balance", account.getBalance()},
            };
        }

        bool deposit(const std::string& cpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            auto it = accounts.find(cpf);
            if(it != accounts.end()){
                it->second.addBalance(value);
                return true;
            }
            return false;
        }

        bool withdraw(const std::string& cpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            auto it = accounts.find(cpf);
            if(it != accounts.end() && it->second.getBalance() >= value){
                it->second.subBalance(value);
                return true;
            }   
            return false;
        }    
};
