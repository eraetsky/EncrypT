#include "ChatMessage.hpp"

ChatMessage::ChatMessage()
    : body_length_(0)
{
}

ChatMessage::ChatMessage(const ChatMessage &otherMessage)
    : body_length_(otherMessage.body_length_)
{
}

ChatMessage::~ChatMessage()
{
}

ChatMessage::ChatMessage(std::size_t body_length)
    : body_length_(body_length)
{
}

void ChatMessage::operator=(const ChatMessage &otherMessage)
{
    std::copy(otherMessage.data_, otherMessage.data_ + header_length + max_body_length, data_);
    body_length_ = otherMessage.body_length_;
}

const char *ChatMessage::data() const
{
    return data_;
}

char *ChatMessage::data()
{
    return data_;
}

ChatMessage *ChatMessage::set_data(char data[], size_t size)
{
    std::copy(data, data + size, data_);
    return this;
}

std::size_t ChatMessage::length() const
{
    return header_length + body_length_;
}

const char *ChatMessage::body() const
{
    return data_ + header_length;
}

char *ChatMessage::body()
{
    return data_ + header_length;
}

std::size_t ChatMessage::body_length() const
{
    return body_length_;
}

ChatMessage *ChatMessage::body_length(std::size_t new_length)
{
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
    return this;
}

bool ChatMessage::decode_header()
{
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length)
    {
        body_length_ = 0;
        return false;
    }
    return true;
}

void ChatMessage::encode_header()
{
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}
