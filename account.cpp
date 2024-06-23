#include <iostream>
#include <vector>
#include <mutex>
#include "person.cpp"
#include <optional>

class Account{

    private:
        short int id;
        std::string agency;
        std::string password;
        double balance;
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
            return balance;
        }

        double addBalance(double value){
            return balance + value;
        }

        double subBalance(double value){
            return balance - value;
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