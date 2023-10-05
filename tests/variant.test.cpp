#include <boost/ut.hpp>
#include <cr/channel.hpp>

#include <thread>
#include <numbers>

using namespace boost::ut;
using namespace boost::ut::literals;
using namespace std::chrono_literals;

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// NOLINTNEXTLINE
suite<"variant"> variant_suite = []
{
    auto [sender, receiver] = cr::channel<int, std::string, double>();

    auto _t1 = [](decltype(receiver) receiver)
    {
        expect(receiver.recv_as<int>() == 10);
        expect(std::get<std::string>(receiver.recv()) == "Some message!");

        receiver.recv(
            []<typename T>(const T &what)
            {
                expect(std::is_same_v<std::decay_t<T>, double>);

                if constexpr (std::is_same_v<T, double>)
                {
                    expect(what == std::numbers::pi);
                }
            });

        std::this_thread::sleep_for(std::chrono::seconds(2));

        expect(receiver.try_recv_as<int>() == 10);
        expect(std::get<std::string>(receiver.try_recv().value()) == "Some message!");

        bool called = false;

        receiver.try_recv(overloaded{[&](double what)
                                     {
                                         expect(what == std::numbers::pi);
                                         called = true;
                                     },
                                     [](auto &&) {
                                     }});

        expect(called);

        expect(receiver.recv_timeout_as<int>(10s) == 10);
        expect(std::get<std::string>(receiver.recv_timeout(10s).value()) == "Some message!");

        called = false;

        receiver.recv_timeout(overloaded{[&](double what)
                                         {
                                             expect(what == std::numbers::pi);
                                             called = true;
                                         },
                                         [](auto &&) {
                                         }},
                              10s);

        expect(called);
    };

    std::jthread t1{_t1, std::move(receiver)};

    sender.send(10);
    sender.send("Some message!");
    sender.send(std::numbers::pi);

    sender.send(10);
    sender.send("Some message!");
    sender.send(std::numbers::pi);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    sender.send(10);
    sender.send("Some message!");
    sender.send(std::numbers::pi);
};