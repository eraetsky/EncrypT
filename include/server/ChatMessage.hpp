#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

class ChatMessage
{
public:
  static constexpr std::size_t header_length = 4;
  static constexpr std::size_t max_body_length = 512;

  ChatMessage();
  ChatMessage(std::size_t body_length);
  ChatMessage(const ChatMessage& otherMessage);
  ~ChatMessage();
  const char *data() const;
  char *data();
  std::size_t length() const;
  const char *body() const;
  char *body();
  std::size_t body_length() const;
  void body_length(std::size_t new_length);
  bool decode_header();
  void encode_header();

private:
  char data_[header_length + max_body_length];
  std::size_t body_length_;
};
