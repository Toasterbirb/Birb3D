#include "Math.hpp"

namespace birb
{
	namespace view_vector
	{
		glm::vec3 front(const component::transform& transform)
		{
			return front(transform.rotation.x, transform.rotation.y);
		}

		glm::vec3 front(const float pitch, const float yaw)
		{
			return glm::normalize(glm::vec3(
				cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
				sin(glm::radians(pitch)),
				sin(glm::radians(yaw)) * cos(glm::radians(pitch)))
			);
		}

		glm::vec3 right(const glm::vec3& front, const glm::vec3& world_up)
		{
			return glm::normalize(glm::cross(front, world_up));
		}

		glm::vec3 up(const glm::vec3& right, const glm::vec3& front)
		{
			return glm::normalize(glm::cross(right, front));
		}
	}
}
