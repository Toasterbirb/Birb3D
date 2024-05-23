#pragma once

#include "Entity.hpp"
#include "State.hpp"
#include "Transform.hpp"

namespace birb
{
	class scene;

	class game_object
	{
	public:
		game_object(birb::entity& entity);
		~game_object() = default;
		game_object(const game_object&) = default;
		game_object(game_object&) = default;

		birb::entity& entity;
		birb::transform& transform;
		birb::state& state;

	private:
	};
}
