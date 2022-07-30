#define CONFIG_CATCH_MAIN
#include <string>
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Check if basic communication works", "[channel]")
{
    auto [sender, receiver] = cr::channel<std::string>();

    std::thread t([receiver = std::move(receiver)]() mutable {
        REQUIRE(receiver.receive_as<std::string>() == "Some message!");
        REQUIRE(receiver.receive_as<std::string>() == "Another message!");
    });

    sender.send("Some message!");
    sender.send("Another message!");

    t.join();
}