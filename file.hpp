#ifndef NIL_FILE_HPP
#define NIL_FILE_HPP

#include <string>
#include <vector>
#include <stdexcept>

#include <cstdio>

#include <nil/memory.hpp>

namespace nil
{
	enum file_mode 
	{
		file_mode_big_endian,
		file_mode_little_endian
	};

	class file
	{
	public:

		file();
		file(char const * file_name, bool create_file = false);
		file(std::string const & file_name, bool create_file = false);
		~file();

		bool is_open() const;

		void set_mode(file_mode new_file_mode);

		void use_big_endian();
		void use_little_endian();

		bool open(char const * file_name, bool create_file = false);
		bool open(std::string const & file_name, bool create_file = false);
		void close();

		void read(char * output, std::size_t bytes);
		unsigned int read_dword();
		unsigned int read_word();
		char read_byte();

		void write(char const * data, std::size_t bytes);
		void write(std::string const & data);
		void write_dword(unsigned int data);
		void write_word(unsigned int data);
		void write_byte(char data);

		std::size_t get_pointer() const;
		void set_pointer(std::size_t offset);
		void seek_end();
		std::size_t get_size();

		void skip(std::size_t offset);

	private:
		FILE * file_descriptor;
		file_mode mode;

		bool read_data(char * output, std::size_t bytes);
		void write_data(char const * output, std::size_t bytes);

		void write_bytes(unsigned int input, std::size_t size);
		unsigned int read_bytes(std::size_t size);

		void check_file_descriptor() const;

		void error_file_descriptor();
		void error_unknown();
	};

	bool read_file(std::string const & file_name, std::string & output);
	bool write_file(std::string const & file_name, std::string const & input);

	bool read_lines(std::string const & file_name, std::vector<std::string> & output);
}

#endif
