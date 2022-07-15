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
			this->req = new Request(request);
			this->res = new Response(this->req);
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
