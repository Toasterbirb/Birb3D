#include "Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace birb
{
	transform::transform()
	:position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), local_scale(1.0f, 1.0f, 1.0f)
	{}

	void transform::draw_editor_ui()
	{
		ImGui::DragFloat3("Position", *position.to_ptr_array().data(), 0.25f);
		ImGui::DragFloat3("Rotation", *rotation.to_ptr_array().data(), 0.25f);
		ImGui::Separator();
		ImGui::DragFloat3("Local scale", *local_scale.to_ptr_array().data(), 0.1f);
		if (ImGui::DragFloat("Combined", &local_scale.x, 0.1f))
		{
			local_scale.y = local_scale.x;
			local_scale.z = local_scale.x;
		}
	}

	std::string transform::collapsing_header_name() const
	{
		return editor_header_name;
	}

	glm::mat4 transform::model_matrix() const
	{
		if (_is_locked)
			return cached_model_matrix;

		glm::mat4 model_matrix = glm::mat4(1.0f);
		model_matrix = glm::translate(model_matrix, position.to_glm_vec());

		const glm::vec3 euler_angles({glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)});
		const glm::quat quaternion(euler_angles);
		const glm::mat4 rotation_matrix(quaternion);
		model_matrix = model_matrix * rotation_matrix;

		model_matrix = glm::scale(model_matrix, local_scale.to_glm_vec());

		return model_matrix;
	}

	void transform::translate(f32 x, f32 y, f32 z)
	{
		position.x += x;
		position.y += y;
		position.z += z;
	}

	void transform::rotate(f32 x, f32 y, f32 z)
	{
		rotation.x += x;
		rotation.y += y;
		rotation.z += z;
	}

	void transform::scale(f32 x, f32 y, f32 z)
	{
		local_scale.x *= x;
		local_scale.y *= y;
		local_scale.z *= z;
	}

	void transform::lock()
	{
		cached_model_matrix = model_matrix();
		_is_locked = true;
	}

	void transform::unlock()
	{
		_is_locked = false;
	}

	bool transform::is_locked() const
	{
		return _is_locked;
	}
}
