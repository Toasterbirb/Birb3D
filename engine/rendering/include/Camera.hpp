#pragma once

#include "EditorComponent.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace birb
{
	class timestep;

	class camera : editor_component
	{
	public:
		camera();
		explicit camera(vec3<f32> position);
		camera(vec3<f32> position, f32 yaw, f32 pitch);

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		enum class projection_mode
		{
			perspective,
			orthographic
		};
		glm::mat4 projection_matrix(camera::projection_mode mode, vec2<i32> window_size) const;

		glm::mat4 view_matrix() const;
		glm::vec3 front_vec() const;
		glm::vec3 right_vec() const;
		void process_input(window& window, const timestep& timestep);
		void process_input_ortho(window& window, const timestep& timestep);
		void update_camera_vectors();

		bool keyboard_controls_enabled = true;
		bool mouse_controls_enabled = true;

		glm::vec3 position = { 0.0, 0.0, 0.0 };

		f32 yaw = -90.0f;
		f32 pitch = 0.0f;

		bool lock_yaw = false;
		bool lock_pitch = false;

		f32 movement_speed = 10.0f;
		f32 mouse_sensitivity = 0.1f;
		f32 fov = 45.0f;

		f32 near_clip = 0.1f;
		f32 far_clip = 100.0f;
		f32 orthograhpic_scale = 1.0f;

		bool editor_mode = false;
		void zoom(f32 delta);

		// Keybinds
		directional_keys keybinds;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(position, yaw, pitch, movement_speed, mouse_sensitivity, fov, front, up, right, world_up);
		}

	private:
		static inline const std::string editor_header_name = "Camera";
		vec2<f64> prev_cursor_pos;

		// Ignore the first mouse delta after the window is clicked
		// to lock the mouse to avoid a big jump in view
		bool first_mouse_delta_after_lock = false;

		glm::vec3 front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 right;
		glm::vec3 world_up;
	};
}
