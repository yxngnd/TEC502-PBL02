#include <iostream>
#include "person.cpp"

class Participant{
    public:
        virtual Vote prepare() = 0;
        virtual void commit() = 0;
        virtual void abort() = 0;
};

class Coordinator{
    private:
        std::vector<Participant*> participants;
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
        std::string account;
        std::string password;
        double balance;
    public:

        void setAgency(std::string newAgency){
            agency = newAgency;
        }

        void setAccount(std::string newAccount){
            account = newAccount;
        }

        std::string getAgency(){
            return agency;
        }
        
        std::string getAccount(){
            return account;
        }

        void setBalance(double newBalance){
            balance = newBalance;
        }

        std::string getBalance(){
            return std::to_string(balance);
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