#define CONFIG_CATCH_MAIN
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Receive single type", "[channel]")
{
    using namespace std::chrono_literals;

    auto [sender, receiver] = cr::channel<std::string>();

    std::jthread t([receiver = std::move(receiver)]() mutable {
        REQUIRE(receiver.recv() == "Some message!");
        std::this_thread::sleep_for(std::chrono::seconds(5));

        REQUIRE(receiver.try_recv() == "Another message!");
        REQUIRE(receiver.recv_timeout(10s) == "Some message!");
    });

    sender.send("Some message!");
    sender.send("Another message!");

    std::this_thread::sleep_for(std::chrono::seconds(10));

    sender.send("Some message!");
}