#include <string>
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Check if multiple senders work properly", "[channel]")
{
    auto [sender, receiver] = cr::channel<std::string>();
    auto sender_clone = sender;

    std::thread t([receiver = std::move(receiver)]() mutable {
        auto message = receiver.receive_as<std::string>();
        REQUIRE((message == "Test-1" || message == "Test-2"));

        message = receiver.receive_as<std::string>();
        REQUIRE((message == "Test-1" || message == "Test-2"));
    });

    std::thread t2([sender = std::move(sender_clone)]() mutable { sender.send("Test-1"); });
    sender.send("Test-2");

    t2.join();
    t.join();
}