#include "engine.hpp"

#include <nil/random.hpp>

void engine::ai_think(ball_move_result ball_move)
{
	if(ball_move == ball_move_result_left_hit)
	{
		ball_moving_to_ai = true;
		ai_set_block_target_height();
	}
	else if(ball_move == ball_move_result_right_hit)
		ball_moving_to_ai = false;

	int block_middle = static_cast<int>(right_block.y) + right_block.height / 2;
	if(ball_moving_to_ai)
	{
		int const tolerance = 4;
		int ball_middle = static_cast<int>(the_ball.y) + the_ball.height / 2;
		int target_height_difference = block_middle + block_target_height - ball_middle;
		if(std::abs(target_height_difference) > tolerance)
		{
			if(target_height_difference > 0)
				right_block.start_moving(-1);
			else
				right_block.start_moving(1);
		}
		else
			right_block.stop_moving();
	}
	else
	{
		int tolerance = std::max(32 - static_cast<int>(level) * 4, 4);
		int distance_to_middle = block_middle - window_height / 2;
		if(std::abs(distance_to_middle) > tolerance)
		{
			if(distance_to_middle > 0)
				right_block.start_moving(-1);
			else
				right_block.start_moving(1);
		}
		else
			right_block.stop_moving();
	}
}

void engine::ai_set_block_target_height()
{
	int maximum_offset = (right_block.height - the_ball.height) / 2;
	unsigned const ai_height_skill_maximum = 10;
	float const pixels_per_level = 1.8f;
	unsigned ai_height_skill = std::min(level, ai_height_skill_maximum);
	block_target_height = std::min(static_cast<int>(static_cast<float>(nil::random::uint(0, ai_height_skill)) / static_cast<float>(ai_height_skill_maximum) * pixels_per_level), maximum_offset);
	if(nil::random::uint(0, 1))
		block_target_height = -block_target_height;
}
