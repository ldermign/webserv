#include "FtServer.hpp"
#include <sys/select.h>
#include <ctime>
#define WRITE 100
#define ACCEPT 101
#define RECV 102
#define BUFF_SIZE 4096

typedef struct	s_bunch_of_fds
{
	int				fd;
	int				flag;
	std::string		&data;
}				t_bunch_of_fds;

/* TO-DO :
 * 			- review	-> revoie tout le programme avant de l'utiliser
 *			- send		-> ce qu'on fait des fds deja utiliser
*/

int		action_to_socket(int fd, std::string s1, int arg)
{
	static fd_set			set_r;
	static fd_set			set_w;
	static fd_set			set_e;
	int						ret_select;
	static t_bunch_of_fds		fds[1024] = (bunch_of_fds[1024]) memset(&fds, 0, sizeof(fds));
	struct timeval			tv = {.tv_sec = 0, .tv_usec = 5000};
	std::runtime_error		expt("Error occured");
	std::vector<t_bunch_of_fds>	ret;

	if (flag != RECV && flag != WRITE && flag != ACCEPT)
		throw expt;
	FD_SET(fd, &set_r);
	FD_SET(fd, &set_w);
	FD_SET(fd, &set_e);
	for (int i = 0; i < 1024; i++)
	{
		fds[i] = (fds[i] == {0}) ? {.fd = fd, .flag = arg, .data = s1} : {0};
		if (fds[i].fd == fd)
			break ;
	}
	ret_select = select(&set_r, &set_w, &set_e, &tv);
	for (int i = 0; i < 1024; i++)
	{
		int						ret_func;
		struct sockaddr_storage their_addr;
		t_bunch_of_fds			curr = fds[i];
		t_bunch_of_fds			new_one;
		std::vector<char>			buff(BUFF_SIZE);
		std::string				rcv;

		if (curr == {0} || FD_ISSET(curr.fd, &set_e)
			|| ((curr.flag == RECV || curr.flag == ACCEPT) && !FD_ISSET(curr.fd, &set_r))
			|| (curr.flag == WRITE && !FD_ISSET(curr.fd, &set_w)))
			continue ;
		if (curr.flag == ACCEPT)
		{
			ret_func = accept(fds[i].fd, &their_addr, sizeof their_addr);
			if (ret_func == -1)
				continue ;
			for (int j = 0; j < 1024; j++)
			{
				fds[j] = (fds[j] == {0}) ? {.fd = fd, .flag = RECV, .data = ""} : {0};
				if (fds[j].fd == fd)
					break ;
			}
		}
		else if (curr.flag == RECV)
		{
			while ((ret_func = recv(curr.fd, &buff[0], buff.size(), 0)) > 0)
			{
				if (ret_func == -1)
					continue ;
				rcv.append(buff.cbegin(), buff.cend());
			}
			new_one = {.fd = curr.fd, .flag = SEND, .data = rcv};
			ret.push_back(new_one);
		}
		else if (curr.flag == WRITE)
		{
			ret_func = send(curr.fd, curr.data, curr.data.size(), 0);
			if (ret_func == -1)
				continue ;
		}
		fds[i] = {0};
	}
	SET_ZERO(&set_r);
	SET_ZERO(&set_w);
	SET_ZERO(&set_e);
	return ret;
}
