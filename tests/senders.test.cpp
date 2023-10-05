#include <boost/ut.hpp>
#include <cr/channel.hpp>

#include <string>
#include <thread>

using namespace boost::ut;
using namespace boost::ut::literals;

// NOLINTNEXTLINE
suite<"sender"> sender_suite = []
{
    auto [sender, receiver] = cr::channel<std::string>();
    auto sender_clone       = sender;

    auto _t1 = [](auto receiver)
    {
        expect(receiver.recv().starts_with("Test"));
        expect(receiver.recv().starts_with("Test"));
    };
    std::jthread t1{_t1, std::move(receiver)};

    auto _t2 = [](auto sender)
    {
        sender.send("Test-1");
    };
    std::jthread t2{_t2, std::move(sender_clone)};

    sender.send("Test-2");
};