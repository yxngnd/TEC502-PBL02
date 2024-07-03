#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include "account.cpp"

using json = nlohmann::json;

class Bank{
    private:
        std::mutex mtx;
        std::multimap<std::string, Account> accounts;
    public:
        std::string viewAccounts();
        std::string viewTransfers();

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

            // Check if there is already an account with the same CPF
            auto range = accounts.equal_range(cpf);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second.getCpf() == cpf && it->second.getType() == type) {
                    return false; // Account with the same CPF already exists
                }
            }

            accounts.insert({cpf, Account(name, cpf, type, password, 0)});
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
            auto range = accounts.equal_range(cpf);
            json result = json::array();
            for (auto it = range.first; it != range.second; ++it) {
                json account_json;
                to_json(account_json, it->second);
                result.push_back(account_json);
            }
            return result;
        }

        void to_json(json& j, Account& account) {
            j = json{
                {"name", account.getName()},
                {"cpf", account.getCpf()},
                {"type", account.getType()},
                {"balance", account.getBalance()},
            };
        }

        bool deposit(const std::string& cpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << cpf << " " << value << std::endl;
            auto it = accounts.find(cpf);
            if(it != accounts.end()){
                it->second.addBalance(value);
                return true;
            }
            return false;
        }

        bool withdraw(const std::string& cpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << cpf << " " << value << std::endl;
            auto it = accounts.find(cpf);
            if(it != accounts.end() && it->second.getBalance() >= value){
                it->second.subBalance(value);
                return true;
            }
            return false;
        }

        bool prepareTransaction(const std::string& senderCpf, const std::string& receiverCpf, double value) {
            std::lock_guard<std::mutex> lock(mtx);
            auto itSender = accounts.find(senderCpf);
            auto itReceiver = accounts.find(receiverCpf);
            if (itSender != accounts.end() && itReceiver != accounts.end() && itSender->second.getBalance() >= value) {
                // Prepare phase: Tentatively decrement balance of Sender and increment balance of Receiver
                itSender->second.subBalance(value);
                itReceiver->second.addBalance(value);
                return true;
            }
            return false;
        }

        bool confirmTransaction(const std::string& senderCpf, const std::string& receiverCpf, double value) {
            std::lock_guard<std::mutex> lock(mtx);
            auto itSender = accounts.find(senderCpf);
            auto itReceiver = accounts.find(receiverCpf);
            if (itSender != accounts.end() && itReceiver != accounts.end()) {
                // Commit phase: The tentative changes are already applied, so just confirm them
                return true;
            }
            return false;
        }

        bool cancelTransaction(const std::string& senderCpf, const std::string& receiverCpf, double value) {
            std::lock_guard<std::mutex> lock(mtx);
            auto itSender = accounts.find(senderCpf);
            auto itReceiver = accounts.find(receiverCpf);
            if (itSender != accounts.end() && itReceiver != accounts.end()) {
                // Abort phase: Rollback the tentative changes
                itSender->second.subBalance(value);
                itReceiver->second.addBalance(value);
                return true;
            }
            return false;
        }

        bool transfer(const std::string& senderCpf, const std::string& receiverCpf, double value) {
            if (prepareTransaction(senderCpf, receiverCpf, value)) {
                if (confirmTransaction(senderCpf, receiverCpf, value)) {
                    return true;
                } else {
                    cancelTransaction(senderCpf, receiverCpf, value);
                }
            }
            return false;
        }
        

};
