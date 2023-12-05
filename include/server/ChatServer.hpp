//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "ChatMessage.hpp"

using asio::ip::tcp;

typedef std::deque<ChatMessage> chat_message_queue;

class ChatParticipant
{
public:
    virtual ~chat_participant() {}
    virtual void deliver(const ChatMessage &msg) = 0;
};

typedef std::shared_ptr<ChatParticipant> chat_participant_ptr;

class ChatRoom
{
private:
    std::set<chat_participant_ptr> participants_;
    enum
    {
        max_recent_msgs = 100
    };
    chat_message_queue recent_msgs

        public : void
                 join(chat_participant_ptr participant);
    void leave(chat_participant_ptr participant);
    void deliver(const ChatMessage &msg);
};

class ChatSession : public ChatParticipant, public std::enable_shared_from_this<ChatSession>
{
private:
    tcp::socket socket_;
    ChatRoom &room_;
    ChatMessage read_msg_;
    chat_message_queue write_msgs_;

public:
    ChatSession(tcp::socket socket, chat_room &room);
    void start();
    void deliver(const ChatMessage &msg);

private:
    void do_read_header();
    void do_read_body();
    void do_write();
};

class ChatServer
{
private:
    tcp::acceptor acceptor_;
    ChatRoom room_;

public:
    ChatServer(asio::io_context &io_context, const tcp::endpoint &endpoint);

private:
    void do_accept();
};

#endif