//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2023 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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
	const char *data() const;
	char *data();
	std::size_t length() const;
	const char *body() const;
	char *body();
	std::size_t body_length() const;
	void body_length(std::size_t new_length);
	bool decode_header();
	void encode_header();
};

#endif