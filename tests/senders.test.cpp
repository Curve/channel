#include <string>
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Multiple senders", "[channel]")
{
    auto [sender, receiver] = cr::channel<std::string>();
    auto sender_clone = sender;

    std::jthread t([receiver = std::move(receiver)]() mutable {
        auto message = receiver.recv();
        REQUIRE((message == "Test-1" || message == "Test-2"));

        message = receiver.recv();
        REQUIRE((message == "Test-1" || message == "Test-2"));
    });

    std::jthread t2([sender = std::move(sender_clone)]() mutable { sender.send("Test-1"); });
    sender.send("Test-2");
}