#include "ChatMessage.hpp"

ChatMessage::ChatMessage()
    : body_length_(0)
{
}

ChatMessage::ChatMessage(std::size_t body_length)
{
    this->body_length_ = body_length;
}

ChatMessage::ChatMessage(const ChatMessage &otherMsg)
{
    this->body_length_ = otherMsg.body_length_;
}

ChatMessage::~ChatMessage()
{
}

char *ChatMessage::GetData()
{
    return data_;
}

std::size_t ChatMessage::GetLength() const
{
    return header_length + body_length_;
}

char *ChatMessage::GetBody()
{
    return data_ + header_length;
}

std::size_t ChatMessage::GetBodyLength() const
{
    return body_length_;
}

ChatMessage ChatMessage::SetBodyLength(std::size_t new_length)
{
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
    return *this;
}

bool ChatMessage::DecodeHeader()
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

void ChatMessage::EncodeHeader()
{
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}
