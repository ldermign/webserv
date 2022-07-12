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
				std::cout << this->req->get_request() << std::endl << std::endl;
				if (this->index_exist(this->req->get_source()))
				{
					this->res = new Response(200, this->req->get_source(), this->req->get_version());
					this->res->print_response();
				}
				else
				{
					this->res = new Response(404, this->req->get_source(), this->req->get_version());
					this->res->print_response();
				}
			}
			else if (!request.compare(i, 4, "POST"))
			{
				this->req = new Post(request);
				std::cout << this->req->get_type() << " " << this->req->get_source() << std::endl;
			}
			else if (!request.compare(i, 6, "DELETE"))
			{
				this->req = new Delete(request);
				std::cout << this->req->get_type() << " " << this->req->get_source() << std::endl;
			}
			else
				std::cout << "unknown this->request type" << std::endl;
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
