//
// chat_client.cpp
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
#include <thread>
#include "ChatClient.hpp"
#include <vector>

using asio::ip::tcp;

ChatClient::ChatClient()
    :read_msg_(), io_context_(asio::io_context()), socket_(io_context_)
{
}

ChatClient::ChatClient(asio::io_context &io_context, const tcp::resolver::results_type &endpoints)
    : io_context_(io_context), socket_(io_context)
{
  DoConnect(endpoints);
}

ChatClient::ChatClient(const ChatClient& otherClient)
    :io_context_(otherClient.io_context_), socket_(io_context_), read_msg_(otherClient.read_msg_), write_msgs_(otherClient.write_msgs_)
{
}

ChatClient::~ChatClient()
{
}

void ChatClient::Write(const ChatMessage &msg)
{
  asio::post(io_context_,
             [this, msg]()
             {
               bool write_in_progress = !write_msgs_.empty();
               write_msgs_.push_back(msg);
               if (!write_in_progress)
               {
                 DoWrite();
               }
             });
}

void ChatClient::Close()
{
  asio::post(io_context_, [this]()
             { socket_.close(); });
}

void ChatClient::DoConnect(const tcp::resolver::results_type &endpoints)
{
  asio::async_connect(socket_, endpoints,
                      [this](std::error_code ec, tcp::endpoint)
                      {
                        if (!ec)
                        {
                          DoReadHeader();
                        }
                      });
}

void ChatClient::DoReadHeader()
{
  asio::async_read(socket_,
                   asio::buffer(read_msg_.GetData(), ChatMessage::header_length),
                   [this](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec && read_msg_.DecodeHeader())
                     {
                       DoReadBody();
                     }
                     else
                     {
                       socket_.close();
                     }
                   });
}

void ChatClient::DoReadBody()
{
  asio::async_read(socket_,
                   asio::buffer(read_msg_.GetBody(), read_msg_.GetBodyLength()),
                   [this](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec)
                     {
                       std::cout.write(read_msg_.GetBody(), read_msg_.GetBodyLength());
                       std::cout << "\n";
                       DoReadHeader();
                     }
                     else
                     {
                       socket_.close();
                     }
                   });
}

void ChatClient::DoWrite()
{
  asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().GetData(),
                                 write_msgs_.front().GetLength()),
                    [this](std::error_code ec, std::size_t /*length*/)
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
                        socket_.close();
                      }
                    });
}

int main(int argc, char *argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: chat_client <host> <port>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], argv[2]);
    ChatClient c(io_context, endpoints);

    std::thread t([&io_context]()
                  { io_context.run(); });

    char line[ChatMessage::max_body_length + 1];
    while (std::cin.getline(line, ChatMessage::max_body_length + 1))
    {
      ChatMessage msg;
      msg.SetBodyLength(std::strlen(line));
      std::memcpy(msg.GetBody(), line, msg.GetBodyLength());
      msg.EncodeHeader();
      c.Write(msg);
    }

    c.Close();
    t.join();
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
