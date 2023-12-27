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
		constexpr c_collision_mask()
			: m_x_size(0)
			, m_y_size(0)
		{
		}

		constexpr c_collision_mask(uint16_t x_size, uint16_t y_size)
			: m_mask(x_size * y_size)
			, m_x_size(x_size)
			, m_y_size(y_size)
		{
		}

		c_collision_mask(sf::Image const&& image, uint16_t scale = 1)
			: m_mask(image.getSize().x * image.getSize().y * scale * scale)
			, m_x_size(image.getSize().x * scale)
			, m_y_size(image.getSize().y * scale)
		{
			uint16_t ox_size = image.getSize().x;
			uint8_t const* pixels = image.getPixelsPtr();
			for (uint16_t y = 0; y < m_y_size; ++y)
			{
				uint16_t oy = y / scale;
				for (uint16_t x = 0; x < m_x_size; ++x)
				{
					uint16_t ox = x / scale;
					uint8_t const* pixel = pixels + (oy * ox_size + ox) * 4;
					m_mask[y * m_x_size + x] = pixel[3] > 0;
				}
			}
		}

		c_collision_mask(sf::Image const&& image, c_vec2i margin_lo, c_vec2i margin_hi)
			: m_mask((image.getSize().x - margin_lo.x() - margin_hi.x()) * (image.getSize().y - margin_hi.y() - margin_hi.y()))
			, m_x_size(image.getSize().x - margin_lo.x() - margin_hi.x())
			, m_y_size(image.getSize().y - margin_hi.y() - margin_hi.y())
		{
			uint8_t const* pixels = image.getPixelsPtr();
			for (uint16_t y = 0; y < m_y_size; ++y)
			{
				for (uint16_t x = 0; x < m_x_size; ++x)
				{
					int32_t i = (y + margin_lo.y()) * image.getSize().x + (x + margin_lo.x());
					if (i < 0 || i >= image.getSize().x * image.getSize().y)
					{
						continue;
					}
					uint8_t const* pixel = pixels + i * 4;
					m_mask[y * m_x_size + x] = pixel[3] > 0;
				}
			}
		}

		constexpr c_collision_mask(std::vector<bool> const&& collision, uint16_t x_size, uint16_t y_size)
		{
			//assert(collision.size() == x_size * y_size);
			m_x_size = x_size;
			m_y_size = y_size;
			m_mask = std::move(collision);
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

		constexpr void from(std::vector<bool> const&& collision, uint16_t x_size, uint16_t y_size)
		{
			assert(collision.size() == x_size * y_size);
			m_x_size = x_size;
			m_y_size = y_size;
			m_mask = std::move(collision);
		}

		constexpr bool get(uint16_t x, uint16_t y) const
		{
			return m_mask[y * m_x_size + x];
		}

		constexpr bool get(c_vec2i pos) const
		{
			return get(pos.x(), pos.y());
		}

		constexpr void set(uint16_t x, uint16_t y, bool value)
		{
			m_mask[y * m_x_size + x] = value;
		}

		void set(c_vec2i xy, bool value)
		{
			m_mask[xy.y() * m_x_size + xy.x()] = value;
		}

		constexpr bool overlaps(c_collision_mask const& other, c_vec2i offset = {0, 0}) const
		{
			for (int32_t y = 0; y < m_y_size; ++y)
			{
				for (int32_t x = 0; x < m_x_size; ++x)
				{
					int32_t ox = x - offset.x();
					int32_t oy = y - offset.y();
					if (ox < 0 || ox >= other.m_x_size || oy < 0 || oy >= other.m_y_size)
					{
						continue;
					}
					if (get(x, y) && other.get(ox, oy))
					{
						return true;
					}
				}
			}
			return false;
		}

		constexpr c_collision_mask rotate(c_rot rot) const
		{
			c_collision_mask result = *this;
			for (uint16_t y = 0; y < m_y_size; ++y)
			{
				for (uint16_t x = 0; x < m_x_size; ++x)
				{
					c_vec2i rotated = rot.rot(c_vec2i{ x, y });
					if (rotated.x() < 0)
					{
						continue;
					}
					if (rotated.y() < 0)
					{
						continue;
					}
					result.set(rotated, get(x, y));
				}
			}
			return result;
		}

	private:
		std::vector<bool> m_mask;
		uint16_t m_x_size;
		uint16_t m_y_size;
	};
}