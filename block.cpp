#include "block.hpp"

#include "engine.hpp"

#include <nil/string.hpp>

float const block_movement_factor = 0.6f;

block::block()
{
}

block::block(int x, int y):
	width(12),
	height(36),

	x(static_cast<float>(x - width / 2)),
	y(static_cast<float>(y - height / 2)),

	speed(default_speed * block_movement_factor),

	direction(0)
{
}

void block::start_moving(int movement_direction)
{
	direction = movement_direction;
}

void block::stop_moving()
{
	direction = 0;
}

void block::move(bool is_right_block)
{
	if(direction)
	{
		y = std::max(y + direction * speed, 1.0f);
		y = std::min(y, static_cast<float>(window_height - height - 2));
	}
}

void block::adjust_speed(unsigned level)
{
	speed = default_speed * block_movement_factor + level * 0.3f;
}
