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

};
