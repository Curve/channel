#include <string>
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

struct add
{
    int target;
    int to_add;
};
struct lower
{
    std::string string;
};
template <typename T> struct result
{
    T result;
};

using recipe = cr::recipe<add, lower, result<int>, result<std::string>>;

void handler(recipe::receiver receiver, recipe::sender sender)
{
    for (int i = 0; 2 > i; i++)
    {
        receiver.recv([&]<typename T>(T &&arg) {
            if constexpr (std::is_same_v<T, add>)
            {
                sender.send(result<int>{arg.target + arg.to_add});
            }
            else if constexpr (std::is_same_v<T, lower>)
            {
                auto value = arg.string;
                std::transform(value.begin(), value.end(), value.begin(), ::tolower);

                sender.send(result<std::string>{value});
            }
        });
    }
}

TEST_CASE("Basic communication with objects", "[channel]")
{
    auto [sender, thread_receiver] = cr::channel<recipe>();
    auto [thread_sender, receiver] = cr::channel<recipe>();

    std::jthread thread(handler, std::move(thread_receiver), std::move(thread_sender));

    sender.send(add{.target = 20, .to_add = 10});
    REQUIRE(receiver.recv_as<result<int>>().result == 30);

    sender.send(lower{"TEST"});
    REQUIRE(receiver.recv_as<result<std::string>>().result == "test");
}