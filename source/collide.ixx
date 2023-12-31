export module stk.collide;

import <SFML/Graphics.hpp>;
import stk.log;
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
			, m_scale(1)
		{
		}

		constexpr c_collision_mask(uint16_t x_size, uint16_t y_size)
			: m_mask(x_size* y_size)
			, m_x_size(x_size)
			, m_y_size(y_size)
			, m_scale(1)
		{
		}

		c_collision_mask(sf::Image const&& image, uint16_t scale = 1)
			: m_mask(image.getSize().x* image.getSize().y* scale* scale)
			, m_x_size(image.getSize().x* scale)
			, m_y_size(image.getSize().y* scale)
			, m_scale(scale)
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
			: m_mask((image.getSize().x - margin_lo.x() - margin_hi.x())* (image.getSize().y - margin_hi.y() - margin_hi.y()))
			, m_x_size(image.getSize().x - margin_lo.x() - margin_hi.x())
			, m_y_size(image.getSize().y - margin_hi.y() - margin_hi.y())
			, m_scale(1)
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
			: m_scale(1)
		{
			//assert(collision.size() == x_size * y_size);
			m_x_size = x_size;
			m_y_size = y_size;
			m_mask = std::move(collision);
		}

		void from(sf::Image const& image, uint16_t scale = 1)
		{
			auto image_size = image.getSize();
			m_x_size = image_size.x;
			m_y_size = image_size.y;
			m_scale = scale;
			try
			{
				m_mask.resize(m_x_size * m_y_size);
			}
			catch (std::bad_alloc const& e)
			{
				errorln("c_collision_mask::from bad alloc: {}", e.what());
				return;
			}
			catch (std::length_error const& e)
			{
				errorln("c_collision_mask::from length error: {}", e.what());
				return;
			}
			catch (std::exception const& e)
			{
				errorln("c_collision_mask::from exception: {}", e.what());
				return;
			}
			catch (...)
			{
				errorln("c_collision_mask::from unknown exception");
				return;
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
		}

		constexpr void from(std::vector<bool> const&& collision, uint16_t x_size, uint16_t y_size)
		{
			assert(collision.size() == x_size * y_size);
			m_x_size = x_size;
			m_y_size = y_size;
			m_mask = std::move(collision);
		}

		void to_image(sf::Image& image) const
		{
			image.create(m_x_size, m_y_size);
			for (uint16_t y = 0; y < m_y_size; ++y)
			{
				for (uint16_t x = 0; x < m_x_size; ++x)
				{
					sf::Color color;
					uint8_t c = get(x, y) ? 255 : 0;
					color.r = c;
					color.g = c;
					color.b = c;
					color.a = c;
					image.setPixel(x, y, color);
				}
			}
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

		constexpr bool overlaps(c_collision_mask const& other, c_vec2i offset = { 0, 0 }) const
		{
			offset.x() /= m_scale;
			offset.y() /= m_scale;
			for (auto y = 0; y < m_y_size; ++y)
			{
				for (auto x = 0; x < m_x_size; ++x)
				{
					auto ox = x - offset.x();
					auto oy = y - offset.y();
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

		constexpr c_collision_mask rotate(c_angle rot) const
		{
			c_collision_mask result = *this;
			std::fill(result.m_mask.begin(), result.m_mask.end(), false);
			c_vec2f center = { static_cast<float>(m_x_size - 1) * 0.5f, static_cast<float>(m_y_size - 1) * 0.5f };
			for (auto y = 0; y < m_y_size; ++y)
			{
				for (auto x = 0; x < m_x_size; ++x)
				{
					c_vec2i rotated = rot.rot(c_vec2f{ static_cast<float>(x), static_cast<float>(y) } - center) + center;
					if (rotated.x() < 0)
					{
						continue;
					}
					if (rotated.y() < 0)
					{
						continue;
					}
					if (rotated.x() >= m_x_size)
					{
						continue;
					}
					if (rotated.y() >= m_y_size)
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
		uint16_t m_scale;
	};
}