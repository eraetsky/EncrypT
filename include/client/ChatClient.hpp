
#ifndef CHATCLIENT_HPP
#define CHATCLIENT_HPP

#include "asio.hpp"
#include "ChatMessage.hpp"
#include <deque>

using asio::ip::tcp;

using ChatMessageQueue = std::deque<ChatMessage>;

class ChatClient
{
private:
	asio::io_context& io_context_;
	tcp::socket socket_;
	ChatMessage read_msg_;
	ChatMessageQueue write_msgs_;

public:
	ChatClient();
	ChatClient(asio::io_context& io_context, const tcp::resolver::results_type& endpoints);
	ChatClient(const ChatClient& otherClient);
	~ChatClient();
	void Write(const ChatMessage& msg);
	void Close();

private:
	void DoConnect(const tcp::resolver::results_type& endpoints);
	void DoReadHeader();
	void DoReadBody();
	void DoWrite();
};

#endif