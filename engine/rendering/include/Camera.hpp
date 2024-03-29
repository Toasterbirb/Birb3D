#pragma once

#include "EditorComponent.hpp"
#include "Timestep.hpp"
#include "Vector.hpp"
#include "Window.hpp"

#include <glm/fwd.hpp>

namespace birb
{
	class camera : editor_component
	{
	public:
		camera();
		explicit camera(vec3<float> position);
		camera(vec3<float> position, float yaw, float pitch);

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		enum class projection_mode
		{
			perspective,
			orthographic
		};
		glm::mat4 get_projection_matrix(camera::projection_mode mode, vec2<int> window_size) const;

		glm::mat4 get_view_matrix() const;
		glm::vec3 front_vec() const;
		glm::vec3 right_vec() const;
		void process_input(window& window, const timestep& timestep);
		void process_input_ortho(window& window, const timestep& timestep);
		void update_camera_vectors();

		glm::vec3 position = { 0.0, 0.0, 0.0 };

		float yaw = -90.0f;
		float pitch = 0.0f;

		float movement_speed = 10.0f;
		float mouse_sensitivity = 0.1f;
		float fov = 45.0f;

		float near_clip = 0.1f;
		float far_clip = 100.0f;
		float orthograhpic_scale = 1.0f;

		bool editor_mode = false;
		void zoom(float delta);

		// Keybinds
		directional_keys keybinds;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(position, yaw, pitch, movement_speed, mouse_sensitivity, fov, front, up, right, world_up);
		}

	private:
		const std::string editor_header_name = "Camera";
		vec2<double> prev_cursor_pos;

		// Ignore the first mouse delta after the window is clicked
		// to lock the mouse to avoid a big jump in view
		bool first_mouse_delta_after_lock = false;

		glm::vec3 front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 right;
		glm::vec3 world_up;
	};
}
