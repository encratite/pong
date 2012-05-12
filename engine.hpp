#ifndef ENGINE_HPP
#define ENGINE_HPP

#define NOMINMAX

#include <set>
#include <string>

#include <windows.h>

#include "midi.hpp"
#include "ball.hpp"
#include "block.hpp"

#include "demo.hpp"

#include "console.hpp"

//extern console debug;

extern int const window_width;
extern int const window_height;

extern int const fps;

extern float const default_speed;

extern float const pi;

enum game_status
{
	game_status_idle,
	game_status_playing,
	game_status_demo
};

class engine
{
public:
	engine();
	~engine();

	void create_dc(HDC window_dc);
	void invalidate(HWND window_handle);

	void process_frame(HWND window_handle);

	void process_key_down(WPARAM key);
	void process_key_up(WPARAM key);

	bool load_demo(std::string const & file_name);

	void start_game();
	void set_demo_level_limit(unsigned new_demo_level_limit);
	
private:
	game_status status;

	unsigned const wait_frame_count;
	unsigned current_frames_to_wait;

	int const width;
	int const height;

	bool initialised;

	HDC buffer_dc;
	HBITMAP bitmap;

	HBRUSH white_brush;
	RECT inner_rectangle;
	HPEN grey_pen;

	block left_block;
	block right_block;
	ball the_ball;

	unsigned level;
	bool ball_moving_to_ai;
	int block_target_height;

	HFONT font;
	midi_player cheap_midi_sound;

	std::set<WPARAM> pressed_keys;

	demo_recorder_engine demo_recorder;
	demo_player_engine demo_player;
	unsigned demo_level_limit;

	void initialise_ball_and_blocks();
	void increase_speed();

	void draw_rectangle(int x1, int y1, int x2, int y2);
	void draw_block(block const & input);
	void draw_ball();
	void draw_middle_line();
	void draw_text(std::string const & text, int x, int y);

	void ai_think(ball_move_result ball_move);
	void ai_set_block_target_height();

	std::string get_speed_string();

	void play_note(unsigned note, unsigned duration);
	void save_demo();

	void process_game_frame();
	void process_demo_frame();
};

#endif
