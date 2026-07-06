<div align="center">

# 🧵 channel

A C++23 library that provides a rust like `std::sync::mpsc::channel` for inter-thread communication.

</div>

# 📦 Installation

> [!NOTE]  
> This library requires a C++23 capable compiler.  
> See `v3.0.1` for C++20 support or commits prior to [6977815](https://github.com/Curve/channel/tree/6977815409b4c3c02d74a7aee3fc29f01d632feb) for C++17 support.

- FetchContent
    ```cmake
    include(FetchContent)
    FetchContent_Declare(channel GIT_REPOSITORY "https://github.com/Curve/channel")

    FetchContent_MakeAvailable(channel)
    target_link_libraries(<YourLibrary> cr::channel)
    ```
- Git Submodule
    ```bash
    git submodule add "https://github.com/Curve/channel"
    ```
    ```cmake
    # Somewhere in your CMakeLists.txt
    add_subdirectory("<path_to_channel>")
    target_link_libraries(<YourLibrary> cr::channel)
    ```

# 📒 Examples

```cpp
#include <print>
#include <thread>

#include <cr/channel.hpp>

auto [sender, receiver] = cr::channel<int, float>();

// Spawn off expensive computations
std::thread t1([sender = std::move(sender)]() mutable
{
    sender.send(expensive_computation_that_returns_int());
    sender.send(another_expensive_computation_that_returns_float());
});

// Do some useful work for awhile

// Let's see what that first answer was
std::println("{}", *receiver.recv_as<int>());

// Let's see what that second answer was
std::println("{}", *receiver.recv_as<float>());
```

> **Note**  
> This library also supports methods like `try_recv` and `try_recv(timeout)`.

> For more examples see [tests](tests/)
