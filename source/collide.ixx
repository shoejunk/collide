export module stk.collide;

import <SFML/Graphics.hpp>;
import stk.math;
import <cassert>;

using namespace stk;

namespace stk
{
	export class c_collision_mask
	{
	public:
		c_collision_mask()
			: m_x_size(0)
			, m_y_size(0)
		{
		}

		c_collision_mask(uint16_t x_size, uint16_t y_size)
			: m_mask(x_size * y_size)
			, m_x_size(x_size)
			, m_y_size(y_size)
		{
		}

		void from(sf::Image const& image)
		{
			auto image_size = image.getSize();
			m_x_size = image_size.x;
			m_y_size = image_size.y;
			m_mask.resize(m_x_size * m_y_size);
			uint8_t const* pixels = image.getPixelsPtr();
			for (uint16_t y = 0; y < m_y_size; ++y)
			{
				for (uint16_t x = 0; x < m_x_size; ++x)
				{
					uint8_t const* pixel = pixels + (y * m_x_size + x) * 4;
					m_mask[y * m_x_size + x] = pixel[3] > 0;
				}
			}
		}

		void from(std::vector<bool> const&& collision, uint16_t x_size, uint16_t y_size)
		{
			assert(collision.size() == x_size * y_size);
			m_x_size = x_size;
			m_y_size = y_size;
			m_mask = std::move(collision);
		}

		bool get(uint16_t x, uint16_t y) const
		{
			return m_mask[y * m_x_size + x];
		}

		bool get(c_vec2i pos) const
		{
			return get(pos.x(), pos.y());
		}

		void set(uint16_t x, uint16_t y, bool value)
		{
			m_mask[y * m_x_size + x] = value;
		}

		bool overlaps(c_collision_mask const& other)
		{
			for (uint16_t y = 0; y < m_y_size && y < other.m_y_size; ++y)
			{
				for (uint16_t x = 0; x < m_x_size && x < other.m_x_size; ++x)
				{
					if (get(x, y) && other.get(x, y))
					{
						return true;
					}
				}
			}
			return false;
		}

	private:
		std::vector<bool> m_mask;
		uint16_t m_x_size;
		uint16_t m_y_size;
	};
}