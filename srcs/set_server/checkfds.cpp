#include "webserv.hpp"

typedef struct	s_bunch_of_fds
{
	int				fd;
	int				flag;
	std::string		data;
}				t_bunch_of_fds;

/* TO-DO :
 * list of what this function have to do : 
 * 											-> store all fds with in function
 * 											-> select them
 * 											-> if fd are okay, choose accept, send or recv
 * 											-> return list of messages recv
 * 											-> delete all fds that where executed
*/

std::vector<t_bunch_of_fds>		action_to_socket(t_bunch_of_fds x)
{
	static fd_set			set_r;
	static fd_set			set_w;
	static fd_set			set_e;
	int						ret_select;
	int						h_fd = 0;
	static t_bunch_of_fds	fds[1024];
	struct timeval			tv = {.tv_sec = 0, .tv_usec = 5000};
	std::runtime_error		expt("Error occured");
	std::vector<t_bunch_of_fds>	ret;

	if (arg != RECV && arg != SEND && arg != ACCEPT)
		throw expt;
	for (int i = 0; i < 1024; i++)
	{
		FD_SET(fd, &set_r);
		FD_SET(fd, &set_w);
		FD_SET(fd, &set_e);
		fds[i] = (fds[i].fd == 0) ? x : fds[i];
		if (fds[i].fd == fd)
			break ;
		h_fd = (fds[i].fd > h_fd) ? fds[i].fd : h_fd;
	}

	ret_select = select(h_fd, &set_r, &set_w, &set_e, &tv);
	for (int i = 0; i < 1024; i++)
	{
		int						ret_func;
		struct sockaddr_storage their_addr;
		t_bunch_of_fds			curr = fds[i];
		std::vector<char>			buff(BUFF_SIZE);
		std::string				rcv;

		if (curr.fd == 0 || FD_ISSET(curr.fd, &set_e)
			|| ((curr.flag == RECV || curr.flag == ACCEPT) && !FD_ISSET(curr.fd, &set_r))
			|| (curr.flag == SEND && !FD_ISSET(curr.fd, &set_w)))
			continue ;
		if (curr.flag == ACCEPT)
		{
			socklen_t	s_addr = sizeof(their_addr);
			ret_func = accept(fds[i].fd, (struct sockaddr *)&their_addr, &s_addr);
			if (ret_func == -1)
				continue ;
			for (int j = 0; j < 1024; j++)
			{
				if (fds[j].fd == 0)
				{
					t_bunch_of_fds	help = {fd, RECV, ""};
					fds[j] = help;
					break ;
				}
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
			t_bunch_of_fds new_one = {.fd = curr.fd, .flag = SEND, .data = rcv};
			ret.push_back(new_one);
		}
		else if (curr.flag == SEND)
		{
			ret_func = send(curr.fd, curr.data.c_str(), curr.data.size(), 0);
			if (ret_func == -1)
				continue ;
		}
		fds[i].fd = 0;
	}
	FD_ZERO(&set_r);
	FD_ZERO(&set_w);
	FD_ZERO(&set_e);
	return ret;
}
