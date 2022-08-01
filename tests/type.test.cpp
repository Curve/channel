#include <string>
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Check if basic channel can be instantiated from desired type", "[channel]")
{
    using channel_t = cr::channel_t<std::string, std::unique_ptr<int>>;
    auto [sender, receiver] = cr::channel_from<channel_t>();

    std::thread t([receiver = std::move(receiver)]() mutable {
        REQUIRE(receiver.receive_as<std::string>() == "Some message!");
        REQUIRE(receiver.receive_as<std::string>() == "Another message!");
        REQUIRE(*receiver.receive_as<std::unique_ptr<int>>() == 1337);
    });

    sender.send("Some message!");
    sender.send("Another message!");
    sender.send(std::make_unique<int>(1337));

    t.join();
}