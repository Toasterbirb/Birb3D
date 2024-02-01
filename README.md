# Birb3D

Birb3D is supposed to be the successor for [Birb2D](http://birbgitfh224rep6tmdofmr6qlo6wx43umqzt3hjubnncr55sdlfmtad.onion/toasterbirb/Birb2D). Birb2D was written with 2D games in mind and it used SDL for pretty much everything. The goal of Birb3D is to add in the third dimension and use OpenGL directly for drawing things. While Birb2D worked for simple games, it didn't scale too well when the projects got more complicated. Also the fact that it skipped ECS entirely made adding new features very laborious.

TLDR: Another never ending hobbyist game engine project

## Building
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

## Installation
To install Birb3D to /usr/local/bin, run the following command
```sh
make install
```
You can customize the installation *PREFIX* and *DESTDIR* variables normally with cmake and make.
