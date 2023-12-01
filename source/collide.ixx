export module stk.collide;

import <SFML/Graphics.hpp>;
import stk.math;

using namespace stk;

namespace stk
{
	export class c_collision_mask
	{
	public:
		c_collision_mask(uint16_t x_size, uint16_t y_size)
			: m_mask(x_size * y_size)
			, m_x_size(x_size)
			, m_y_size(y_size)
		{
		}

		bool from_image(sf::Image const& image)
		{
			auto image_size = image.getSize();
			if (image_size.x != m_x_size || image_size.y != m_y_size)
			{
				return false;
			}
			uint8_t const* pixels = image.getPixelsPtr();
			for (uint16_t y = 0; y < m_y_size; ++y)
			{
				for (uint16_t x = 0; x < m_x_size; ++x)
				{
					uint8_t const* pixel = pixels + (y * m_x_size + x) * 4;
					m_mask[y * m_x_size + x] = pixel[3] > 0;
				}
			}
			return true;
		}

		bool get(uint16_t x, uint16_t y) const
		{
			return m_mask[y * m_x_size + x];
		}

		bool get(c_vec2i pos) const
		{
			return get(pos.x, pos.y);
		}

	private:
		std::vector<bool> m_mask;
		uint16_t m_x_size;
		uint16_t m_y_size;
	};
}