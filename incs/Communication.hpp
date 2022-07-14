#pragma once

#include "Response.hpp"
#include "Request.hpp"

#include <string>
#include <vector>

class Communication
{
	public :

		Communication(std::string request)
		{
			////////////////////////////////
			//							  //
			// waiting for liena's parsing//
			//							  //
			////////////////////////////////

			std::string		root("html");
			std::vector<std::string>	indexes;


			//////////////////////////////////

			indexes.push_back("index.html");
			indexes.push_back("index.php");
			if (!request.compare(0, 3, "GET"))
			{
				this->req = new Get(request);
				if (!this->req->get_format())
					this->res = new Response(400, this->req->get_source(), this->req->get_version());
				else if (this->index_exist(this->req->get_source(), indexes))
				{
					this->res = new Response(200, this->req->get_source(), this->req->get_version());
				}
				else
				{
					this->res = new Response(404, this->req->get_source(), this->req->get_version());
				}
			}
			else if (!request.compare(0, 4, "POST"))
			{
				this->req = new Post(request);
			}
			else if (!request.compare(0, 6, "DELETE"))
			{
				this->req = new Delete(request);
			}
			else
			{
				this->res = new Response(400, this->req->get_source(), this->req->get_version());
			}
		}

		std::string		get_response(void) const
		{
			return (res->get_response());
		}

		Response*		get_res(void) const
		{
			return (this->res);
		}

	private :

		Request		*req;
		Response	*res;

		bool	index_exist(const std::string source, std::vector<std::string> indexes)
		{
			for (std::vector<std::string>::iterator it = indexes.begin(); it != indexes.end(); ++it)
			{
				if (source == *it)
					return (true);
			}
			return (false);
		}
};
