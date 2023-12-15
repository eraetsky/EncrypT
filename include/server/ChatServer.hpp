#pragma once

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

//----------------------------------------------------------------------

typedef std::deque<ChatMessage> ChatMessageQueue;

//----------------------------------------------------------------------

class ChatParticipant
{
public:
    virtual ~ChatParticipant() {}
    virtual void deliver(const ChatMessage &msg) = 0;
};

typedef std::shared_ptr<ChatParticipant> ChatParticipant_ptr;

//----------------------------------------------------------------------

class ChatRoom
{
public:
    ChatRoom();
    ChatRoom(std::set<ChatParticipant_ptr> participants, ChatMessageQueue recent_msgs);
    ChatRoom(const ChatRoom &otherRoom);
    ~ChatRoom();
    void join(ChatParticipant_ptr participant);
    void leave(ChatParticipant_ptr participant);
    void deliver(const ChatMessage &msg);

    ChatMessageQueue Get_MessageQueue() const;

    ChatRoom* Set_MessageQueue(ChatMessageQueue &otherMessageQueue);

private:
    std::set<ChatParticipant_ptr> participants_;
    enum
    {
        max_recent_msgs = 100
    };
    ChatMessageQueue recent_msgs_;
};

//----------------------------------------------------------------------

class ChatSession
    : public ChatParticipant,
      public std::enable_shared_from_this<ChatSession>
{
public:
    ChatSession(tcp::socket &socket, ChatRoom &room);
    ChatSession();
    ChatSession(const ChatSession &otherSession, asio::io_context &newContext);
    ~ChatSession();
    void start();
    void deliver(const ChatMessage &msg);

     ChatRoom GetRoom() const;

    ChatMessage Get_Message() const;

    ChatMessageQueue Get_MessageQueue() const;

    ChatSession* SetRoom(ChatRoom &otherRoom);

    ChatSession* Set_Message(ChatMessage &otherMessage);

    ChatSession* Set_MessageQueue(ChatMessageQueue &otherMessageQueue);

private:
    void do_read_header();
    void do_read_body();
    void do_write();


    tcp::socket socket_;
    ChatRoom &room_;
    ChatMessage read_msg_;
    ChatMessageQueue write_msgs_;
};

//----------------------------------------------------------------------

class ChatServer
{
public:

    ChatServer();
    ChatServer(asio::io_context &io_context, const tcp::endpoint &endpoint);
    ChatServer(const ChatServer &otherServer, asio::io_context &newContext);
    ~ChatServer();
    ChatRoom GetRoom() const;
    ChatServer* SetRoom(ChatRoom &otherRoom);


private:
    void do_accept();

    tcp::acceptor acceptor_;
    ChatRoom room_;
};

//----------------------------------------------------------------------
