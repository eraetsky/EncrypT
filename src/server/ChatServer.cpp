#include "ChatServer.hpp"

ChatRoom::ChatRoom()
    : recent_msgs_(), participants_()
{
}

ChatRoom::ChatRoom(std::set<ChatParticipant_ptr> participants, ChatMessageQueue recent_msgs)
    : participants_(participants), recent_msgs_(recent_msgs)
{
}

ChatRoom::ChatRoom(const ChatRoom &otherRoom)
    : participants_(otherRoom.participants_), recent_msgs_(otherRoom.recent_msgs_)
{
}

ChatRoom::~ChatRoom()
{
}

void ChatRoom::join(ChatParticipant_ptr participant)
{
  participants_.insert(participant);
  for (auto msg : recent_msgs_)
    participant->deliver(msg);
}

void ChatRoom::leave(ChatParticipant_ptr participant)
{
  participants_.erase(participant);
}

void ChatRoom::deliver(const ChatMessage &msg)
{
  recent_msgs_.push_back(msg);
  while (recent_msgs_.size() > max_recent_msgs)
    recent_msgs_.pop_front();

  for (auto participant : participants_)
    participant->deliver(msg);
}


ChatMessageQueue ChatRoom::Get_MessageQueue() const
{
  return recent_msgs_;
}

ChatRoom* ChatRoom::Set_MessageQueue(ChatMessageQueue &otherMessageQueue)
{
    recent_msgs_ = otherMessageQueue;
    return this;
}

const ChatMessage& ChatRoom::operator[](const int index) const
{
  return this->recent_msgs_[index];
}

ChatSession::ChatSession(tcp::socket &socket, ChatRoom &room)
    : socket_(std::move(socket)),
      room_(room)
{
}

ChatSession::ChatSession()
    : socket_(asio::io_context()), read_msg_(), write_msgs_(), room_(ChatRoom::ChatRoom())
{
}

ChatSession::ChatSession(const ChatSession &otherSession, asio::io_context &newContext)
    : read_msg_(otherSession.read_msg_), write_msgs_(otherSession.write_msgs_), room_(otherSession.room_),
      socket_(newContext)
{
}

ChatSession::~ChatSession()
{
}

void ChatSession::start()
{
  room_.join(shared_from_this());
  do_read_header();
}

void ChatSession::deliver(const ChatMessage &msg)
{
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress)
  {
    do_write();
  }
}

void ChatSession::do_read_header()
{
  auto self(shared_from_this());
  asio::async_read(socket_,
                   asio::buffer(read_msg_.data(), ChatMessage::header_length),
                   [this, self](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec && read_msg_.decode_header())
                     {
                       do_read_body();
                     }
                     else
                     {
                       room_.leave(shared_from_this());
                     }
                   });
}

void ChatSession::do_read_body()
{
  auto self(shared_from_this());
  asio::async_read(socket_,
                   asio::buffer(read_msg_.body(), read_msg_.body_length()),
                   [this, self](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec)
                     {
                       room_.deliver(read_msg_);
                       do_read_header();
                     }
                     else
                     {
                       room_.leave(shared_from_this());
                     }
                   });
}

void ChatSession::do_write()
{
  auto self(shared_from_this());
  asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().data(),
                                 write_msgs_.front().length()),
                    [this, self](std::error_code ec, std::size_t /*length*/)
                    {
                      if (!ec)
                      {
                        write_msgs_.pop_front();
                        if (!write_msgs_.empty())
                        {
                          do_write();
                        }
                      }
                      else
                      {
                        room_.leave(shared_from_this());
                      }
                    });
}

ChatRoom ChatSession::GetRoom() const
{
    return room_;
}

ChatMessage ChatSession::Get_Message() const
{
    return read_msg_;
}

ChatMessageQueue ChatSession::Get_MessageQueue() const
{
  return write_msgs_;
}

ChatSession* ChatSession::SetRoom(ChatRoom &otherRoom)
{
    room_ = otherRoom;
    return this;
}

ChatSession* ChatSession::Set_Message(ChatMessage &otherMessage)
{
    read_msg_ = otherMessage;
    return this;
}

ChatSession* ChatSession::Set_MessageQueue(ChatMessageQueue &otherMessageQueue)
{
    write_msgs_ = otherMessageQueue;
    return this;
}

ChatServer::ChatServer()
    : room_(), acceptor_(asio::io_context())
{
}

ChatServer::ChatServer(asio::io_context &io_context, const tcp::endpoint &endpoint)
    : acceptor_(io_context, endpoint)
{
  do_accept();
}

ChatServer::ChatServer(const ChatServer &otherServer, asio::io_context &newContext)
    : room_(otherServer.room_), acceptor_(newContext)
{
}

ChatServer::~ChatServer()
{
}

void ChatServer::do_accept()
{
  acceptor_.async_accept(
      [this](std::error_code ec, tcp::socket socket)
      {
        if (!ec)
        {
          std::make_shared<ChatSession>(std::move(socket), room_)->start();
        }

        do_accept();
      });
}

ChatRoom ChatServer::GetRoom() const
{
    return room_;
}

ChatServer* ChatServer::SetRoom(ChatRoom &otherRoom)
{
    room_ = otherRoom;
    return this;
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
