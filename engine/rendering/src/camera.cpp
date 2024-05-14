#include "Camera.hpp"
#include "Math.hpp"
#include "Profiling.hpp"
#include "Timestep.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

/* Some portions of this class are yoinked from here:
 * https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h */

namespace birb
{
	camera::camera()
	{
		world_up = up;
		update_camera_vectors();
	}

	camera::camera(vec3<f32> position) : position(position.to_glm_vec())
	{
		world_up = up;
		update_camera_vectors();
	}

	camera::camera(vec3<f32> position, f32 yaw, f32 pitch)
	:position(position.to_glm_vec()), yaw(yaw), pitch(pitch)
	{
		world_up = up;
		update_camera_vectors();
	}

	void camera::draw_editor_ui()
	{
		ImGui::DragFloat3("Position", &position[0], 0.25f);
	}

	std::string camera::collapsing_header_name() const
	{
		return editor_header_name;
	}

	glm::mat4 camera::get_view_matrix() const
	{
		return glm::lookAt(position, position + front, up);
	}

	glm::mat4 camera::get_projection_matrix(camera::projection_mode mode, vec2<i32> window_size) const
	{
		ensure(window_size.x > 0, "Invalid window width");
		ensure(window_size.y > 0, "Invalid window height");

		glm::mat4 projection;

		switch (mode)
		{
			case camera::projection_mode::perspective:
				projection = glm::perspective(glm::radians(fov), static_cast<f32>(window_size.x) / static_cast<f32>(window_size.y), near_clip, far_clip);
				break;

			case camera::projection_mode::orthographic:
				f32 width = static_cast<f32>(window_size.x) * orthograhpic_scale;
				f32 height = static_cast<f32>(window_size.y) * orthograhpic_scale;
				projection = glm::ortho(0.0f, width, 0.0f, height, near_clip, far_clip);
				break;
		}

		return projection;
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
		PROFILER_SCOPE_INPUT_FN()

		// Keyboard input
		if (keyboard_controls_enabled)
		{
			if (window.is_key_held(keybinds.up))
				position += front * static_cast<f32>(timestep.deltatime()) * movement_speed;

			if (window.is_key_held(keybinds.left))
				position -= right * static_cast<f32>(timestep.deltatime()) * movement_speed;

			if (window.is_key_held(keybinds.down))
				position -= front * static_cast<f32>(timestep.deltatime()) * movement_speed;

			if (window.is_key_held(keybinds.right))
				position += right * static_cast<f32>(timestep.deltatime()) * movement_speed;
		}

		if (mouse_controls_enabled)
		{
			// Skip processing mouse input if the cursor isn't locked to the window
			// (unless editor mode is active, though with that require mouse3 to be held down)
			if ((window::is_cursor_locked_to_window() || (editor_mode && window.is_key_held(input::keycode::mouse_3))) && !first_mouse_delta_after_lock)
			{
				// Calculate cursor positions
				birb::vec2<f64> new_cursor_pos = window.cursor_pos();
				birb::vec2<f64> cursor_delta = new_cursor_pos - prev_cursor_pos;
				prev_cursor_pos = new_cursor_pos;

				if (!lock_yaw)
					yaw += cursor_delta.x * mouse_sensitivity;

				if (!lock_pitch)
					pitch -= cursor_delta.y * mouse_sensitivity;
			}
			else if ((window::is_cursor_locked_to_window() || (editor_mode && window.is_key_held(input::keycode::mouse_3))) && first_mouse_delta_after_lock)
			{
				// Ignore the first mouse click after locking
				prev_cursor_pos = window.cursor_pos();

				first_mouse_delta_after_lock = false;
			}
			else
			{
				first_mouse_delta_after_lock = true;
			}
		}

		pitch = std::clamp(pitch, -89.0f, 89.0f);

		update_camera_vectors();
	}

	void camera::process_input_ortho(window& window, const timestep& timestep)
	{
		PROFILER_SCOPE_INPUT_FN()

		if (window.is_key_held(keybinds.left))
			position.x -= movement_speed * timestep.deltatime();

		if (window.is_key_held(keybinds.right))
			position.x += movement_speed * timestep.deltatime();

		if (window.is_key_held(keybinds.up))
			position.y += movement_speed * timestep.deltatime();

		if (window.is_key_held(keybinds.down))
			position.y -= movement_speed * timestep.deltatime();
	}

	void camera::update_camera_vectors()
	{
		PROFILER_SCOPE_RENDER_FN()

		// Calculate the front vector
		front = birb::view_vector::front(pitch, yaw);

		// Update the right and up vectors
		right = birb::view_vector::right(front, world_up);
		up = birb::view_vector::up(right, front);
	}

	void camera::zoom(f32 delta)
	{
		position += front * delta;
	}
}
