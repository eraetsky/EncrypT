#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>

class ChatMessage
{
public:
  static constexpr std::size_t header_length = 4;
  static constexpr std::size_t max_body_length = 512;

  ChatMessage();
  ChatMessage(std::size_t body_length);
  ChatMessage(const ChatMessage &otherMessage);

  void operator=(const ChatMessage &otherMessage);

  friend bool operator==(const ChatMessage &thisMessage, const ChatMessage &otherMessage);
  friend bool operator!=(const ChatMessage &thisMessage, const ChatMessage &otherMessage);

  friend ChatMessage operator+(ChatMessage &thisMessage, const ChatMessage &otherMessage);

  ChatMessage &operator++();
  ChatMessage operator++(int);

  friend std::ostream &operator<<(std::ostream &out, const ChatMessage &message);
  friend std::istream &operator>>(std::istream &in, ChatMessage &message);

  ChatMessage &operator()(char data[], size_t size, std::size_t new_length);

  explicit operator char *();
  explicit ChatMessage(int);

  ~ChatMessage();

  const char *data() const;
  char *data();
  ChatMessage *set_data(char data[], size_t size);
  std::size_t length() const;
  const char *body() const;
  char *body();
  std::size_t body_length() const;
  ChatMessage *body_length(std::size_t new_length);
  bool decode_header();
  void encode_header();

private:
  char data_[header_length + max_body_length];
  std::size_t body_length_;
};
