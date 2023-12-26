#pragma once

#include <string>
#include <iostream>
#include "Singleton.hpp"
#include "asio.hpp"

using asio::ip::tcp;

class Client
{
private:
    std::string username_;
    std::string password_;
    std::string ip_;
    std::string port_;

protected:
    asio::io_context io_context_;

public:
    Client() {}
    Client(asio::io_context &io_context, const std::string &username, const std::string &password) : username_{username}, password_{password} {}
    virtual ~Client(){};
    virtual tcp::endpoint getEndpoint();
    virtual std::string getUserId();
};

class ClientApp : public Client, public Singleton<ClientApp>
{
private:
    std::string userid_;
    tcp::endpoint endpoint_;

public:
    ClientApp(const std::string &userid, tcp::endpoint &endpoint) : userid_{userid}, endpoint_{endpoint} {}
    tcp::endpoint getEndpoint() override;
    std::string getUserId() override;
};