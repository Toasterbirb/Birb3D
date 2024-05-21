#include "Box2DCollider.hpp"
#include "Transform.hpp"

namespace birb
{
	namespace collider
	{
		box2d::box2d() {}

		box2d::box2d(const transform& transform)
		:_size(transform.local_scale), _position(transform.position)
		{
			update_min_max_values();
		}

		bool box2d::collides_with(const box2d& box) const
		{
			// AABB algorithm

			return	(this->min().x <= box.max().x && this->max().x >= box.min().x) &&
					(this->min().y <= box.max().y && this->max().y >= box.min().y);
		}

		void box2d::set_position(const vec2<f32>& position)
		{
			this->_position = position;
			update_min_max_values();
		}

		void box2d::set_size(const vec2<f32>& size)
		{
			this->_size = size;
			update_min_max_values();
		}

		void box2d::set_position_and_size(const vec2<f32>& position, const vec2<f32>& size)
		{
			this->_position = position;
			this->_size = size;
			update_min_max_values();
		}

		void box2d::set_position_and_size(const transform& transform)
		{
			this->_position = transform.position;
			this->_size = transform.local_scale;
			update_min_max_values();
		}

		vec2<f32> box2d::size() const
		{
			return _size;
		}

		vec2<f32> box2d::position() const
		{
			return _position;
		}

		vec2<f32> box2d::min() const
		{
			return _min;
		}

		vec2<f32> box2d::max() const
		{
			return _max;
		}

		void box2d::update_min_max_values()
		{
			_min.x = _position.x - (_size.x / 2.0f);
			_min.y = _position.y - (_size.y / 2.0f);

			_max.x = _position.x + (_size.x / 2.0f);
			_max.y = _position.y + (_size.y / 2.0f);
		}
	}
}
