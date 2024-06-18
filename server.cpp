#include <crow.h>
#include <iostream>
#include <map>

using namespace crow;

std::map<std::string, int> accounts;

int main() {
    SimpleApp app;

    accounts.insert(std::make_pair("1", 1));

    CROW_ROUTE(app, "/")
    ([](const request& req){

    });
    
    CROW_ROUTE(app, "/login")
    ([](const request& req){
        
    });

    CROW_ROUTE(app, "/register")
    ([](const request& req){
        
    });

    CROW_ROUTE(app, "/accounts/<string>")
    ([](const request& req, std::string accountId){
        
    });

    CROW_ROUTE(app, "/balance/<string>")
    ([](const request& req, std::string accountId) {
        if (accounts.find(accountId) != accounts.end()) {
            return response(std::to_string(accounts[accountId]));
        } 
        else {
            return response(404);
        }
    });

    CROW_ROUTE(app, "/deposit/<string>/<int>")
    ([](const request& req, std::string accountId, int amount) {
        accounts[accountId] += amount;
        return response(200);
    });

    CROW_ROUTE(app, "/withdraw/<string>/<int>")
    ([](const request& req, std::string accountId, int amount) {
        if (accounts[accountId] >= amount) {
            accounts[accountId] -= amount;
            return response(200);
        } 
        else {
            return response(400);
        }
    });

    app.port(8080).multithreaded().run();
    return 0;
}
