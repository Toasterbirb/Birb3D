#include "BoxCollider.hpp"
#include "Transform.hpp"

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

		bool box::collides_with(const box& box)
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
