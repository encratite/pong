#include "engine.hpp"

#include <ctime>

#include <nil/string.hpp>

void engine::process_frame(HWND window_handle)
{
	PAINTSTRUCT paint_object;
	HDC window_dc = BeginPaint(window_handle, &paint_object);

	if(status != game_status_idle)
	{

		FillRect(buffer_dc, &inner_rectangle, white_brush);

		int const inner_height = height - 1;
		int const inner_width = width - 1;

		HPEN old_pen = reinterpret_cast<HPEN>(SelectObject(buffer_dc, grey_pen));

		draw_middle_line();
		int text_x = window_width / 2 + 8;
		draw_text("Level " + nil::string::number_to_string<unsigned>(level), text_x, 8);
		draw_text(get_speed_string(), text_x, 24);

		SelectObject(buffer_dc, old_pen);

		if(status == game_status_playing)
			process_game_frame();
		else
			process_demo_frame();
	}

	cheap_midi_sound.process_frame();

	BitBlt(window_dc, 0, 0, window_width, window_height, buffer_dc, 0, 0, SRCCOPY);

	EndPaint(window_handle, &paint_object);
}

void engine::process_game_frame()
{
	left_block.move();

	if(current_frames_to_wait)
	{
		draw_block(left_block);
		draw_block(right_block);

		draw_ball();

		demo_recorder.add_frame(left_block, right_block, the_ball);

		current_frames_to_wait--;
	}
	else
	{
		right_block.move(true);

		ball_move_result ball_move = the_ball.move(left_block, right_block);

		draw_block(left_block);
		draw_block(right_block);

		draw_ball();

		ai_think(ball_move);

		unsigned short_sound_length = fps / 10;

		switch(ball_move)
		{
			case ball_move_result_left_out:
				play_note(36, fps);
				if(level >= demo_level_limit)
					save_demo();
				else
					demo_recorder.clear();
				level = 0;
				initialise_ball_and_blocks();
				current_frames_to_wait = wait_frame_count;
				break;

			case ball_move_result_right_out:
				play_note(72, fps);
				level++;
				initialise_ball_and_blocks();
				current_frames_to_wait = wait_frame_count;
				break;

			case ball_move_result_left_hit:
				increase_speed();
				play_note(60, short_sound_length);
				break;

			case ball_move_result_right_hit:
				increase_speed();
				play_note(57, short_sound_length);
				break;

			case ball_move_result_side_hit:
				play_note(84, short_sound_length);
				break;

			case ball_move_result_nothing:
				demo_recorder.add_frame(left_block, right_block, the_ball);
				break;
		}
	}
}

void engine::process_demo_frame()
{
	demo_player.process_frame(left_block, right_block, the_ball, cheap_midi_sound, level);

	draw_block(left_block);
	draw_block(right_block);

	draw_ball();
}
