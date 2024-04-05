#include "State.hpp"

namespace birb
{
	namespace component
	{
		void state::toggle()
		{
			active = !active;
		}
	}
}
