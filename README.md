<div align="center">

# 🧵 channel

A C++17 library that provides a rust like `std::sync::mpsc::channel` for inter-thread communication.

</div>

# ⚙️ Configuration

## Tests

```cmake
set(channel_tests OFF)
```
> If set to `ON`, *channel* will build tests.

# 📦 Installation

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
std::cout << receiver.receive_as<int>() << std::endl;

// Let's see what that second answer was
std::cout << receiver.receive_as<float>() << std::endl;
```