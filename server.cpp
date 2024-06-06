#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

void handlePostRequest();

void handleGetRequest();

void handleRequest();

void* runServer(void* arg);