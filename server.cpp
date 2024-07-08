#include <crow.h>
#include <iostream>
#include <map>
#include <mutex>
#include "bank.cpp"
#include <vector>
#include <fstream>
#include <sstream>
#include "httplib.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::mutex accounts_mutex;

std::unordered_map<std::string, std::string> consortium = {

    {"BANCO1", "127.0.0.1:8080"},
    {"BANCO2", "127.0.0.1:8081"},
    {"BANCO3", "127.0.0.1:8082"}

};

std::unordered_map<std::string, std::string> sessions;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    int port = std::stoi(argv[1]);

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

    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::GET)
    ([]() {
        std::ifstream file("../interfaces/register.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/withdraw").methods(crow::HTTPMethod::GET)
    ([]() {
        std::ifstream file("../interfaces/withdraw.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/deposit").methods(crow::HTTPMethod::GET)
    ([]() {
        std::ifstream file("../interfaces/deposit.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/transfer").methods(crow::HTTPMethod::GET)
    ([]() {
        std::ifstream file("../interfaces/transfer.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/account").methods(crow::HTTPMethod::GET)
    ([]() {
        std::ifstream file("../interfaces/account.html");
        if (!file.is_open()) {
            return crow::response(404, "File not found.");
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return crow::response(buffer.str());
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::GET)
    ([](){
        std::ifstream file("../interfaces/login.html");
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

            res["name"] = account.getName();
            res["cpf"] = account.getCpf();
            res["type"] = account.getType();
            res["balance"] = account.getBalance();
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

    CROW_ROUTE(app, "/account/cpf").methods(crow::HTTPMethod::GET)
    ([&bank](const crow::request& req){
        std::string cpf = req.url_params.get("cpf");
        //Account account("", "", 0, "", 0.0);
        json data = bank.getAccountsByCpf(cpf);
        return crow::response(200, data.dump());
    });
    
    CROW_ROUTE(app, "/accounts").methods(crow::HTTPMethod::GET)
    ([](const crow::request& req) {
        std::string cpf = req.url_params.get("cpf");

        json accountsList = json::array();
        bool foundAccounts = false;

        for (const auto& bank : consortium) {
            httplib::Client client(bank.second.c_str());
            auto res = client.Get(("/account/cpf?cpf=" + cpf).c_str());
            if (res && res->status == 200) {
                foundAccounts = true;

                auto jsonRes = json::parse(res->body);
                if (!jsonRes.is_array()) {
                    return crow::response(500, "Invalid JSON response from bank");
                }

                for (auto& account : jsonRes) {
                    json accountInfo;
                    accountInfo["bank_name"] = bank.first;
                    accountInfo["account"] = account;
                    accountsList.push_back(accountInfo);
                }
            }
        }

        if (foundAccounts) {
            return crow::response(200, accountsList.dump());
        } else {
            return crow::response(404, "No accounts found in consortium for the given CPF");
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
    ([&](const crow::request& req) {
        auto body = json::parse(req.body);
        if (!body.contains("senders") || !body.contains("receiver")) {
            return crow::response(400);
        }

        auto senders = body["senders"];
        auto receiver = body["receiver"];

        if (!receiver.contains("cpf") || !receiver.contains("bank") || !receiver.contains("value")) {
            return crow::response(400);
        }

        std::string receiverCpf = receiver["cpf"];
        std::string receiverBank = receiver["bank"];
        double totalValue = receiver["value"];
        std::string receiverHost = consortium[receiverBank];

        double cumulativeValue = 0.0;

        // Calcula o valor cumulativo antes de iniciar qualquer operação
        for (const auto& sender : senders) {
            if (!sender.contains("cpf") || !sender.contains("bank") || !sender.contains("value")) {
                return crow::response(400);
            }

            double value = sender["value"];
            cumulativeValue += value;
        }

        // Certificar que o valor cumulativo seja igual ao valor a ser recebido
        if (cumulativeValue != totalValue) {
            return crow::response(400, "Total value mismatch");
        }

        // Preparação e commit das operações de retirada
        std::vector<std::pair<std::string, double>> successfulWithdrawals;
        for (const auto& sender : senders) {
            std::string senderCpf = sender["cpf"];
            std::string senderBank = sender["bank"];
            double value = sender["value"];
            std::string senderHost = consortium[senderBank];

            json sendBody = { {"cpf", senderCpf}, {"value", value} };

            httplib::Client sClient(senderHost.c_str());
            auto res = sClient.Post("/withdraw", sendBody.dump(), "application/json");

            if (!res || res->status != 200) {
                // Em caso de falha na retirada, reverter retiradas anteriores
                for (const auto& successfulWithdrawal : successfulWithdrawals) {
                    std::string rollbackCpf = successfulWithdrawal.first;
                    double rollbackValue = successfulWithdrawal.second;
                    json rollbackBody = { {"cpf", rollbackCpf}, {"value", rollbackValue} };

                    httplib::Client rollbackClient(consortium[senderBank].c_str());
                    rollbackClient.Post("/deposit", rollbackBody.dump(), "application/json");
                }
                return crow::response(404, "Transfer cannot be completed: withdrawal failed");
            } else {
                successfulWithdrawals.push_back({senderCpf, value});
            }
        }

        // Preparar o corpo da requisição como JSON corretamente
        json receiveBody = { {"cpf", receiverCpf}, {"value", totalValue} };

        // Enviar requisição para o servidor do receptor
        httplib::Client receiverClient(receiverHost.c_str());
        auto receiverRes = receiverClient.Post("/deposit", receiveBody.dump(), "application/json");

        // Checar a resposta do servidor do receptor
        if (receiverRes && receiverRes->status == 200) {
            return crow::response(200, "Transfer completed successfully");
        } else {
            // Em caso de falha no depósito, tentar reverter todas as retiradas
            for (const auto& successfulWithdrawal : successfulWithdrawals) {
                std::string rollbackCpf = successfulWithdrawal.first;
                double rollbackValue = successfulWithdrawal.second;
                std::string rollbackHost = consortium[receiverBank];

                json rollbackBody = { {"cpf", rollbackCpf}, {"value", rollbackValue} };

                httplib::Client rollbackClient(rollbackHost.c_str());
                rollbackClient.Post("/deposit", rollbackBody.dump(), "application/json");
            }
            return crow::response(404, "Transfer cannot be completed: deposit failed");
        }
    });

    app.port(port).multithreaded().run();
    return 0;
}
