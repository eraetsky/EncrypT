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
    ChatClient(const ChatClient &otherClient);
    ~ChatClient();
    void write(const ChatMessage &msg);
    void close();
    asio::io_context &get_context() const;
    ChatMessage get_read_msg() const;
    ChatClient *set_read_msg(ChatMessage &newReadMsg);
    ChatMessageQueue get_write_msgs() const;
    ChatClient *set_write_msgs(const ChatMessageQueue &newWriteMsgs);

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
