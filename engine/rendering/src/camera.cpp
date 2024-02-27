#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* Some portions of this class are yoinked from here:
 * https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h */

namespace birb
{
	camera::camera()
	{
		world_up = up;
		update_camera_vectors();
	}

	camera::camera(vec3<float> position) : position(position.to_glm_vec())
	{
		world_up = up;
		update_camera_vectors();
	}

	camera::camera(vec3<float> position, float yaw, float pitch)
	:position(position.to_glm_vec()), yaw(yaw), pitch(pitch)
	{
		world_up = up;
		update_camera_vectors();
	}

	glm::mat4 camera::get_view_matrix() const
	{
		return glm::lookAt(position, position + front, up);
	}

	void camera::process_input(window& window, const timestep& timestep)
	{
		// Keyboard input
		if (window.is_key_held(birb::input::keycode::W))
			position += front * static_cast<float>(timestep.deltatime()) * movement_speed;

		if (window.is_key_held(birb::input::keycode::A))
			position -= right * static_cast<float>(timestep.deltatime()) * movement_speed;

		if (window.is_key_held(birb::input::keycode::S))
			position -= front * static_cast<float>(timestep.deltatime()) * movement_speed;

		if (window.is_key_held(birb::input::keycode::D))
			position += right * static_cast<float>(timestep.deltatime()) * movement_speed;

		// Calculate cursor positions
		birb::vec2<double> new_cursor_pos = window.cursor_pos();
		birb::vec2<double> cursor_delta = new_cursor_pos - prev_cursor_pos;
		prev_cursor_pos = new_cursor_pos;

		yaw += cursor_delta.x * mouse_sensitivity;
		pitch -= cursor_delta.y * mouse_sensitivity;

		pitch = std::clamp(pitch, -89.0f, 89.0f);

		update_camera_vectors();
	}

	void camera::update_camera_vectors()
	{
		// Calculate the front vector
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		// Update the right and up vectors
		right = glm::normalize(glm::cross(front, world_up));
		up = glm::normalize(glm::cross(right, front));
	}
}
