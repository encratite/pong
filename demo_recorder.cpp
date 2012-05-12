#include "demo.hpp"

#include <nil/file.hpp>

void demo_recorder_engine::add_frame(block const & left_block, block const & right_block, ball const & the_ball)
{
	write_block_type_identifier(demo_block_type_position);

	add_byte(left_block.y);
	add_byte(right_block.y);

	add_word(the_ball.x);
	add_byte(the_ball.y);
}

void demo_recorder_engine::add_sound(unsigned note, unsigned duration)
{
	write_block_type_identifier(demo_block_type_sound);

	add_byte(note);
	add_byte(duration);
}

void demo_recorder_engine::set_level(unsigned level)
{
	write_block_type_identifier(demo_block_type_level);

	add_byte(level);
}

void demo_recorder_engine::set_speed(float speed)
{
	write_block_type_identifier(demo_block_type_speed);

	add_float(speed);
}

void demo_recorder_engine::save(std::string const & file_name)
{
	nil::write_file(file_name, data);
	clear();
}

void demo_recorder_engine::clear()
{
	data.clear();
}

void demo_recorder_engine::write_block_type_identifier(demo_block_type type)
{
	data.push_back(static_cast<char>(type));
}

void demo_recorder_engine::add_byte(unsigned input)
{
	data.push_back(static_cast<char>(input));
}

void demo_recorder_engine::add_byte(float input)
{
	data.push_back(static_cast<char>(input));
}

void demo_recorder_engine::add_word(float input)
{
	data.push_back(static_cast<char>(static_cast<unsigned>(input) >> 8));
	data.push_back(static_cast<char>(input));
}

void demo_recorder_engine::add_float(float input)
{
	char const * byte_hack = reinterpret_cast<char *>(&input);
	for(std::size_t i = 0; i < sizeof(float); i++)
		data.push_back(byte_hack[i]);
}
