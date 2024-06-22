#include <iostream>
#include <string>

class Person{
    private:
        short int id;
        std::string name;
        std::string birthday;
        std::string cpf;

    public:

        Person(){}

        Person(std::string nameInit, std::string birthdayInit, std::string cpfInit){
            name = nameInit;
            birthday = birthdayInit;
            cpf = cpfInit;
        }

        void setName(std::string newName){
            name = newName;
        }   

        void setBirthday(std::string newBirthday){
            birthday = newBirthday;
        }   

        void setCpf(std::string newCpf){
            cpf = newCpf;
        }

        std::string getName(){
            return name;
        }

        std::string getBirthday(){
            return birthday;
        }

        std::string getCpf() const {
            return cpf;
        }
};