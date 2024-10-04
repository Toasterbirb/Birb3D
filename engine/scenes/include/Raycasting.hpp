#pragma once

#include "Scene.hpp"
#include "Types.hpp"
#include "Vector.hpp"

#include <entt.hpp>
#include <glm/vec3.hpp>
#include <optional>

namespace birb
{
	std::optional<entt::entity> raycast_hit(const vec3<f32> ray, scene& scene, const glm::vec3 camera_position);
}
