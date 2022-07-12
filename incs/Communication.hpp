#pragma once

#include "response.hpp"
#include "request.hpp"

class Communication
{
	public :

		Communication(std::string request)
		{
			size_t			i;
			std::string		root("html");
			std::string		index("index.html index.php");

//			std::cout << request << std::endl;
			i = 0;
			while (i < request.size())
			{
				if (request.at(i) == ' ')
					++i;
				else
					break ;
			}
			if (!request.compare(i, 3, "GET"))
			{
				this->req = new Get(request);
				if (this->index_exist(this->req->get_source()))
				{
					this->res = new Response(200, this->req->get_source(), this->req->get_version());
				}
				else
				{
					this->res = new Response(404, this->req->get_source(), this->req->get_version());
				}
			}
			else if (!request.compare(i, 4, "POST"))
			{
				this->req = new Post(request);
			}
			else if (!request.compare(i, 6, "DELETE"))
			{
				this->req = new Delete(request);
			}
			else
				std::cout << "unknown this->request type" << std::endl;
		}

		std::string		get_response(void) const
		{
			return (res->get_response());
		}

	private :

		Request		*req;
		Response	*res;

		bool	index_exist(const std::string source)
		{
			(void)source;

			// Need parsing section

			return (false);
		}
};
