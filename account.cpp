#include <iostream>
#include <vector>
#include <mutex>

class Account{

    private:
        std::string name;
        std::string cpf;
        bool type;
        std::string password;
        double balance;
    public:
        
        enum class AccountType { Individual = 0, Joint = 1 };

        Account(){}

        Account(std::string name, std::string cpf, bool type, std::string password, double balance) : name(name), cpf(cpf), type(type), password(password), balance(balance){
            name = name;
            cpf = cpf;
            type = type;
            password = password;
            balance = balance;
        }

        void setName(std::string newName){
            name = newName;
        }    

        void setCpf(std::string newCpf){
            cpf = newCpf;
        }

        std::string getName(){
            return name;
        }

        std::string getCpf(){
            return cpf;
        }

        bool getType(){
            return type;
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
            balance += value;
            return balance + value;
        }

        double subBalance(double value){
            balance -= value;
            return balance - value;
        }
};