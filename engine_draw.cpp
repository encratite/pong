#include "engine.hpp"

#include <nil/array.hpp>

void engine::draw_rectangle(int x1, int y1, int x2, int y2)
{
	POINT rectangle_points[5];
	rectangle_points[0].x = x1;
	rectangle_points[0].y = y1;
	rectangle_points[1].x = x2;
	rectangle_points[1].y = y1;
	rectangle_points[2].x = x2;
	rectangle_points[2].y = y2;
	rectangle_points[3].x = x1;
	rectangle_points[3].y = y2;
	rectangle_points[4] = rectangle_points[0];
	Polyline(buffer_dc, rectangle_points, static_cast<int>(nil::countof(rectangle_points)));
}

void engine::draw_block(block const & input)
{
	int x = static_cast<int>(input.x);
	int y = static_cast<int>(input.y);
	draw_rectangle(x, y, x + input.width, y + input.height);
}

void engine::draw_ball()
{
	int x = static_cast<int>(the_ball.x);
	int y = static_cast<int>(the_ball.y);

	draw_rectangle(x, y, x + the_ball.width, y + the_ball.height);
}

void engine::draw_middle_line()
{
	POINT line_points[2];
	int const middle_line_x = width / 2;
	line_points[0].x = middle_line_x;
	line_points[0].y = 1;
	line_points[1].x = middle_line_x;
	line_points[1].y = height;
	Polyline(buffer_dc, line_points, static_cast<int>(nil::countof(line_points)));
}

void engine::draw_text(std::string const & text, int x, int y)
{
	TextOut(buffer_dc, x, y, text.c_str(), static_cast<int>(text.length()));
}
