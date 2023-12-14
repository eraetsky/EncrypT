#ifndef CHATMESSAGE_HPP
#define CHATMESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class ChatMessage
{
public:
	static constexpr std::size_t header_length = 4;
	static constexpr std::size_t max_body_length = 512;

private:
	char data_[header_length + max_body_length];
	std::size_t body_length_;

public:
	ChatMessage();
	ChatMessage(std::size_t body_length);
	ChatMessage(const ChatMessage &otherMsg);
	~ChatMessage();
	char *GetData();
	const char *GetData() const;
	std::size_t GetLength() const;
	char *GetBody();
	const char *GetBody() const;
	std::size_t GetBodyLength() const;
	ChatMessage SetBodyLength(std::size_t new_length);
	bool DecodeHeader();
	void EncodeHeader();
};

#endif