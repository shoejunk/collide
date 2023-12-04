import stk.collide;
import stk.sprite;
import <SFML/Graphics.hpp>;

using namespace stk;

int main()
{
	c_collision_mask collision(128, 128);
	c_sprite_bank sprites;
	sprites.make_sprite("rocket"_h, "rocket_small.png", 300, 300);
	return 0;
}
