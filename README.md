# Birb3D

Birb3D is supposed to be the successor for [Birb2D](http://birbgitfh224rep6tmdofmr6qlo6wx43umqzt3hjubnncr55sdlfmtad.onion/toasterbirb/Birb2D). Birb2D was written with 2D games in mind and it used SDL for pretty much everything. The goal of Birb3D is to add in the third dimension and use OpenGL directly for drawing things. While Birb2D worked for simple games, it didn't scale too well when the projects got more complicated. Also the fact that it skipped ECS entirely made adding new features very laborious.

TLDR: Another never ending hobbyist game engine project

## Building

### Dependencies
Vendored dependencies that are included as submodules or directly in the repository:
- [Dear ImGui](https://github.com/ocornut/imgui/tree/docking)
- [EnTT](https://github.com/skypjack/entt)
- [GLAD](https://github.com/Dav1dde/glad)
- [GLFW](https://www.glfw.org/)
- [assimp](https://github.com/assimp/assimp)
- [cereal](https://github.com/USCiLab/cereal)
- [doctest](https://github.com/doctest/doctest)
- [libsndfile](https://github.com/libsndfile/libsndfile)
- [microprofile](https://github.com/jonasmr/microprofile)
- [nlohmann::json](https://github.com/nlohmann/json)
- [openal-soft](https://github.com/kcat/openal-soft)
- [stb](https://github.com/nothings/stb)

Optional dependencies:
- [doxygen](https://www.doxygen.nl/index.html) for building documentation

### How to compile
Build the project with cmake by running the following commands
```sh
mkdir build
cd build
cmake ..
make -j$(nproc)
```

You can build and run tests by running the following command in the build directory
```sh
make -j$(nproc) test
```

You can also build the documentation with `make docs`

#### Cross-compilation
To target the Windows platform, you can build a game project with the following commands (assuming Birb3D is a sub-project and symlinked at the root of the game project)
```sh
mkdir build_win
cmake .. -DBIRB_STATIC=ON -DBIRB_WINDOWS=ON -DBIRB_PROFILER=OFF -DCMAKE_TOOLCHAIN_FILE=../Birb3D/x86_64-w64-mingw32.cmake -DCMAKE_CXX_FLAGS="-std=c++20 -static -static-libstdc++"
make -j$(nproc)
```

Some DLL files may need to be moved around to fix any missing DLL errors that may appear. A release build script might be made in the future to automate this process.

> **Warning**
> Building the engine by itself for Windows will not be supported officially and you may face DLL errors when trying to run the built executables. An effort will be made though to keep the project building with mingw to keep the cross-compiling of games functional

To build the Birb3D project itself, you need to adjust the file paths a bit
```sh
mkdir build_win
cmake .. -DBIRB_WINDOWS=ON -DBIRB_PROFILER=OFF -DCMAKE_TOOLCHAIN_FILE=../x86_64-w64-mingw32.cmake -DCMAKE_CXX_FLAGS="-std=c++20"
make -j$(nproc)
```


## Installation
To install Birb3D to /usr/local/bin, run the following command
```sh
make install
```
You can customize the installation *PREFIX* and *DESTDIR* variables normally with cmake and make.

## Resources
Awesome websites/people/resources etc. etc. that notably helped with the creation of this project

- [learnopengl.com](https://learnopengl.com)
- [OpenGL Course - Create 3D and 2D Graphics With C++](https://www.youtube.com/watch?v=45MIykWJ-C4&t=1716)
- [GLFW documentation](https://www.glfw.org/documentation.html)
- [Vulkan Game Engine Series by Travis Vroman](https://www.youtube.com/watch?v=teg23SJlyl8)
- [The Cherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)
- StackOverflow
