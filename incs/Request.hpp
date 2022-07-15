#pragma once 
#include <string>
#include <iostream>

class Request
{
	public :

		Request() {}

		Request(std::string const & request) : request(request)
		{
			std::string::iterator		it;
			
			std::cout << "request = " << request << std::endl;
			try
			{
				it = this->parse_start_line();
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
		
		std::string				request;
		std::string				type;
		std::string				index;
		std::string				version;
		bool					connection;
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
			std::pair<std::string::iterator, bool>		ret;

			for (int i = 0; i < 3; i++)
			{
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
};
