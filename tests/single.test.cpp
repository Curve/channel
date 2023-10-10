#include <boost/ut.hpp>
#include <cr/channel.hpp>

#include <thread>

using namespace boost::ut;
using namespace boost::ut::literals;
using namespace std::chrono_literals;

// NOLINTNEXTLINE
suite<"single"> single_suite = []
{
    auto [sender, receiver] = cr::channel<std::string>();

    auto _t1 = [](auto receiver)
    {
        expect(receiver.recv() == "Some message!");
        expect(receiver.try_recv() == "Another message!");
        expect(receiver.recv_timeout(15s) == "Some message!");
    };

    std::jthread t1{_t1, std::move(receiver)};

    sender.send("Some message!");
    sender.send("Another message!");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    sender.send("Some message!");
};
