#include "Input.hpp"

namespace birb
{
	directional_keys::directional_keys()
	{
		// Use WASD by default
		reset();
	}

	void directional_keys::reset()
	{
		up		= birb::input::keycode::w;
		down	= birb::input::keycode::s;
		left	= birb::input::keycode::a;
		right	= birb::input::keycode::d;
	}

	void directional_keys::vim_preset()
	{
		up		= birb::input::keycode::k;
		down	= birb::input::keycode::j;
		left	= birb::input::keycode::h;
		right	= birb::input::keycode::l;
	}

	void directional_keys::arrow_key_preset()
	{
		up		= birb::input::keycode::up;
		down	= birb::input::keycode::down;
		left	= birb::input::keycode::left;
		right	= birb::input::keycode::right;
	}
}
