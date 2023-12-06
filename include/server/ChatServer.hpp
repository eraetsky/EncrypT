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

typedef std::deque<ChatMessage> ChatMessageQueue;

class ChatParticipant
{
public:
    virtual ~ChatParticipant() {}
    virtual void Deliver(const ChatMessage &msg) = 0;
};

typedef std::shared_ptr<ChatParticipant> ChatParticipantPtr;

class ChatRoom
{
private:
    std::set<ChatParticipantPtr> participants_;
    enum
    {
        max_recent_msgs = 100
    };
    ChatMessageQueue recent_msgs_;

public:
    ChatRoom();
    ChatRoom(std::set<ChatParticipantPtr> participants, ChatMessageQueue recent_msgs);
    ChatRoom(const ChatRoom& otherRoom);
    ~ChatRoom();
    void Join(ChatParticipantPtr participant);
    void Leave(ChatParticipantPtr participant);
    void Deliver(const ChatMessage &msg);
};

class ChatSession : public ChatParticipant, public std::enable_shared_from_this<ChatSession>
{
private:
    tcp::socket socket_;
    ChatRoom &room_;
    ChatMessage read_msg_;
    ChatMessageQueue write_msgs_;

public:
    ChatSession();
    ChatSession(tcp::socket socket, ChatRoom &room);
    ChatSession(const ChatSession& otherSession);
    ~ChatSession();
    void Start();
    void Deliver(const ChatMessage &msg);

private:
    void DoReadHeader();
    void DoReadBody();
    void DoWrite();
};

class ChatServer
{
private:
    tcp::acceptor acceptor_;
    ChatRoom room_;

public:
    ChatServer();
    ChatServer(asio::io_context &io_context, const tcp::endpoint &endpoint);
    ChatServer(const ChatServer& otherServer);
    ~ChatServer();
    ChatRoom GetRoom() const;
    ChatServer* SetRoom(ChatRoom otherRoom);

private:
    void DoAccept();
};

#endif