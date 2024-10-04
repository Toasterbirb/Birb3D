#include "Camera.hpp"
#include "Logger.hpp"
#include "Math.hpp"
#include "Profiling.hpp"
#include "ShaderUniforms.hpp"
#include "Timestep.hpp"

#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

/* Some portions of this class are yoinked from here:
 * https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h */

namespace birb
{
	camera::camera(const vec2<i32> window_size)
	:projection_matrix_ubo(shader_uniforms::block::projection_matrices)
	{
		event_bus::register_event_id(event::window_resized, this);

		world_up = up;
		mode = mode::flying;
		update_camera_vectors();
		update_projection_matrices(window_size);
	}

	camera::camera(const vec3<f32> position, const vec2<i32> window_size)
	:position(position.to_glm_vec()), projection_matrix_ubo(shader_uniforms::block::projection_matrices)
	{
		event_bus::register_event_id(event::window_resized, this);

		world_up = up;
		mode = mode::flying;
		update_camera_vectors();
		update_projection_matrices(window_size);
	}

	camera::camera(const vec3<f32> position, const f32 yaw, const f32 pitch, const vec2<i32> window_size)
	:position(position.to_glm_vec()), yaw(yaw), pitch(pitch),
	 projection_matrix_ubo(shader_uniforms::block::projection_matrices)
	{
		event_bus::register_event_id(event::window_resized, this);

		world_up = up;
		mode = mode::flying;
		update_camera_vectors();
		update_projection_matrices(window_size);
	}

	camera::~camera()
	{
		event_bus::unregister_event_id(event::window_resized, this);
	}

	void camera::process_event(u16 event_id, const event_data& data)
	{
		switch (event_id)
		{
			case (event::window_resized):
				update_projection_matrices(birb::vec2<i32>(data._i32[0], data._i32[1]));
				break;
		}
	}

	void camera::draw_editor_ui()
	{
		static i32 current_mode = 0;
		if (ImGui::Combo("Camera type", &current_mode, "Flying\0FPS\0Editor\0"))
		{
			editor_mode = false;
			switch (current_mode)
			{
				case 0: mode = mode::flying; break;
				case 1: mode = mode::fps; break;
				case 2: mode = mode::flying; editor_mode = true; break;
			}
		}

		ImGui::Checkbox("Keyboard control", &keyboard_controls_enabled);
		ImGui::Checkbox("Mouse control", &mouse_controls_enabled);

		if (ImGui::Button("Lock cursor to window"))
			event_bus::send_event(event::window_lock_cursor);

		if (ImGui::Button("Log position and rotation"))
		{
			birb::log("Position: ", position.x, ", ", position.y, ", ", position.z);
			birb::log("Pitch: ", pitch);
			birb::log("Yaw: ", yaw);
		}

		ImGui::Separator();
		ImGui::Spacing();

		ImGui::DragFloat3("Position", &position[0], 0.25f);
		ImGui::Spacing();
		ImGui::DragFloat("Yaw", &yaw, 0.25f);
		ImGui::DragFloat("Pitch", &pitch, 0.25f);
		ImGui::Spacing();
		ImGui::Checkbox("Lock yaw", &lock_yaw);
		ImGui::Checkbox("Lock pitch", &lock_pitch);
		ImGui::Spacing();
		ImGui::DragFloat("Movement speed", &movement_speed, 0.25f);
		ImGui::DragFloat("Mouse sensitivity", &mouse_sensitivity, 0.25f);

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("FOV: %.2f", fov);
		ImGui::Text("Orthographic scale: %.2f", orthographic_scale);
		ImGui::Text("Near clip: %.2f", near_clip);
		ImGui::Text("Far clip: %.2f", far_clip);
		ImGui::Text("Previous cursor position: [%d, %d]", static_cast<int>(prev_cursor_pos.x), static_cast<int>(prev_cursor_pos.y));

		if (ImGui::CollapsingHeader("Vectors"))
		{
			ImGui::Text("Front:     [%.2f, %.2f, %.2f]", front.x, front.y, front.z);
			ImGui::Text("Front FPS: [%.2f, %.2f, %.2f]", front_fps.x, front_fps.y, front_fps.z);
			ImGui::Text("Right:     [%.2f, %.2f, %.2f]", right.x, right.y, right.z);
			ImGui::Text("Up:        [%.2f, %.2f, %.2f]", up.x, up.y, up.z);
			ImGui::Text("World up:  [%.2f, %.2f, %.2f]", world_up.x, world_up.y, world_up.z);
		}
	}

