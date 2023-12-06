
#ifndef CHATCLIENT_HPP
#define CHATCLIENT_HPP

#include "asio.hpp"
#include "ChatMessage.hpp"

using asio::ip::tcp;

typedef std::deque<ChatMessage> chat_message_queue;

class ChatClient
{
private:
  asio::io_context &io_context_;
  tcp::socket socket_;
  ChatMessage read_msg_;
  chat_message_queue write_msgs_;

public:
  ChatClient(asio::io_context &io_context, const tcp::resolver::results_type &endpoints);
  void write(const ChatMessage &msg);
  void close();

private:
  void do_connect(const tcp::resolver::results_type &endpoints);
  void do_read_header();
  void do_read_body();
  void do_write();
};

#endif