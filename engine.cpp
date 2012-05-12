#include "engine.hpp"

#include <sstream>

#define _USE_MATH_DEFINES
#include <cmath>

#include <ctime>

#include <nil/string.hpp>

//console debug;

float const pi = static_cast<float>(M_PI);

float const default_speed = 3.0f;

engine::engine():
	wait_frame_count(fps),
	current_frames_to_wait(wait_frame_count),

	width(window_width - 1),
	height(window_height - 1),

	left_block(32, window_height / 2),

	initialised(false),

	level(0),
	demo_level_limit(3),

	status(game_status_idle)
{
	inner_rectangle.left = 1;
	inner_rectangle.top = 1;

	inner_rectangle.right = width;
	inner_rectangle.bottom = height;

	white_brush = CreateSolidBrush(RGB(255, 255, 255));
	grey_pen = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	font = CreateFont(14, 8, 9, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, "Fixedsys");

	initialise_ball_and_blocks();
	ai_set_block_target_height();
}

engine::~engine()
{
	if(initialised)
	{
		DeleteObject(bitmap);
		DeleteDC(buffer_dc);
	}

	DeleteObject(white_brush);
	DeleteObject(grey_pen);
	DeleteObject(font);
}

void engine::start_game()
{
	status = game_status_playing;
}

void engine::set_demo_level_limit(unsigned new_demo_level_limit)
{
	demo_level_limit = new_demo_level_limit;
}

bool engine::load_demo(std::string const & file_name)
{
	bool success = demo_player.load(file_name);
	if(success)
	{
		status = game_status_demo;
		return true;
	}

	return false;
}

void engine::initialise_ball_and_blocks()
{
	ball_moving_to_ai = true;
	the_ball = ball(window_width / 2, window_height / 2, left_block, right_block, level);
	right_block = block(window_width - 32, window_height / 2);

	left_block.adjust_speed(level);
	right_block.adjust_speed(level);

	demo_recorder.set_level(level);
	demo_recorder.set_speed(the_ball.speed);
}

void engine::increase_speed()
{
	float const ball_speed_increase = 0.05f;
	float const block_speed_increase = ball_speed_increase * 0.6f;

	the_ball.speed += ball_speed_increase;
	left_block.speed += block_speed_increase;
	left_block.speed += block_speed_increase;

	demo_recorder.set_speed(the_ball.speed);
}

void engine::create_dc(HDC window_dc)
{
	buffer_dc = CreateCompatibleDC(window_dc);
	bitmap = CreateCompatibleBitmap(window_dc, window_width, window_height);
	SelectObject(buffer_dc, bitmap);
	SelectObject(buffer_dc, white_brush);
	SetTextColor(buffer_dc, RGB(192, 192, 192));
	initialised = true;

	draw_rectangle(0, 0, width, height);
}

void engine::invalidate(HWND window_handle)
{
	InvalidateRect(window_handle, &inner_rectangle, TRUE);
}

void engine::process_key_down(WPARAM key)
{
	if(key == 37 || key == 38)
	{
		left_block.start_moving(-1);
		pressed_keys.insert(key);
	}
	else if(key == 39 || key == 40)
	{
		left_block.start_moving(1);
		pressed_keys.insert(key);
	}
}

void engine::process_key_up(WPARAM key)
{
	pressed_keys.erase(key);
	if(pressed_keys.empty())
		left_block.stop_moving();
}

void engine::play_note(unsigned note, unsigned duration)
{
	demo_recorder.add_sound(note, duration);
	cheap_midi_sound.play_note(note, duration);
}

std::string engine::get_speed_string()
{
	std::stringstream stream;
	stream.precision(2);
	stream << "Speed: " << std::fixed << the_ball.speed;
	return stream.str();
}

void engine::save_demo()
{
	unsigned current_time = static_cast<unsigned>(std::time(0));
	demo_recorder.save(nil::string::number_to_string<unsigned>(level) + "_" + nil::string::number_to_string<unsigned>(current_time) + ".pdm");
}
