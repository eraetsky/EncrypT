#include "ChatMessage.hpp"
#include "ChatClient.hpp"

ChatClient::ChatClient(asio::io_context &io_context,
                       const tcp::resolver::results_type &endpoints)
    : io_context_(io_context),
      socket_(io_context)
{
  do_connect(endpoints);
}

ChatClient::ChatClient(const ChatClient &otherClient)
    : read_msg_(otherClient.read_msg_), write_msgs_(otherClient.write_msgs_),
      socket_(otherClient.io_context_), io_context_(otherClient.io_context_)
{
}

ChatClient::~ChatClient()
{
}

ChatClient::ChatClient()
    : read_msg_(), write_msgs_(), io_context_(asio::io_context()), socket_(io_context_)
{
}

void ChatClient::write(const ChatMessage &msg)
{
  asio::post(io_context_,
             [this, msg]()
             {
               bool write_in_progress = !write_msgs_.empty();
               write_msgs_.push_back(msg);
               if (!write_in_progress)
               {
                 do_write();
               }
             });
}

void ChatClient::close()
{
  asio::post(io_context_, [this]()
             { socket_.close(); });
}

asio::io_context &ChatClient::get_context() const
{
  return io_context_;
}

ChatMessage ChatClient::get_read_msg() const
{
  return read_msg_;
}

ChatClient *ChatClient::set_read_msg(ChatMessage &newReadMsg)
{
  read_msg_ = newReadMsg;
  return this;
}

ChatMessageQueue ChatClient::get_write_msgs() const
{
  return write_msgs_;
}

ChatClient *ChatClient::set_write_msgs(const ChatMessageQueue &newWriteMsgs)
{
  write_msgs_ = newWriteMsgs;
  return this;
}

void ChatClient::do_connect(const tcp::resolver::results_type &endpoints)
{
  asio::async_connect(socket_, endpoints,
                      [this](std::error_code ec, tcp::endpoint)
                      {
                        if (!ec)
                        {
                          do_read_header();
                        }
                      });
}

void ChatClient::do_read_header()
{
  asio::async_read(socket_,
                   asio::buffer(read_msg_.data(), ChatMessage::header_length),
                   [this](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec && read_msg_.decode_header())
                     {
                       do_read_body();
                     }
                     else
                     {
                       socket_.close();
                     }
                   });
}

void ChatClient::do_read_body()
{
  asio::async_read(socket_,
                   asio::buffer(read_msg_.body(), read_msg_.body_length()),
                   [this](std::error_code ec, std::size_t /*length*/)
                   {
                     if (!ec)
                     {
                       std::cout.write(read_msg_.body(), read_msg_.body_length());
                       std::cout << "\n";
                       do_read_header();
                     }
                     else
                     {
                       socket_.close();
                     }
                   });
}

void ChatClient::do_write()
{
  asio::async_write(socket_,
                    asio::buffer(write_msgs_.front().data(),
                                 write_msgs_.front().length()),
                    [this](std::error_code ec, std::size_t /*length*/)
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
      std::cerr << "Usage: ChatClient <host> <port>\n";
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
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();
  }
  catch (std::exception &e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
