#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include "account.cpp"

std::unordered_map<std::string, Account> accounts;

class Bank{
    private:
        std::string host;
        std::string port;
        std::string name;
        std::mutex mtx;
        std::map<std::string, Account> accounts;
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

        bool registerAccount(const Person& person, const std::string& agency, const std::string& password) {
            std::lock_guard<std::mutex> lock(mtx);
            if (accounts.find(person.getCpf()) != accounts.end()) {
                return false;
            }
            accounts[person.getCpf()] = Account(1 , agency, password, 20, person);
            return true;
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

        bool transfer(const std::string& senderCpf, const std::string& receiverCpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            auto itSender = accounts.find(senderCpf);
            auto itReceiver = accounts.find(receiverCpf);
            if(itSender != accounts.end() && itReceiver != accounts.end() && itSender->second.getBalance() >= value){
                itSender->second.subBalance(value);
                itReceiver->second.addBalance(value);
                return true;
            }
            return false;
        }

        bool exit(){

            return true;
        }

        bool entry(){

            return true;
        }

        bool defineHost(std::string newHost){
            host = newHost;
            return true;
        }

        bool definePort(std::string newPort){
            port = newPort;
            return true;
        }

};
