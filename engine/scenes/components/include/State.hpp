#pragma once

namespace birb
{
	struct state
	{
		bool active = true;
		bool invisible = false;

		/**
		 * @brief Toggle the active status of the entity
		 */
		void toggle();

		/**
		 * @brief Check if the entity is meant to be rendered
		 *
		 * @return True if the entity is not invisible and is active
		 */
		__attribute__((always_inline))
		bool should_be_rendered() const
		{
			return active && !invisible;
		}
	};
}
