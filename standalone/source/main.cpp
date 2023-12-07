import stk.collide;
import stk.log;
import stk.sprite;
import <SFML/Graphics.hpp>;
import <cassert>;

using namespace stk;

int main()
{
	c_collision_mask collision;
	collision.from(std::vector<bool>{ true, false, false, true }, 2, 2);
	c_collision_mask collision2;
	collision2.from(std::vector<bool>{ false, true, true, false }, 2, 2);
	assert(!collision.overlaps(collision2));
	collision2.set(0, 0, true);
	assert(collision.overlaps(collision2));
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
