#include "request.hpp"

void	execute_request(std::string request)
{
	size_t			i;
	Request			*req;

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
		std::cout << req->get_type() << " " << req->get_source() << std::endl;
	}
}
