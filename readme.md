# ZoNiCaL's RPG Project
This is an RPG project that I've been working on for a while, primarily written in C++ with the SDL. It so far can have independent game-states, entities, level loading, and a non-dynamic lighting system. I don't know where this project will go, but we'll see in the future.

## Requirements
This project has many different things that need to be included into the project.
- [SDL 2.0.16 or greater, 32-bit.](https://www.libsdl.org/)
- [SDL_image 2.0.5 or greater, 32-bit.](https://www.libsdl.org/projects/SDL_image/)
- [SDL_ttf 2.0.15 or greater, 32-bit.](https://www.libsdl.org/projects/SDL_ttf/release/)
- [Nholhmann's implementation of JSON for C++.](https://github.com/nlohmann/json)
    - Wherever your store include headers, the `json.hpp` file needs to be stored in a folder called `json`.
- [Python 3.10+](https://www.python.org/)
    - You will need to install the debug libraries if compiling under Debug in Visual Studio. In my build environment I simply just get the include and libraries from my local computer install and link them into my project.
- [Boost.Python](https://www.boost.org/doc/libs/1_75_0/libs/python/doc/html/index.html)
    - In my build environment I statically link Boost.Python. I built my copy of Boost.Python with Visual Studio 2019 Build Tools and the Python 3.10 install.

## Building
This project can be compiled using Microsoft Visual Studio. The minimum platform toolset is Visual Studio 2022 (v143) following the ISO C++20 standard. In my build environment, I build the `.exe` file out to a folder (`/bin`) and then include the `assets` folder as a symbolic link. The game cannot run without the `assets` folder in the same location as the executable. Make sure before you build that you link the SDL libraries before compiling.