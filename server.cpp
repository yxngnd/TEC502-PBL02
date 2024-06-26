#include <crow.h>
#include <iostream>
#include <map>
#include <mutex>
#include "bank.cpp"
#include <vector>
#include <fstream>
#include <sstream>

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

std::unordered_map<std::string, std::string> sessions;

std::string generateSessionToken() {
    // Gera um token de sessão aleatório
    std::string token = std::to_string(std::rand()) + std::to_string(std::time(0));
    return token;
}

int main() {
    crow::SimpleApp app;

    IndividualAccount account;
    Bank bank;

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)
    ([]() {
        std::ifstream file("../interfaces/index.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::GET)
    ([](){
        std::ifstream file("login.html");
        if (!file.is_open()){
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)
    ([&bank](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("cpf") || !body.has("password")) {
            return crow::response(400);
        }

        Account account(0, "", "", 0.0, Person("", "", ""));
        bool success = bank.login(body["cpf"].s(), body["password"].s(), account);
        if (success) {
            crow::json::wvalue res;

            std::string sessionToken = generateSessionToken();
            sessions[sessionToken] = account.person.getCpf();

            res["name"] = account.person.getName();
            res["cpf"] = account.person.getCpf();
            res["balance"] = account.getBalance();
            res["token"] = sessionToken; 
            return crow::response(200, res);
        } else {
            return crow::response(401, "Invalid credentials");
        }
    });

    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)
    ([&bank](const crow::request& req){
        auto body = crow::json::load(req.body);
        if(!body || !body.has("name") || !body.has("cpf") || !body.has("birthday")){
            return crow::response(400);
        }

        Person person(body["name"].s(), body["cpf"].s(), body["birthday"].s());
        bool success = bank.registerAccount(person, body["agency"].s(), body["password"].s());
        if(success){
            return crow::response(200, "Account created successfully");
        }
        else{
            return crow::response(400, "Error when creating account");
        }
    });

    CROW_ROUTE(app, "/deposit").methods(crow::HTTPMethod::POST)
    ([&bank](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("cpf") || !body.has("value")){
            return crow::response(400, "Invalid JSON");
        }

        bool success = bank.deposit(body["cpf"].s(), body["value"].d());
        if (success) {
            return crow::response(200);
        } else {
            return crow::response(404);
        }
    });

    CROW_ROUTE(app, "/withdraw").methods(crow::HTTPMethod::POST)
    ([&bank](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("cpf") || !body.has("valor")){
            return crow::response(400);
        }

        bool success = bank.withdraw(body["cpf"].s(), body["value"].d());
        if(success){
            return crow::response(200);
        }
        else{
            return crow::response(404);
        }
    });

    CROW_ROUTE(app, "/transfer").methods(crow::HTTPMethod::POST)
    ([&bank](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("senderCpf") || !body.has("receiverCpf") || !body.has("value")){
            return crow::response(400);
        }

        bool success = bank.transfer(body["senderCpf"].s(), body["receiverCpf"].s(), body["value"].d());
        if(success){
            return crow::response(200, "Transfer completed successfully");
        }
        else{
            return crow::response(404, "Transfer cannot be completed");
        }
    });

    app.port(8080).multithreaded().run();
    return 0;
}
