#pragma once

namespace birb
{
	/**
	 * @class invisible
	 * @brief Entities with this component are ignored during rendering
	 *
	 */
	struct invisible
	{
		/**
		 * This variable is here just to make sure that the struct
		 * doesn't get optimized out
		 */
		const bool hidden = true;
	};
}
