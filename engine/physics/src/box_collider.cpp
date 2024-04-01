#include "BoxCollider.hpp"

namespace birb
{
	namespace collider
	{
		box::box() {}

		bool box::collides_with(const box& box)
		{
			// AABB algorithm

			return	(this->min().x <= box.max().x && this->max().x >= box.min().x) &&
					(this->min().y <= box.max().y && this->max().y >= box.min().y) &&
					(this->min().z <= box.max().z && this->max().z >= box.min().z);
		}

		void box::set_position(const vec3<float>& position)
		{
			this->position = position;
			update_min_max_values();
		}

		void box::set_size(const vec3<float>& size)
		{
			this->size = size;
			update_min_max_values();
		}

		void box::set_position_and_size(const vec3<float>& position, const vec3<float>& size)
		{
			this->position = position;
			this->size = size;
			update_min_max_values();
		}

		void box::set_position_and_size(const component::transform& transform)
		{
			this->position = transform.position;
			this->size = transform.local_scale;
			update_min_max_values();
		}

		vec3<float> box::min() const
		{
			return _min;
		}

		vec3<float> box::max() const
		{
			return _max;
		}

		void box::update_min_max_values()
		{
			_min.x = position.x - (size.x / 2.0f);
			_min.y = position.y - (size.y / 2.0f);
			_min.z = position.z - (size.z / 2.0f);

			_max.x = position.x + (size.x / 2.0f);
			_max.y = position.y + (size.y / 2.0f);
			_max.z = position.z + (size.z / 2.0f);
		}
	}
}
