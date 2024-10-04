#include "Profiling.hpp"
#include "RaycastTarget.hpp"
#include "Raycasting.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace birb
{
	f32 squared_distance(const glm::vec3 a, const glm::vec3 b);

	f32 squared_distance(const glm::vec3 a, const glm::vec3 b)
	{
		return
			std::pow(a.x - b.x, 2) +
			std::pow(a.y - b.y, 2) +
			std::pow(a.z - b.z, 2);
	}

	std::optional<entt::entity> raycast_hit(const vec3<f32> ray, scene& scene, const glm::vec3 camera_position)
	{
		PROFILER_SCOPE_MISC_FN();

		std::optional<entt::entity> hit;
		const auto view = scene.registry.view<raycast_target>();

		for (const auto& entity : view)
		{
			const raycast_target target = view.get<raycast_target>(entity);

			// calculate the distance from the camera to the target
			const f32 distance_from_camera_to_sphere = squared_distance(camera_position, target.position.to_glm_vec());

			const f32 t = glm::dot((target.position.to_glm_vec() - camera_position), ray.to_glm_vec());
			const glm::vec3 p = camera_position + t * ray.to_glm_vec();

			const f32 p2 = squared_distance(p, target.position.to_glm_vec());

			if (p2 <= target.radius * target.radius)
			{
				hit = entity;
				return hit;
			}
		}

		return hit;
	}
}
