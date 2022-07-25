#pragma once 
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

struct Lower
{
	void		operator()(char & c)
	{
		c = std::tolower(c);
	}
};

class Request
{
	public :

		Request() {}

		Request(std::string const & request) : request(request)
		{
			std::string::iterator		it;
			
			try
			{
				it = this->parse_start_line();
				it = this->parse_header(it);
				this->format = true;
			}
			catch (std::exception const & e)
			{
				this->format = false;
			}
		}

		virtual ~Request() {}

		Request(Request const & src) : index(src.index)
		{
		}

		Request&		operator=(Request const & rhs)
		{
			this->index = rhs.index;
			return (*this);
		}

		// Getters

		std::string			get_type(void) const
		{
			return (this->type);
		}

		std::string			get_version(void) const
		{
			return (this->version);
		}

		const std::string			&get_request(void) const
		{
			return (this->request);
		}

		const std::string			&get_index(void) const
		{
			return (this->index);
		}

		bool						get_connection(void) const
		{
			return (this->connection);
		}

		bool						get_format(void) const
		{
			return (this->format);
		}

		std::string			get_content_type(void) const
		{
			return (this->content_type);
		}

		// Setters

		void	set_request(std::string const & request)
		{
			this->request = request;
		}

		void	set_type(std::string const & type)
		{
			this->type = type;
		}

		void	set_index(std::string const & index)
		{
			this->index = index;
		}

		void	set_version(std::string const & version)
		{
			this->version = version;
		}

		void	set_connection(bool connection)
		{
			this->connection = connection;
		}

		void	set_format(bool format)
		{
			this->format = format;
		}

		void	set_content_type(std::string const & content_type)
		{
			this->content_type = content_type;
		}

		class FormatException : public std::exception
		{
			public :
				FormatException() {};
				virtual const char* what() const throw()
				{
					return ("Error: missing space");
				};
		};

	protected :

		// general
	
		std::string				request;

		// first request line

		std::string				index;
		std::string				type;
		std::string				version;

		// request header

		bool					connection;
		std::string				content_type;

		// error

		bool					format;

		std::string		get_elem_at(int n)
		{
			std::string::const_iterator it = this->get_request().begin();
			std::string::const_iterator it2;
			
			for (int i = 0; i < n; i++)
			{
				while (*it == ' ')
				{
					it++;
				}
				it2 = it;
				while (*it != ' ' && it != this->get_request().end())
				{
					it++;
				}
			}
			return (get_request().substr(it2 - get_request().begin(), it - it2));
		}

		std::string::iterator		skip_space(std::string::iterator it)
		{
			while (it != request.end() && (*it == ' ' || *it == '\t'))
			{
				++it;
			}
			return (it);
		}

		std::string		read_string_in_request(std::string::iterator it)
		{
			std::string		str;
			bool			format = false;

			while (it != this->request.end() && *it != ' ' && *it != '\t' && *it != '\r' && *it != '\n')
			{
				str.append(1, *it);
				format = true;
				++it;
			}
			if (!format)
				throw (FormatException());
			return (str);
		}

		std::string::iterator		skip_end_line(std::string::iterator it)
		{
			std::pair<std::string::iterator, bool>		ret;

			ret.first = it;
			it = this->skip_space(it);
			if (*it == '\r')
			{
				++it;
				if (*it == '\n')
				{
					++it;
					ret.second = true;
					return (it);
				}
			}
			throw (FormatException());
		}

		std::string::iterator		parse_start_line(void)
		{
			std::string::iterator it = this->request.begin();

			for (int i = 0; i < 3; i++)
			{
				std::cout << "i = " << i << std::endl;
				it = this->skip_space(it);
				switch (i)
				{
					case 0 :
						it += (this->type = this->read_string_in_request(it)).length();
						break;
					case 1:
						it += (this->index = this->read_string_in_request(it)).length();
						break;
					case 2:
						it += (this->version = this->read_string_in_request(it)).length();
				}
			}
			it = skip_end_line(it);
			return (it);
		}

		bool						end_of_request_line(std::string::iterator it)
		{
			if (*it == '\r')
			{
				++it;
				if (*it == '\n')
				{
					++it;
					return (true);
				}
			}
			return (false);
		}

		void	assign_valid_fields(std::map<std::string, std::string> & fields)
		{
			if (!fields["connection"].compare("keep-alive"))
				this->set_connection(1);
			else
				this->set_connection(0);
			this->set_content_type(fields["content-type"]);
		}

		void	print_header(void) const
		{
			std::cout << "Connection: " << this->get_connection() << std::endl;
			std::cout << "Content-Type: " << this->get_content_type() << std::endl;
		}

		std::string::iterator		parse_header(std::string::iterator it)
		{
			std::map<std::string, std::string>		fields;
			std::pair<std::string, std::string>		header_line;

			it = skip_space(it);
			while (!end_of_request_line(it))
			{

				while (*it != ':' && *it != '\r' && *it != '\n')
				{
					header_line.first.append(1, *it);
					++it;
				}
				if (*it == ':')
					++it;
				it = skip_space(it);
				while (!end_of_request_line(it))
				{
					header_line.second.append(1, *it);
					++it;
				}
				std::for_each(header_line.first.begin(), header_line.first.end(), Lower());
				std::for_each(header_line.second.begin(), header_line.second.end(), Lower());
				fields.insert(header_line);
				header_line.first.clear();
				header_line.second.clear();
				it = skip_end_line(it);
			}
			this->assign_valid_fields(fields);
			/*
			std::cout << "HEADER = " << std::endl;
			for (std::map<std::string, std::string>::iterator it = fields.begin();
				it != fields.end(); ++it)
			{
				std::cout << "first = " << it->first << " second = " << it->second << std::endl;
			}
			*/
			return (it);
		}
};
