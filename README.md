
# RPG Maker Engine
This is the original source code of a simple 2D game engine developed in 2022 while I was serving in military.

## Story behind RPGME
These days, we can use Unreal Engine, one of the most powerful game engines on the planet, with just a single click of a button. However, due to the nature of the military, we cannot even use tools that are easily available on the Internet. All that was left on the computer were the tools used by developers in the past who were disbanded a long time ago: The compiler, STL, and Dev-C++ IDE that stopped in 2004.

The absence of tools makes people more obsessive and tenacious. At first, I simply used these tools to learn data structure and algorithm. However, when I came across Dev-C++ samples using the win32 API, I was able to create Windows applications. I even found samples using OpenGL, and I thought, 'Maybe I can do more with this.'

This was the first meaningful program I wrote after studying C++ in my life. I hope you understand that it was a program created in a poor environment and with a very poor understanding of C++.

BTW, the reason it's called RPG Maker Engine is because it was initially intended to be a tool for creating tile-based RPG games. As I mentioned earlier, I saw the example of Dev-C++ and thought it would be possible to create a more general-purpose game engine. Although it expanded into a general-purpose 2D engine during development, I didn't really change the name.


### Develop envirionment
- Dev-C++ IDE 4.9.9.2
- GCC, G++ mingw32 3.4.2 (September 6, 2004)
- STL from mingw32
- OpenGL headers 1.2

### Features
- Render and Logic threads are separated to ensure update precision
	- Logic thread use `QueryPerformanceFrequency` while render thread use  `WM_TIMER` event
	- Render rate and Logic update rate are also separated
- Support affine transform of `SceneObject`
	- Translate, rotate and scale
	- An actual object size and texture size are seperated parameters and users are free to set either one as a size reference
- `SceneObject` can have parent-child relation
	- Transforming a parent object also affects all child objects
	- Each object can have one of its center or corners set as an anchor
- Support animated texture
- Support text KR and EN with `TextObject` 
- Key input API provided
	- `Ctrl`, `Alt` and `Shift` modifier
	- Provides distinct state `down` and `pressed` for whether the input is continuously pressed or a single press-release
- Support `Scene`
	- Each `Scene` can have inter-scene transition function that performs loading or transition animation stuff
	- GameEngine can only run one scene at a time
- Support named texture and named `Scene`
	- Texture and `Scene` can be loaded with given alias instead of `[uint] uid`
