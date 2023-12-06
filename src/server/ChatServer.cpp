//
// chat_server.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include "asio.hpp"
#include "ChatMessage.hpp"
#include "ChatServer.hpp"

using asio::ip::tcp;

typedef std::deque<ChatMessage> ChatMessageQueue;

typedef std::shared_ptr<ChatParticipant> ChatParticipantPtr;

ChatRoom::ChatRoom()
{
}

ChatRoom::ChatRoom(std::set<ChatParticipantPtr> participants, ChatMessageQueue recent_msgs)
    :recent_msgs_(recent_msgs), participants_(participants)
{
}

ChatRoom::ChatRoom(const ChatRoom& otherRoom)
    :recent_msgs_(otherRoom.recent_msgs_), participants_(otherRoom.participants_)
{
}

ChatRoom::~ChatRoom()
{
}

void ChatRoom::Join(ChatParticipantPtr participant)
{
  participants_.insert(participant);
  for (auto msg : recent_msgs_)
    participant->Deliver(msg);
}

void ChatRoom::Leave(ChatParticipantPtr participant)
{
  participants_.erase(participant);
}

void ChatRoom::Deliver(const ChatMessage &msg)
{
  recent_msgs_.push_back(msg);
  while (recent_msgs_.size() > max_recent_msgs)
    recent_msgs_.pop_front();

  for (auto participant : participants_)
    participant->Deliver(msg);
}

ChatSession::ChatSession()
    :room_(ChatRoom::ChatRoom()), read_msg_(), socket_(asio::io_context()), write_msgs_()
{
}

ChatSession::ChatSession(tcp::socket socket, ChatRoom &room)
    : socket_(std::move(socket)),
      room_(room)
{
}

//ChatSession::ChatSession(const ChatSession& otherSession)
//    :room_(otherSession.room_), read_msg_(otherSession.read_msg_), write_msgs_(otherSession.write_msgs_),socket_(otherSession.socket_) 
//{         //не можу знайти спосіб скопіювати сокет
//}

ChatSession::~ChatSession()
{
}

void ChatSession::Start()
{
  room_.Join(shared_from_this());
  DoReadHeader();
}

void ChatSession::Deliver(const ChatMessage &msg)
{
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress)
  {
    DoWrite();
  }
}

void ChatSession::DoReadHeader()
{
  auto self(shared_from_this());
  asio::async_read(socket_,
                   asio::buffer(read_msg_.GetData(), ChatMessage::header_length),
                   [this, self](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec && read_msg_.DecodeHeader())
                     {
                       DoReadBody();
                     }
                     else
                     {
                       room_.Leave(shared_from_this());
                     }
                   });
}

void ChatSession::DoReadBody()
{
  auto self(shared_from_this());
  asio::async_read(socket_,
                   asio::buffer(read_msg_.GetBody(), read_msg_.GetBodyLength()),
                   [this, self](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec)
                     {
                       room_.Deliver(read_msg_);
                       DoReadHeader();
                     }
                     else
                     {
                       room_.Leave(shared_from_this());
                     }
                   });
}

void ChatSession::DoWrite()
{
  auto self(shared_from_this());
  asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().GetData(),
                                 write_msgs_.front().GetLength()),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                      if (!ec)
                      {
                        write_msgs_.pop_front();
                        if (!write_msgs_.empty())
                        {
                          DoWrite();
                        }
                      }
                      else
                      {
                        room_.Leave(shared_from_this());
                      }
                    });
}

ChatServer::ChatServer()
    :acceptor_(asio::io_context(),tcp::endpoint())
{
    DoAccept();
}

ChatServer::ChatServer(asio::io_context& io_context, const tcp::endpoint& endpoint)
    : acceptor_(io_context, endpoint)
{
    DoAccept();
}

//ChatServer::ChatServer(const ChatServer& otherServer) //треба розібратись як скопіювати io_context та endpoint
//{
//}

ChatServer::~ChatServer()
{
}

ChatRoom ChatServer::GetRoom() const
{
    return room_;
}

ChatServer* ChatServer::SetRoom(ChatRoom otherRoom)
{
    room_ = otherRoom;
    return this;
}

void ChatServer::DoAccept()
{
  acceptor_.async_accept(
      [this](std::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
          std::make_shared<ChatSession>(std::move(socket), room_)->Start();
        }

        DoAccept();
      });
}

int main(int argc, char *argv[])
{
  try
  {
    if (argc < 2)
    {
      std::cerr << "Usage: chat_server <port> [<port> ...]\n";
      return 1;
    }

    asio::io_context io_context;

    std::list<ChatServer> servers;
    for (int i = 1; i < argc; ++i)
    {
      tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
      servers.emplace_back(io_context, endpoint);
    }

    io_context.run();
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
