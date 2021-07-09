#include <memory>
#include <iostream>
#include <future>
#include <utility>

#include <grpcpp/grpcpp.h>
#include "chat.prot.grpc.pb.h"
#include "build/chat.prot.pb.h"

class ChatClient
{
public:
    ChatClient(std::shared_ptr<grpc::ChannelInterface> channel, std::string nick_name)
        :m_stub(greet::ChatService::NewStub(channel)),
        m_nick_name(std::move(nick_name)){}

    void SendMessage(const std::string & message)
    {
        greet::MessageRequest request;
        request.set_nick_name(m_nick_name);
        request.set_message(message);

        grpc::ClientContext context;
        greet::MessageReply response;
        grpc::Status status = m_stub->SendMessage(&context, request, &response);
        if (!status.ok())
        {
            std::cerr << "SendMessage failed: " << status.error_code() << ", " << status.error_message() << std::endl;
        }
    }

    std::future<void> StartMessageFeedWorker()
    {
        return std::async(std::launch::async, [this]{HandleMessageFeed();});
    }

private:
    void HandleMessageFeed()
    {
        greet::MessageFeedRequest request;
        request.set_nick_name(m_nick_name);
        grpc::ClientContext context;
        std::unique_ptr<grpc::ClientReader<greet::MessageFeedResponse>> stream = m_stub->MessageFeed(&context, request);

        greet::MessageFeedResponse response;
        while (stream->Read(&response))
        {
            std::cout << "(" << response.time_received() << ") "
                << response.nick_name() << ": "
                << response.message() << std::endl;
        }
    }
private:
    std::unique_ptr<greet::ChatService::Stub> m_stub;
    std::string m_nick_name;
};


constexpr char CHAT_SOCKET [] {"localhost:80"};

int main()
{
    // Create channel
    auto chat_channel = grpc::CreateChannel(CHAT_SOCKET, grpc::InsecureChannelCredentials());
    // Create chat client
    ChatClient client(chat_channel, "Oleksandr Dehtiar");
    // Start messages receiving
    auto msg_feed_end = client.StartMessageFeedWorker();
    // Start user input
    while (msg_feed_end.valid())
    {
        std::string message;
        std::cout << "> ";
        std::getline(std::cin, message);
        client.SendMessage(message);
    }

    // Block until Message Feed stream end
    msg_feed_end.get();
    return 0;
}
