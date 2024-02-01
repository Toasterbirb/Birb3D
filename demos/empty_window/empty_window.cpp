#include "Vector.hpp"
#include "Window.hpp"

int main(void)
{
	birb::window window("Empty window", birb::vec2<unsigned int>(960, 540));

	while (!window.should_close())
	{
		window.clear();
		window.flip();
		window.poll();
	}
	return 0;
}
