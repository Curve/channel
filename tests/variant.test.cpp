#include <thread>
#include <numbers>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Receive variant", "[channel]")
{
    using namespace std::chrono_literals;

    auto [sender, receiver] = cr::channel<int, std::string, double>();

    std::jthread t([receiver = std::move(receiver)]() mutable {
        REQUIRE(receiver.recv_as<int>() == 10);
        REQUIRE(std::get<std::string>(receiver.recv()) == "Some message!");
        receiver.recv([]<typename T>(const T &what) {
            REQUIRE(std::is_same_v<std::decay_t<T>, double>);

            if constexpr (std::is_same_v<std::decay_t<T>, double>)
            {
                REQUIRE(what == std::numbers::pi);
            }
        });

        std::this_thread::sleep_for(std::chrono::seconds(5));

        REQUIRE(receiver.try_recv_as<int>() == 10);
        REQUIRE(std::get<std::string>(receiver.try_recv().value()) == "Some message!");

        bool called = false;

        receiver.try_recv([&]<typename T>(const T &what) {
            called = true;
            REQUIRE(std::is_same_v<std::decay_t<T>, double>);

            if constexpr (std::is_same_v<std::decay_t<T>, double>)
            {
                REQUIRE(what == std::numbers::pi);
            }
        });

        REQUIRE(called);

        REQUIRE(receiver.recv_timeout_as<int>(10s) == 10);
        REQUIRE(std::get<std::string>(receiver.recv_timeout(10s).value()) == "Some message!");

        called = false;

        receiver.recv_timeout(
            [&]<typename T>(const T &what) {
                called = true;
                REQUIRE(std::is_same_v<std::decay_t<T>, double>);

                if constexpr (std::is_same_v<std::decay_t<T>, double>)
                {
                    REQUIRE(what == std::numbers::pi);
                }
            },
            10s);

        REQUIRE(called);
    });

    sender.send(10);
    sender.send("Some message!");
    sender.send(std::numbers::pi);

    sender.send(10);
    sender.send("Some message!");
    sender.send(std::numbers::pi);

    std::this_thread::sleep_for(std::chrono::seconds(10));

    sender.send(10);
    sender.send("Some message!");
    sender.send(std::numbers::pi);
}