	std::string camera::collapsing_header_name() const
	{
		return editor_header_name;
	}

	glm::mat4 camera::perspective_projection_matrix() const
	{
		return cached_projection_matrix_perspective;
	}

	glm::mat4 camera::orthographic_projection_matrix() const
	{
		return cached_projection_matrix_ortho;
	}

	glm::mat4 camera::orthographic_no_near_clip_projection_matrix() const
	{
		return cached_projection_matrix_ortho_no_clipping;
	}

	glm::mat4 camera::view_matrix() const
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
		PROFILER_SCOPE_INPUT_FN();

		// Keyboard input
		if (keyboard_controls_enabled)
		{
			switch (mode)
			{
				case mode::flying:
				{
					if (window.is_key_held(keybinds.up))
						position += front * static_cast<f32>(timestep.deltatime()) * movement_speed;

					if (window.is_key_held(keybinds.left))
						position -= right * static_cast<f32>(timestep.deltatime()) * movement_speed;

					if (window.is_key_held(keybinds.down))
						position -= front * static_cast<f32>(timestep.deltatime()) * movement_speed;

					if (window.is_key_held(keybinds.right))
						position += right * static_cast<f32>(timestep.deltatime()) * movement_speed;

					break;
				}

				case mode::fps:
				{
					if (window.is_key_held(keybinds.up))
						position += front_fps * static_cast<f32>(timestep.deltatime()) * movement_speed;

					if (window.is_key_held(keybinds.left))
						position -= right * static_cast<f32>(timestep.deltatime()) * movement_speed;

					if (window.is_key_held(keybinds.down))
						position -= front_fps * static_cast<f32>(timestep.deltatime()) * movement_speed;

					if (window.is_key_held(keybinds.right))
						position += right * static_cast<f32>(timestep.deltatime()) * movement_speed;

					break;
				}
			}
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
		PROFILER_SCOPE_INPUT_FN();

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
		PROFILER_SCOPE_RENDER_FN();

		// Calculate the front vector
		front = birb::view_vector::front(pitch, yaw);
		if (mode == mode::fps)
			front_fps = birb::view_vector::front(0.0f, yaw);

		// Update the right and up vectors
		right = birb::view_vector::right(front, world_up);
		up = birb::view_vector::up(right, front);
	}

	void camera::update_projection_matrices(const vec2<i32> window_size)
	{
		ensure(window_size.x > 0, "Invalid window width");
		ensure(window_size.y > 0, "Invalid window height");

		cached_projection_matrix_perspective = glm::perspective(glm::radians(fov), static_cast<f32>(window_size.x) / static_cast<f32>(window_size.y), near_clip, far_clip);

		const f32 width = (static_cast<f32>(window_size.x) / static_cast<f32>(window_size.y)) * orthographic_scale;
		const f32 height = orthographic_scale;

		cached_projection_matrix_ortho = glm::ortho(0.0f, width, 0.0f, height, near_clip, far_clip);
		cached_projection_matrix_ortho_no_clipping = glm::ortho(0.0f, width, 0.0f, height, 0.0f, far_clip);

		// Update the projection matrix shader uniforms
		projection_matrix_ubo.update_data(glm::value_ptr(cached_projection_matrix_perspective), sizeof(glm::mat4), 0);
		projection_matrix_ubo.update_data(glm::value_ptr(cached_projection_matrix_ortho), sizeof(glm::mat4), sizeof(glm::mat4) * 1);
		projection_matrix_ubo.update_data(glm::value_ptr(cached_projection_matrix_ortho_no_clipping), sizeof(glm::mat4), sizeof(glm::mat4) * 2);
	}

	void camera::zoom(f32 delta)
	{
		position += front * delta;
	}
}
