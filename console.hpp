#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include <string>

#include <windows.h>

class console
{
public:
	console();
	~console();

	void write(std::string const & input);

private:
	HANDLE output_handle;
};

#endif
