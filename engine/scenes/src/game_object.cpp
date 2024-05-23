#include "Entity.hpp"
#include "GameObject.hpp"

namespace birb
{
	game_object::game_object(birb::entity& entity)
	:entity(entity),
	 transform(entity.get_component<birb::transform>()),
	 state(entity.get_component<birb::state>())
	{}
}
