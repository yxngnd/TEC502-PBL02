#include <iostream>
#include "person.cpp"

class Account{

    private:
        short int id;
        std::string agency;
        std::string account;
    
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