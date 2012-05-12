#include <nil/file.hpp>

#include <nil/string.hpp>

#include <iostream>

namespace nil
{
	file::file():
		file_descriptor(0),
		mode(file_mode_big_endian)
	{
	}

	file::file(char const * file_name, bool create_file):
		file_descriptor(0),
		mode(file_mode_big_endian)
	{
		open(file_name, create_file);
	}

	file::file(std::string const & file_name, bool create_file):
		file_descriptor(0),
		mode(file_mode_big_endian)
	{
		open(file_name, create_file);
	}

	file::~file()
	{
		if(file_descriptor != 0)
		{
			close();
		}
	}

	void file::check_file_descriptor() const
	{
		if(file_descriptor == 0)
			throw std::runtime_error("Missing file descriptor");
	}

	void file::error_unknown()
	{
		throw std::runtime_error("Unknown error occured");
	}

	bool file::is_open() const
	{
		return file_descriptor != 0;
	}

	void file::set_mode(file_mode new_file_mode)
	{
		mode = new_file_mode;
	}

	void file::use_big_endian()
	{
		mode = file_mode_big_endian;
	}

	void file::use_little_endian()
	{
		mode = file_mode_little_endian;
	}

	bool file::read_data(char * output, std::size_t bytes)
	{
		check_file_descriptor();

		if(bytes == 0)
		{
			return false;
		}

		std::size_t success = std::fread(output, 1, bytes, file_descriptor);
		if(success == 0)
		{
			int fail = std::ferror(file_descriptor);
			if(fail != 0)
			{
				error_unknown();
			}
			return true;
		}

		return false;
	}

	void file::write_data(char const * output, std::size_t bytes)
	{
		check_file_descriptor();

		if(bytes == 0)
		{
			return;
		}

		std::size_t success = std::fwrite(output, 1, bytes, file_descriptor);
		if(success == 0)
		{
			int fail = std::ferror(file_descriptor);
			if(fail != 0)
			{
				error_unknown();
			}
		}
	}

	void file::write_bytes(unsigned int input, std::size_t size)
	{
		check_file_descriptor();

		char output[4];

		switch(mode)
		{
			case file_mode_big_endian:
			{
				write_big_endian(input, output, size);
				break;
			}

			case file_mode_little_endian:
			{
				write_little_endian(input, output, size);
				break;
			}

			default:
			{
				throw std::runtime_error("Encountered an unknown file mode while trying to write to a file");
			}
		}

		write_data(output, size);
	}

	unsigned int file::read_bytes(std::size_t size)
	{
		check_file_descriptor();

		char data[4];
		read_data(data, size);

		unsigned int output;

		switch(mode)
		{
			case file_mode_big_endian:
			{
				output = read_big_endian(data, size);
				break;
			}

			case file_mode_little_endian:
			{
				output = read_little_endian(data, size);
				break;
			}

			default:
			{
				throw std::runtime_error("Encountered an unknown file mode while trying to read from a file");
			}
		}
		return output;
	}

	bool file::open(char const * file_name, bool create_file)
	{
		if(file_descriptor)
		{
			close();
		}

		char fopen_mode[] = "w+b";
		if(!create_file)
		{
			fopen_mode[0] = 'r';
		}

		file_descriptor = std::fopen(file_name, fopen_mode);

		return (file_descriptor != 0);
	}

	bool file::open(std::string const & file_name, bool create_file)
	{
		return open(file_name.c_str(), create_file);
	}

	void file::close()
	{
		check_file_descriptor();
		std::fclose(file_descriptor);
	}

	void file::read(char * buffer, std::size_t bytes)
	{
		read_data(buffer, bytes);
	}

	unsigned int file::read_dword()
	{
		return read_bytes(4);
	}

	unsigned int file::read_word()
	{
		return read_bytes(2);
	}

	char file::read_byte()
	{
		char output;
		read_data(&output, 1);
		return output;
	}

	void file::write(char const * data, std::size_t bytes)
	{
		write_data(data, bytes);
	}

	void file::write(std::string const & data)
	{
		write_data(data.c_str(), data.length());
	}

	void file::write_dword(unsigned int data)
	{
		write_bytes(data, 4);
	}

	void file::write_word(unsigned int data)
	{
		write_bytes(data, 2);
	}

	void file::write_byte(char data)
	{
		write_data(&data, 1);
	}

	std::size_t file::get_size()
	{
		check_file_descriptor();
		fpos_t position;
		int fail = std::fgetpos(file_descriptor, &position);
		if(fail != 0)
		{
			throw std::runtime_error("Failed to retrieve file pointer");
		}
		seek_end();
		std::size_t output = static_cast<std::size_t>(std::ftell(file_descriptor));
		fail = std::fsetpos(file_descriptor, &position);
		if(fail != 0)
		{
			throw std::runtime_error("Failed to set file pointer");
		}
		return output;
	}

	std::size_t file::get_pointer() const
	{
		check_file_descriptor();
		long position = std::ftell(file_descriptor);
		if(position == -1)
		{
			throw std::runtime_error("Failed to retrieve file pointer");
		}
		return static_cast<std::size_t>(position);
	}

	void file::set_pointer(std::size_t offset)
	{
		check_file_descriptor();
		int fail = std::fseek(file_descriptor, static_cast<long>(offset), SEEK_SET);
		if(fail != 0)
		{
			throw std::runtime_error("Failed to set file pointer");
		}
	}

	void file::seek_end()
	{
		check_file_descriptor();
		int fail = std::fseek(file_descriptor, 0, SEEK_END);
		if(fail != 0)
		{
			throw std::runtime_error("Failed to seek end");
		}
	}

	void file::skip(std::size_t offset)
	{
		set_pointer(get_pointer() + offset);
	}

	bool read_file(std::string const & file_name, std::string & output)
	{
		file file_object;
		bool success = file_object.open(file_name);
		if(!success)
		{
			return false;
		}
		std::size_t file_size = file_object.get_size();
		char * file_content = new char[file_size];
		file_object.read(file_content, file_size);
		output.assign(file_content, file_size);
		delete [] file_content;
		file_object.close();
		return true;
	}

	bool write_file(std::string const & file_name, std::string const & input)
	{
		file file_object;
		bool success = file_object.open(file_name, true);
		if(success == false)
		{
			return false;
		}
		file_object.write(input);
		file_object.close();
		return true;
	}

	bool read_lines(std::string const & file_name, std::vector<std::string> & output)
	{
		std::string input;
		bool result = read_file(file_name, input);
		if(result == false)
		{
			return false;
		}
		input = string::erase(input, "\r");
		output = string::tokenise(input, '\n');
		return true;
	}
}
