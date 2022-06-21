#pragma once

#include <string>
#include <iostream>

class Request
{
	public :

		Request() {}

		Request(std::string const & request) : request(request) {}

		virtual ~Request() {}

		Request(Request const & src) : source(src.source)
		{
		}

		Request&		operator=(Request const & rhs)
		{
			this->source = rhs.source;
			return (*this);
		}

		// Getters

		std::string			get_type(void) const
		{
			return (this->type);
		}

		const std::string			&get_request(void) const
		{
			return (this->request);
		}

		const std::string			&get_source(void) const
		{
			return (this->source);
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

		void	set_source(std::string const & source)
		{
			this->source = source;
		}

	protected :
		
		std::string		request;
		std::string				type;
		std::string				source;

		size_t			size_elem(std::string request)
		{
			size_t		pos;

			if ((pos = request.find(' ')) != std::string::npos)
			{
				return (pos);
			}
			pos = request.find('\0');
			return (pos);
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
};

class Get : public Request
{
	public :

	Get(std::string request) : Request(request)
	{
		this->set_type("GET");
		this->set_source(this->get_elem_at(2));
	}
};

void	execute_request(std::string request);
