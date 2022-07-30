#include <string>
#include <thread>
#include <cr/channel.hpp>
#include <catch2/catch.hpp>

struct please_add_ten
{
    int value;
};

struct please_to_lower
{
    std::string value;
};

template <typename T> struct result
{
    T result;
};

auto [t_sender, t_receiver] = cr::channel<please_add_ten, please_to_lower>();
auto [sender, receiver] = cr::channel<result<int>, result<std::string>>();

void run_in_thread(decltype(t_receiver) receiver, decltype(sender) sender)
{
    for (int i = 0; 2 > i; i++)
    {
        receiver.receive([&](auto &&arg) {
            using arg_t = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<arg_t, please_add_ten>)
            {
                sender.send<result<int>>({arg.value + 10});
            }
            else if constexpr (std::is_same_v<arg_t, please_to_lower>)
            {
                auto value = arg.value;
                std::transform(value.begin(), value.end(), value.begin(), ::tolower);

                sender.send<result<std::string>>({value});
            }
        });
    }
}

TEST_CASE("Check if objects can be properly exchanged", "[channel]")
{
    std::thread t(run_in_thread, std::move(t_receiver), std::move(sender));

    t_sender.send<please_add_ten>({20});
    REQUIRE(receiver.receive_as<result<int>>().result == 30);

    t_sender.send<please_to_lower>({"TEST"});
    std::this_thread::sleep_for(std::chrono::seconds(10));
    REQUIRE(receiver.receive_as<result<std::string>>().result == "test");

    t.join();
}