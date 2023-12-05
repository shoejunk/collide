import stk.collide;
import stk.log;
import stk.sprite;
import <SFML/Graphics.hpp>;

using namespace stk;

int main()
{
	c_collision_mask collision(128, 128);
	c_sprite_bank sprites;
	sf::Sprite* rocket_sprite = sprites.make_sprite("rocket"_h, "data/rocket_small.png", 300, 300);
	if (rocket_sprite == nullptr)
	{
		errorln("Failed to load rocket sprite");
		return 1;
	}
	collision.from_image(rocket_sprite->getTexture()->copyToImage());
	return 0;
}
