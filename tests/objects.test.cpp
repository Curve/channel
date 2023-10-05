#include <boost/ut.hpp>
#include <cr/channel.hpp>

#include <string>
#include <thread>

using namespace boost::ut;
using namespace boost::ut::literals;

struct add
{
    int target;
    int to_add;
};

struct lower
{
    std::string string;
};

template <typename T>
struct result
{
    T result;
};

using recipe = cr::recipe<add, lower, result<int>, result<std::string>>;

// NOLINTNEXTLINE
suite<"object"> object_suite = []
{
    auto handler = [](auto receiver, auto sender)
    {
        for (int i = 0; 2 > i; i++)
        {
            receiver.recv(
                [&]<typename T>(const T &arg)
                {
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
    };

    auto [sender, thread_receiver] = cr::channel<recipe>();
    auto [thread_sender, receiver] = cr::channel<recipe>();

    std::jthread thread{handler, std::move(thread_receiver), std::move(thread_sender)};

    sender.send(add{.target = 20, .to_add = 10});
    expect(eq(receiver.recv_as<result<int>>().result, 30));

    sender.send(lower{"TEST"});
    expect(receiver.recv_as<result<std::string>>().result == "test");
};