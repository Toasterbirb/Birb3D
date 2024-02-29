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
		up		= birb::input::keycode::W;
		down	= birb::input::keycode::S;
		left	= birb::input::keycode::A;
		right	= birb::input::keycode::D;
	}

	void directional_keys::vim_preset()
	{
		up		= birb::input::keycode::K;
		down	= birb::input::keycode::J;
		left	= birb::input::keycode::H;
		right	= birb::input::keycode::L;
	}

	void directional_keys::arrow_key_preset()
	{
		up		= birb::input::keycode::UP;
		down	= birb::input::keycode::DOWN;
		left	= birb::input::keycode::LEFT;
		right	= birb::input::keycode::RIGHT;
	}
}
