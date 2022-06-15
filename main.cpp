#include <iostream>

int	main(int argc, char **argv)
{
	if (argc > 1)
	{
		std::cerr << "Error: " << "Only a single configuration file expected" \
			<< std::endl;
	}
	return (0);
}
