#ifndef BLOCK_HPP
#define BLOCK_HPP

struct block
{
	int width;
	int height;

	float x;
	float y;

	int direction;

	float speed;

	block();
	block(int x, int y);

	void start_moving(int movement_direction);
	void stop_moving();
	void move(bool is_right_block = false);
	void adjust_speed(unsigned level);
};

#endif
