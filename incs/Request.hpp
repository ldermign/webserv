#pragma once 
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <sstream>

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

		Request(std::string const & request) : request(request), content_length(0)
		{
			std::string::iterator		it;
			
			try
			{
				it = this->parse_start_line();
				it = this->parse_header(it);
				it = this->parse_body(it);
				this->parse_upload();
				std::cout << "filename = " << this->get_upload_file().first << " file content = "<< this-> get_upload_file().second << std::endl;
				this->format = true;
			}
			catch (std::exception const & e)
			{
				this->format = false;
			}
			std::cout << "HEADER request :" << std::endl;
			this->print_header();
		}

		virtual ~Request() {}

		Request(Request const & rhs)
		{
			this->set_request(rhs.get_request());
			this->set_source(rhs.get_source());
			this->set_params(rhs.get_params());
			this->set_method(rhs.get_method());
			this->set_version(rhs.get_version());
			this->set_connection(rhs.get_connection());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_body(rhs.get_body());
			this->set_format(rhs.get_format());
			this->set_upload_file(rhs.get_upload_file());
		}

		Request&		operator=(Request const & rhs)
		{
			this->set_request(rhs.get_request());
			this->set_source(rhs.get_source());
			this->set_params(rhs.get_params());
			this->set_method(rhs.get_method());
			this->set_version(rhs.get_version());
			this->set_connection(rhs.get_connection());
			this->set_content_type(rhs.get_content_type());
			this->set_content_length(rhs.get_content_length());
			this->set_body(rhs.get_body());
			this->set_format(rhs.get_format());
			this->set_upload_file(rhs.get_upload_file());
			return (*this);
		}

		// Getters

		std::string			get_method(void) const
		{
			return (this->method);
		}

		std::string			get_version(void) const
		{
			return (this->version);
		}

		const std::string			&get_request(void) const
		{
			return (this->request);
		}

		const std::string			&get_source(void) const
		{
			return (this->source);
		}

		const std::string			&get_params(void) const
		{
			return (this->params);
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

		std::string			get_body(void) const
		{
			return (this->body);
		}

		size_t				get_content_length(void) const
		{
			return (this->content_length);
		}

		std::pair<std::string, std::string>		get_upload_file(void) const
		{
			return (this->upload_file);
		}

		// Setters

		void	set_request(std::string const & request)
		{
			this->request = request;
		}

		void	set_method(std::string const & method)
		{
			this->method = method;
		}

		void	set_source(std::string const & source)
		{
			this->source = source;
		}

		void	set_params(std::string const & params)
		{
			this->params = params;
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

		void	set_body(std::string const & body)
		{
			this->body = body;
		}

		void	set_content_length(size_t content_length)
		{
			this->content_length = content_length;
		}

		void	set_upload_file(std::pair<std::string, std::string> upload_file)
		{
			this->upload_file = upload_file;
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

		std::string				source;
		std::string				params;
		std::string				method;
		std::string				version;

		// request header

		bool					connection;
		std::string				content_type;
		size_t					content_length;

		std::string				body;
		std::pair<std::string, std::string>		upload_file;

		// error

		bool					format;

		std::string::iterator		skip_request_line(std::string::iterator it)
		{
			while (!end_of_request_line(it))
			{
				++it;
			}
			it += 2;
			return (it);
		}

		std::string			stock_request_line(std::string::iterator const & it)
		{
			std::string					request_line;
			std::string::iterator		end_it = it;

			while (!end_of_request_line(end_it))
			{
				++end_it;
			}
			request_line = std::string(it, end_it);
			return (request_line);
		}

		void			parse_upload(void)
		{
			std::string::iterator		it = this->body.begin();
			std::string					content_disposition;
			std::string					elem_content_dispo;

			if (!this->get_content_type().compare(0, 19, "multipart/form-data"))
			{
				if (this->body.empty())
					throw (FormatException());
				it = skip_request_line(it);
				content_disposition = stock_request_line(it);
				if (!content_disposition.compare(0, 20, "Content-Disposition:"))
				{
					it += 20;
					while (it != this->body.end() && !end_of_request_line(it))
					{
						it = this->skip_space(it);
						elem_content_dispo.erase(elem_content_dispo.begin(), elem_content_dispo.end());
						while (it != this->body.end() && *it != ';' && !end_of_request_line(it))
						{
							elem_content_dispo.append(1, *it);
							++it;
						}
						if (!elem_content_dispo.compare(0, 8, "filename"))
							this->upload_file.first = elem_content_dispo.substr(8);
						if (*it == ';')
							++it;
					}
					++it;
					++it;
				}
				else
					throw (FormatException());
				it = skip_request_line(it);
				it = skip_request_line(it);
				if (it == this->body.end())
					throw (FormatException());
				while (!end_of_request_line(it) && it != this->body.end())
				{
					this->upload_file.second.append(stock_request_line(it));
					it = skip_request_line(it);
				}
			}
		}

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

		std::string		read_string_in_request(std::string::iterator it, unsigned int i)
		{
			std::string		str;
			bool			format = false;
			bool			index = false;
			std::string		params;

			if (i == 1)
				index = true;
			while (it != this->request.end() && *it != ' ' && *it != '\t' && *it != '\r' && *it != '\n' && !(index && *it == '?'))
			{
				str.append(1, *it);
				format = true;
				++it;
			}
			if (index && *it == '?')
			{
				if (this->get_method().compare("GET"))
					throw (FormatException());
				while (it != this->request.end() && *it != ' ' && *it != '\t' && *it != '\r' && *it != '\n')
				{
					params.append(1, *it);
					format = true;
					++it;
				}
				this->set_params(params);
			}
			if (!format && i != 2)
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

			for (unsigned int i = 0; i < 3; i++)
			{
				it = this->skip_space(it);
				switch (i)
				{
					case 0 :
						it += (this->method = this->read_string_in_request(it, i)).length();
						break;
					case 1:
						it += (this->source = this->read_string_in_request(it, i)).length() + this->get_params().length();
						if (!params.empty())
							params.erase(params.begin());
						break;
					case 2:
						it += (this->version = this->read_string_in_request(it, i)).length();
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

		bool	is_number(std::string const & str)
		{
			return (str.find_first_not_of("0123456789") == std::string::npos);
		}

		void	assign_valid_fields(std::map<std::string, std::string> & fields)
		{
			if (!fields["connection"].compare("keep-alive"))
				this->set_connection(1);
			else
				this->set_connection(0);
			this->set_content_type(fields["content-type"]);
			if (this->is_number(fields["content-length"]))
				this->set_content_length(std::atol(fields["content-length"].c_str()));
		}

		void	print_header(void) const
		{
			std::cout << "header: " << std::endl;
			std::cout << "Connection: " << this->get_connection() << std::endl;
			std::cout << "Content-Type: " << this->get_content_type() << std::endl;
			std::cout << "Content-length: " << this->get_content_length() << std::endl;
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
			it = skip_end_line(it);
			return (it);
		}

		std::string::iterator		parse_body(std::string::iterator const & it)
		{
			std::string			body;

			body.append(it, this->request.end());
			this->set_body(body);
			return (it);
		}

};
