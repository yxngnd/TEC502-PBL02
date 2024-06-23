#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <vector>
#include "account.cpp"

enum class Vote { COMMIT, ABORT };
enum class Decision { COMMIT, ABORT };

std::unordered_map<std::string, Account> accounts;

class Participant{
    public:
        virtual Vote prepare() = 0;
        virtual void commit() = 0;
        virtual void abort() = 0;
};

class Coordinator{
    private:
        std::vector<Participant *> participants;
    public:

        void addParticipant(Participant* p){

        }

        void execute(){

        }
};

class Bank : public Participant{
    private:
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

        Vote prepare() override{

        }

        void commit() override{

        }

        void abort() override{

        }

};
