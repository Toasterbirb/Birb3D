# Birb3D

Birb3D is supposed to be the successor for [Birb2D](http://birbgitfh224rep6tmdofmr6qlo6wx43umqzt3hjubnncr55sdlfmtad.onion/toasterbirb/Birb2D). Birb2D was written with 2D games in mind and it used SDL for pretty much everything. The goal of Birb3D is to add in the third dimension and use OpenGL directly for drawing things. While Birb2D worked for simple games, it didn't scale too well when the projects got more complicated. Also the fact that it skipped ECS entirely made adding new features very laborious.

TLDR: Another never ending hobbyist game engine project

Visit the [wiki](http://birbgitfh224rep6tmdofmr6qlo6wx43umqzt3hjubnncr55sdlfmtad.onion/toasterbirb/Birb3D/wiki) for more documentation.

> **Note**
> To access the wiki, you may need to use the [Tor browser](https://www.torproject.org/) or some other alternative way to browse .onion sites

## Building
Build the project with cmake by running the following commands
```sh
mkdir build
cd build
cmake ..
make -j$(nproc)
```

For more information about the build process and dependencies, check the [wiki](http://birbgitfh224rep6tmdofmr6qlo6wx43umqzt3hjubnncr55sdlfmtad.onion/toasterbirb/Birb3D/wiki).


## Credits
- [Quartzi](https://github.com/realQuartzi), the creator of the fancy logo

## Resources
Awesome websites/people/resources etc. etc. that notably helped with the creation of this project

- [learnopengl.com](https://learnopengl.com)
- [OpenGL Course - Create 3D and 2D Graphics With C++](https://www.youtube.com/watch?v=45MIykWJ-C4&t=1716)
- [GLFW documentation](https://www.glfw.org/documentation.html)
- [Vulkan Game Engine Series by Travis Vroman](https://www.youtube.com/watch?v=teg23SJlyl8)
- [The Cherno](https://www.youtube.com/channel/UCQ-W1KE9EYfdxhL6S4twUNw)
- StackOverflow
