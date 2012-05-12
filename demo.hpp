#ifndef DEMO_HPP
#define DEMO_HPP

#include <string>
#include <vector>

#include "block.hpp"
#include "ball.hpp"

#include "midi.hpp"

enum demo_block_type
{
	demo_block_type_position,
	demo_block_type_sound,
	demo_block_type_level,
	demo_block_type_speed
};

class demo_recorder_engine
{
public:
	void add_frame(block const & left_block, block const & right_block, ball const & the_ball);
	void add_sound(unsigned note, unsigned duration);
	void set_level(unsigned level);
	void set_speed(float speed);
	void save(std::string const & file_name);
	void clear();

private:
	std::string data;

	void write_block_type_identifier(demo_block_type type);

	void add_byte(unsigned input);
	void add_byte(float input);
	void add_word(float input);
	void add_float(float input);
};

class demo_block
{
public:
	demo_block_type type;

	float left_block_y;
	float right_block_y;
	float ball_x;
	float ball_y;

	unsigned note;
	unsigned duration;

	unsigned level;

	float speed;

	demo_block();
	demo_block(char const * & data);

private:
	unsigned char read_byte(char const * & data);

	demo_block_type read_type(char const * & data);
	float read_float_byte(char const * & data);
	float read_float_word(char const * & data);
	float read_float(char const * & data);
};

class demo_player_engine
{
public:
	bool load(std::string const & file_name);
	bool process_frame(block & left_block, block & right_block, ball & the_ball, midi_player & cheap_midi_sound, unsigned & level);

private:
	std::vector<demo_block> blocks;
	std::vector<demo_block>::iterator iterator;
};

#endif
