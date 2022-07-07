#include "request.hpp"
#include "response.hpp"

bool	index_exist(const std::string source)
{
	(void)source;

	// Need parsing section

	return (false);
}

void	execute_request(std::string request)
{
	size_t			i;
	Request			*req;
	Response		*res;

	// Need parsing section:
	
	std::string		root("html");
	std::string		index("index.html index.php");

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
		req = new Get(request);
		std::cout << req->get_request() << std::endl << std::endl;
		if (index_exist(req->get_source()))
		{
			res = new Response(200, req->get_source(), req->get_version());
			res->print_response();
		}
		else
		{
			res = new Response(404, req->get_source(), req->get_version());
			res->print_response();
		}
	}
	else if (!request.compare(i, 4, "POST"))
	{
		req = new Post(request);
		std::cout << req->get_type() << " " << req->get_source() << std::endl;
	}
	else if (!request.compare(i, 6, "DELETE"))
	{
		req = new Delete(request);
		std::cout << req->get_type() << " " << req->get_source() << std::endl;
	}
	else
		std::cout << "unknown request type" << std::endl;
}
