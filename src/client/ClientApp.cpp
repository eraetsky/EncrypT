#include "ClientApp.hpp"

tcp::endpoint Client::getEndpoint()
{
    std::cout << "Client::getEndpoint\n";
    tcp::endpoint endpoint(tcp::v4(), std::atoi(port_.c_str()));

    return endpoint;
}

std::string Client::getUserId()
{
    // std::string userid = dbClient::getInstance().getUserIdByUserName(username_);
    std::cout << "Client::getUserId\n";
    std::string userid = "abfjgjt-245-gthjhgn";
    return userid;
}

tcp::endpoint ClientApp::getEndpoint()
{
    std::cout << "ClientApp::getEndpoint\n";
    return endpoint_;
}

std::string ClientApp::getUserId()
{
    std::cout << "ClientApp::getUserId\n";
    return userid_;
}
