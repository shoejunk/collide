import stk.collide;
import stk.log;
import stk.math;
import stk.sprite;
import <SFML/Graphics.hpp>;
import <cassert>;

using namespace stk;

int main()
{
	c_collision_mask collision(std::vector<bool>{ false, false, true, true }, 2, 2);
	c_collision_mask collision2(std::vector<bool>{ true, true, true, true }, 2, 2);

	// These two shapes should overlap
	assert(collision.overlaps(collision2));

	// After moving the second shape down by one pixel, they should no longer overlap
	assert(!collision.overlaps(collision2, {0, -1}));

	c_collision_mask collision3(std::vector<bool>{ true, false, true, false }, 2, 2);

	// collision and collision3 should overlap
	assert(collision.overlaps(collision3));

	c_collision_mask collision4 = collision3.rotate(90_deg);

	// collision and collision4 should not overlap
	assert(!collision.overlaps(collision4));

	c_sprite_bank sprites;
	sf::Sprite* rocket_sprite = sprites.make_sprite("rocket"_h, "data/rocket_small.png", 300, 300);
	if (rocket_sprite == nullptr)
	{
		errorln("Failed to load rocket sprite");
		return 1;
	}
	collision.from(rocket_sprite->getTexture()->copyToImage());
	return 0;
}
