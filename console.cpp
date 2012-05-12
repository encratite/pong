#include "console.hpp"

console::console()
{
	AllocConsole();
	output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
}

console::~console()
{
	FreeConsole();
}

void console::write(std::string const & input)
{
	DWORD bytes_written;
	WriteConsole(output_handle, reinterpret_cast<VOID const *>(input.c_str()), static_cast<DWORD>(input.length()), &bytes_written, 0);
}
