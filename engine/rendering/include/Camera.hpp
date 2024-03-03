#pragma once

#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <glm/fwd.hpp>

namespace birb
{
	class camera
	{
	public:
		camera();
		explicit camera(vec3<float> position);
		camera(vec3<float> position, float yaw, float pitch);

		glm::mat4 get_view_matrix() const;
		void process_input(window& window, const timestep& timestep);
		void update_camera_vectors();

		glm::vec3 position = { 0.0, 0.0, 0.0 };

		float yaw = -90.0f;
		float pitch = 0.0f;

		float movement_speed = 2.5f;
		float mouse_sensitivity = 0.1f;
		float fov = 45.0f;

		// Keybinds
		directional_keys keybinds;

	private:
		vec2<double> prev_cursor_pos;

		glm::vec3 front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 right;
		glm::vec3 world_up;
	};
}