#include "Vector.hpp"
#include "Window.hpp"

int main(void)
{
	birb::Window window("Empty window", birb::vec2<int>(960, 540));

	while (!window.should_close())
	{
		window.clear();
		window.flip();
		window.poll();
	}
	return 0;
}
