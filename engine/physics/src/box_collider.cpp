#include "BoxCollider.hpp"
#include "Transform.hpp"

#include <imgui.h>

namespace birb
{
	namespace collider
	{
		box::box() {}

		box::box(const transform& transform)
		:_size(transform.local_scale), _position(transform.position)
		{
			update_min_max_values();
		}

		void box::draw_editor_ui()
		{
			static vec3<f32> new_size = _size;
			ImGui::Text("Size: [%.2f, %.2f, %.2f]", _size.x, _size.y, _size.z);
			ImGui::InputFloat3("Size", *new_size.to_ptr_array().data());
			if (ImGui::Button("Set"))
				set_size(new_size);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			static vec3<f32> new_position = _position;
			ImGui::Text("Position: [%.2f, %.2f, %.2f]", _position.x, _position.y, _position.z);
			ImGui::InputFloat3("Position", *new_position.to_ptr_array().data());
			if (ImGui::Button("Set"))
				set_position(new_position);

			ImGui::Spacing();
			ImGui::Separator();
			if (ImGui::Button("Update input fields"))
			{
				new_size = _size;
				new_position = _position;
			}
		}

		std::string box::collapsing_header_name() const
		{
			return editor_header_name;
		}

		bool box::collides_with(const box& box) const
		{
			// AABB algorithm

			return	(this->min().x <= box.max().x && this->max().x >= box.min().x) &&
					(this->min().y <= box.max().y && this->max().y >= box.min().y) &&
					(this->min().z <= box.max().z && this->max().z >= box.min().z);
		}

		void box::set_position(const vec3<f32>& position)
		{
			this->_position = position;
			update_min_max_values();
		}

		void box::set_size(const vec3<f32>& size)
		{
			this->_size = size;
			update_min_max_values();
		}

		void box::set_position_and_size(const vec3<f32>& position, const vec3<f32>& size)
		{
			this->_position = position;
			this->_size = size;
			update_min_max_values();
		}

		void box::set_position_and_size(const transform& transform)
		{
			this->_position = transform.position;
			this->_size = transform.local_scale;
			update_min_max_values();
		}

		vec3<f32> box::size() const
		{
			return _size;
		}

		vec3<f32> box::position() const
		{
			return _position;
		}

		vec3<f32> box::min() const
		{
			return _min;
		}

		vec3<f32> box::max() const
		{
			return _max;
		}

		void box::update_min_max_values()
		{
			_min.x = _position.x - (_size.x / 2.0f);
			_min.y = _position.y - (_size.y / 2.0f);
			_min.z = _position.z - (_size.z / 2.0f);

			_max.x = _position.x + (_size.x / 2.0f);
			_max.y = _position.y + (_size.y / 2.0f);
			_max.z = _position.z + (_size.z / 2.0f);
		}
	}
}
