#ifndef BALL_HPP
#define BALL_HPP

#include "block.hpp"

enum ball_move_result
{
	ball_move_result_nothing,
	ball_move_result_side_hit,
	ball_move_result_left_hit,
	ball_move_result_left_out,
	ball_move_result_right_hit,
	ball_move_result_right_out
};

struct ball
{
	int width;
	int height;

	float x;
	float y;

	float direction;
	float speed;

	ball();
	ball(int x, int y, block const & left_block, block const & right_block, unsigned level);

	ball_move_result move(block const & left_block, block const & right_block);
	bool block_collision_check(block const & the_block, bool is_right_block);
	void collision(bool is_y);
	void block_collision(block const & the_block, bool is_right_block);
};

#endif
