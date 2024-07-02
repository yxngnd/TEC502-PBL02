#include <crow.h>
#include <iostream>
#include <map>
#include <mutex>
#include "bank.cpp"
#include <vector>
#include <fstream>
#include <sstream>

std::mutex accounts_mutex;

std::unordered_map<std::string, std::string> consortium = {

    {"BANCO1", "127.0.0.1:8080"},
    {"BANCO2", "127.0.0.1:8081"}

};
std::unordered_map<std::string, std::string> sessions;

std::string generateSessionToken() {
    // Gera um token de sessão aleatório
    std::string token = std::to_string(std::rand()) + std::to_string(std::time(0));
    return token;
}

int main() {
    crow::SimpleApp app;

    Account account;
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

        Account account("", "", 0, "", 0.0);
        bool success = bank.login(body["cpf"].s(), body["password"].s(), account);
        if (success) {
            crow::json::wvalue res;

            std::string sessionToken = generateSessionToken();
            sessions[sessionToken] = account.getCpf();

            res["name"] = account.getName();
            res["cpf"] = account.getCpf();
            res["type"] = account.getType();
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
        if(!body || !body.has("name") || !body.has("cpf")){
            return crow::response(400);
        }

        bool success = bank.registerAccount(body["name"].s(), body["cpf"].s(), body["type"].i(), body["password"].s());
        if(success){
            return crow::response(200, "Account created successfully");
        }
        else{
            return crow::response(400, "Error when creating account");
        }
    });

    CROW_ROUTE(app, "/account").methods(crow::HTTPMethod::GET)
    ([&bank](const crow::request& req){
        std::string token = req.url_params.get("token");
        Account account("", "", 0, "", 0.0);
        if(bank.getAccount(sessions[token], account)){
            crow::json::wvalue res;
            res["name"] = account.getName();
            res["cpf"] = account.getCpf();
            res["type"] = account.getType();
            res["balance"] = account.getBalance();
            res["token"] = token; 
            return crow::response(200, res);
        }
        else{
            return crow::response(400, "This account doesnt exist.");
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
            return crow::response(200, "Deposit make with success");
        } else {
            return crow::response(404);
        }
    });

    CROW_ROUTE(app, "/withdraw").methods(crow::HTTPMethod::POST)
    ([&bank](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if(!body || !body.has("cpf") || !body.has("value")){
            return crow::response(400);
        }

        bool success = bank.withdraw(body["cpf"].s(), body["value"].d());
        if(success){
            return crow::response(200, "Withdraw make with success");
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
