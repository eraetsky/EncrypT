#pragma once

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include "ChatMessage.hpp"

using asio::ip::tcp;
using ChatMessageQueue = std::deque<ChatMessage>;

class ChatClient
{
public:
    ChatClient(asio::io_context &io_context, const tcp::resolver::results_type &endpoints);
    ChatClient();
    ChatClient(const ChatClient& otherClient);
    ~ChatClient();
    void write(const ChatMessage &msg);
    void close();

private:
    void do_connect(const tcp::resolver::results_type &endpoints);
    void do_read_header();
    void do_read_body();
    void do_write();

private:
    asio::io_context &io_context_;
    tcp::socket socket_;
    ChatMessage read_msg_;
    ChatMessageQueue write_msgs_;
};
