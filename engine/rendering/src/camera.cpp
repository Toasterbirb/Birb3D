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

	glm::vec3 camera::front_vec() const
	{
		return front;
	}

	glm::vec3 camera::right_vec() const
	{
		return right;
	}

	void camera::process_input(window& window, const timestep& timestep)
	{
		// Keyboard input
		if (window.is_key_held(keybinds.up))
			position += front * static_cast<float>(timestep.deltatime()) * movement_speed;

		if (window.is_key_held(keybinds.left))
			position -= right * static_cast<float>(timestep.deltatime()) * movement_speed;

		if (window.is_key_held(keybinds.down))
			position -= front * static_cast<float>(timestep.deltatime()) * movement_speed;

		if (window.is_key_held(keybinds.right))
			position += right * static_cast<float>(timestep.deltatime()) * movement_speed;

		// Skip processing mouse input if the cursor isn't locked to the window
		// (unless editor mode is active, though with that require mouse3 to be held down)
		if ((window::is_cursor_locked_to_window() || (editor_mode && window.is_key_held(input::keycode::MOUSE_3))) && !first_mouse_delta_after_lock)
		{
			// Calculate cursor positions
			birb::vec2<double> new_cursor_pos = window.cursor_pos();
			birb::vec2<double> cursor_delta = new_cursor_pos - prev_cursor_pos;
			prev_cursor_pos = new_cursor_pos;

			yaw += cursor_delta.x * mouse_sensitivity;
			pitch -= cursor_delta.y * mouse_sensitivity;
		}
		else if ((window::is_cursor_locked_to_window() || (editor_mode && window.is_key_held(input::keycode::MOUSE_3))) && first_mouse_delta_after_lock)
		{
			// Ignore the first mouse click after locking
			prev_cursor_pos = window.cursor_pos();

			first_mouse_delta_after_lock = false;
		}
		else
		{
			first_mouse_delta_after_lock = true;
		}

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

	void camera::zoom(float delta)
	{
		position += front * delta;
	}
}
