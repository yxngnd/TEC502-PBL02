#include <iostream>
#include <vector>
#include <mutex>
#include "person.cpp"
#include <optional>

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

class Account : public Participant{

    private:
        short int id;
        std::string agency;
        std::string password;
        double balance;
        std::mutex mtx;
    public:
        Person person;

        Account(){}

        Account(int id, std::string agency, std::string password, double balance, Person person) : person(person){
            id = id;
            agency = agency;
            password = password;
            balance = balance;
            person = person;
        }

        bool login(const std::string& cpf, const std::string& password, Account& account) {
            std::lock_guard<std::mutex> lock(mtx);
            auto it = accounts.find(cpf);
            if (it != accounts.end() && it->second.password == password) {
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
                it->second.balance += value;
                return true;
            }
            return false;
        }

        bool withdraw(const std::string& cpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            auto it = accounts.find(cpf);
            if(it != accounts.end() && it->second.balance >= value){
                it->second.balance = it->second.balance;
                return true;
            }
            return false;
        }

        bool transfer(const std::string& senderCpf, const std::string& receiverCpf, double value){
            std::lock_guard<std::mutex> lock(mtx);
            auto itSender = accounts.find(senderCpf);
            auto itReceiver = accounts.find(receiverCpf);
            if(itSender != accounts.end() && itReceiver != accounts.end() && itSender->second.balance >= value){
                itSender->second.balance -= value;
                itReceiver->seconde.balance += value;
                return true;
            }
            return false;
        }

        void setAgency(std::string newAgency){
            agency = newAgency;
        }

        std::string getAgency(){
            return agency;
        }

        void setPassword(std::string newPassword){
            password = newPassword;
        }

        std::string getPassword(){
            return password;
        }

        void setBalance(double newBalance){
            balance = newBalance;
        }

        double getBalance(){
            balance;
        }

        void setBalance(double newBalance){
            balance = newBalance;
        }

        double addBalance(double value){
            return balance + value;
        }

        double subBalance(double value){
            return balance - value;
        }

        Vote prepare() override{

        }

        void commit() override{

        }

        void abort() override{

        }
};

class IndividualAccount : private Account{

    private:
        Person person;
};

class JointAccount : private Account{
    Person person1;
    Person person2;
};

class EntityAccount : private Account{

};