#include "demo.hpp"

#include <cstdlib>

#include <nil/file.hpp>
#include <nil/exception.hpp>

demo_block::demo_block()
{
}

demo_block::demo_block(char const * & data)
{
	type = read_type(data);

	switch(type)
	{
		case demo_block_type_position:
			left_block_y = read_float_byte(data);
			right_block_y = read_float_byte(data);
			ball_x = read_float_word(data);
			ball_y = read_float_byte(data);
			break;

		case demo_block_type_sound:
			note = read_byte(data);
			duration = read_byte(data);
			break;

		case demo_block_type_level:
			level = read_byte(data);
			break;

		case demo_block_type_speed:
			speed = read_float(data);
			break;

		default:
			throw nil::exception("Corrupted demo file, invalid demo block type encountered");
	}
}

unsigned char demo_block::read_byte(char const * & data)
{
	unsigned char output = static_cast<unsigned char>(*data);
	data++;
	return output;
}

demo_block_type demo_block::read_type(char const * & data)
{
	return static_cast<demo_block_type>(read_byte(data));
}

float demo_block::read_float_byte(char const * & data)
{
	return static_cast<float>(read_byte(data));
}

float demo_block::read_float_word(char const * & data)
{
	unsigned word = read_byte(data) << 8;
	word |= read_byte(data);
	return static_cast<float>(word);
}

float demo_block::read_float(char const * & data)
{
	float output;
	std::memcpy(&output, data, static_cast<int>(sizeof(float)));
	data += sizeof(float);
	return output;
}

bool demo_player_engine::load(std::string const & file_name)
{
	blocks.clear();

	std::string data;
	bool success = nil::read_file(file_name, data);
	if(!success)
		return false;

	for(char const * pointer = data.c_str(), * end = pointer + data.length(); pointer < end;)
		blocks.push_back(demo_block(pointer));

	iterator = blocks.begin();

	return true;
}

bool demo_player_engine::process_frame(block & left_block, block & right_block, ball & the_ball, midi_player & cheap_midi_sound, unsigned & level)
{
	for(std::vector<demo_block>::iterator end = blocks.end(); iterator != end; iterator++)
	{
		demo_block const & current_block = *iterator;
		switch(current_block.type)
		{
			case demo_block_type_position:
				left_block.y = current_block.left_block_y;
				right_block.y = current_block.right_block_y;
				the_ball.x = current_block.ball_x;
				the_ball.y = current_block.ball_y;
				iterator++;
				return false;

			case demo_block_type_sound:
				cheap_midi_sound.play_note(current_block.note, current_block.duration);
				break;

			case demo_block_type_level:
				level = current_block.level;
				break;

			case demo_block_type_speed:
				the_ball.speed = current_block.speed;
				break;
		}
	}
	
	return true;
}
