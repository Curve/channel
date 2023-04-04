<div align="center">

# 🧵 channel

A C++20 library that provides a rust like `std::sync::mpsc::channel` for inter-thread communication.

</div>

# ⚙️ Configuration

## Tests

```cmake
set(channel_tests OFF)
```
> If set to `ON`, *channel* will build tests.

# 📦 Installation

> **Note**  
> This library requires a C++20 capable compiler.
> In case you need support for C++17 checkout commits prior to 6977815409b4c3c02d74a7aee3fc29f01d632feb

- FetchContent
    ```cmake
    include(FetchContent)
    FetchContent_Declare(channel GIT_REPOSITORY "https://github.com/Soundux/channel")

    FetchContent_MakeAvailable(channel)
    target_link_libraries(<YourLibrary> cr::channel)
    ```
- Git Submodule
    ```bash
    git submodule add "https://github.com/Soundux/channel"
    ```
    ```cmake
    # Somewhere in your CMakeLists.txt
    add_subdirectory("<path_to_channel>")
    target_link_libraries(<YourLibrary> cr::channel)
    ```

# 📒 Examples

```cpp
#include <thread>
#include <iostream>
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
std::cout << receiver.recv_as<int>() << std::endl;

// Let's see what that second answer was
std::cout << receiver.recv_as<float>() << std::endl;
```

> **Note**  
> This library also supports methods like `try_recv` and `recv_timeout`.

> For more examples see [tests](tests/)