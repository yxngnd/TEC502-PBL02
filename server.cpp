#include <crow.h>
#include <iostream>
#include <map>
#include <mutex>
#include "account.cpp"
#include <vector>
#include "person.cpp"

std::mutex accounts_mutex;

/*
bool authenticate(const std::string& cpf, const std::string& password) {
    std::lock_guard<std::mutex> lock(accounts_mutex);
    auto it = accounts.find(cpf);
    if (it != accounts.end() && it->second.getPassword() == password) {
        return true;
    }
    return false;
}
*/

int main() {
    crow::SimpleApp app;

    Account account;
    Person p("jorge", "01/01/2001", "012345678910");
    Account a(1, "123", "123", 20, p);

    CROW_ROUTE(app, "/")
    ([](const crow::request& req){

    });

    CROW_ROUTE(app, "/login")
    .methods(crow::HTTPMethod::POST)
    ([&account](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("cpf") || !body.has("password")) {
            return crow::response(400);
        }

        Account account(1, "", "", 0.0, Person("", "", ""));
        bool success = account.login(body["cpf"].s(), body["password"].s(), account);
        if (success) {
            crow::json::wvalue res;
            res["nome"] = account.person.getName();
            res["cpf"] = account.person.getCpf();
            res["saldo"] = account.getBalance();
            return crow::response(200, res);
        } else {
            return crow::response(401, "Invalid credentials");
        }
    });

    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)
    ([&account](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body || !body.has("name") || !body.has("cpf") || !body.has("birthday")){
            return crow::response(400);
        }

        Person person(body["name"].s(), body["cpf"].s(), body["birthday"].s());
        bool success = account.registerAccount(person, body["agency"].s(), body["password"].s());
        if(success){
            return crow::response(200, "Account created successfully");
        }
        else{
            return crow::response(400, "Error when creating account");
        }
    });

    CROW_ROUTE(app, "/deposit").methods(crow::HTTPMethod::POST)
    ([&account](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("cpf") || !body.has("value")){
            return crow::response(400, "Invalid JSON");
        }

        bool success = account.deposit(body["cpf"].s(), body["value"].d());
        if (success) {
            return crow::response(200);
        } else {
            return crow::response(404);
        }
    });

    CROW_ROUTE(app, "/withdraw").methods(crow::HTTPMethod::POST)
    ([&account](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("cpf") || !body.has("valor")){
            return crow::response(400);
        }

        bool success = account.withdraw(body["cpf"].s(), body["value"].d());
        if(success){
            return crow::response(200);
        }
        else{
            return crow::response(404);
        }
    });

    CROW_ROUTE(app, "/transfer").methods(crow::HTTPMethod::POST)
    ([&account](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("senderCpf") || !body.has("receiverCpf") || !body.has("value")){
            return crow::response(400);
        }

        bool success = account.transfer(body["senderCpf"].s(), body["receiverCpf"].s(), body["value"].d());
        if(success){
            return crow::response(200, "Transfer completed successfully");
        }
        else{
            return crow::response(404, "Transfer cannot be completed");
        }
    });

    CROW_ROUTE(app, "/accounts/<string>")
    ([](const crow::request& req, std::string accountId){
        
    });

    CROW_ROUTE(app, "/balance/<string>")
    ([](const crow::request& req, std::string accountId) {
        if (accounts.find(accountId) != accounts.end()) {
            return crow::response(std::to_string(accounts[accountId].getBalance()));
        } 
        else {
            return crow::response(404);
        }
    });

    app.port(8080).multithreaded().run();
    return 0;
}
