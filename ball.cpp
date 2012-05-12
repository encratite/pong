#include "ball.hpp"

#define NOMINMAX
#include <cmath>

#include <nil/random.hpp>

#include "engine.hpp"

ball::ball()
{
}

ball::ball(int x, int y, block const & left_block, block const & right_block, unsigned level):
	width(8),
	height(8),

	x(static_cast<float>(x - width / 2)),
	y(static_cast<float>(y - height / 2)),

	direction(nil::random::sp_float(-0.2f, 0.2f)),
	speed(default_speed + static_cast<float>(level) * 0.5f)
{
	if(nil::random::uint(0, 1))
		direction += pi;
}

ball_move_result ball::move(block const & left_block, block const & right_block)
{
	for(float i = speed; i > 0.0f; i -= 1.0f)
	{
		float step = std::min(1.0f, i);
		x += std::cos(direction) * step;
		y += std::sin(direction) * step;

		float right_border = static_cast<float>(window_width - width - 2);
		float bottom_border = static_cast<float>(window_height - height - 2);

		if(x <= 1.0f)
			return ball_move_result_left_out;
		if(y <= 1.0f)
		{
			y = 2.f;
			collision(true);
			return ball_move_result_side_hit;
		}
		if(x >= right_border)
			return ball_move_result_right_out;
		if(y >= bottom_border)
		{
			y = bottom_border - 1.0f;
			collision(true);
			return ball_move_result_side_hit;
		}

		if(block_collision_check(left_block, false))
		{
			block_collision(left_block, false);
			return ball_move_result_left_hit;
		}
		if(block_collision_check(right_block, true))
		{
			block_collision(right_block, true);
			return ball_move_result_right_hit;
		}
	}

	return ball_move_result_nothing;
}

bool ball::block_collision_check(block const & the_block, bool is_right_block)
{
	float const tolerance = 1.0f;

	bool side_edge_hit;
	if(is_right_block)
		side_edge_hit = std::abs((x + width) - the_block.x) <= tolerance;
	else
		side_edge_hit = std::abs(x - (the_block.x + the_block.width)) <= tolerance;

	bool upper_edge_hit = y >= the_block.y && y <= the_block.y + the_block.height;
	bool lower_edge_hit = y + height >= the_block.y && y + height <= the_block.y + the_block.height;

	return side_edge_hit && (upper_edge_hit || lower_edge_hit);
}

void ball::collision(bool is_y)
{
	direction = (is_y ? 0.0f : pi) - direction;
}

void ball::block_collision(block const & the_block, bool is_right_block)
{
	int offset = static_cast<int>(speed);
	if(is_right_block)
		x = the_block.x - offset - width;
	else
		x = the_block.x + the_block.width + offset;

	float const maximum_angle = 0.5f * pi;

	direction = (is_right_block ? pi : 0.0f) + (is_right_block ? -1.0f : 1.0f) * ((y + static_cast<float>(height / 2) - static_cast<float>(the_block.y)) / static_cast<float>(the_block.height) - 0.5f) * maximum_angle;
}
