#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#include "Request.hpp"

#define CGI_ENV_NB 3

class Cgi
{

	public :

		Cgi()
		{
		}

		~Cgi()
		{
			size_t		i;

			i = 0;
			while (this->get_envp()[i] != NULL)
			{
				delete [] this->get_envp()[i];
				i++;
			}
			delete [] this->get_envp();
		}

		Cgi&	operator=(Cgi const & rhs)
		{
			this->set_cgi_env(rhs.get_cgi_env());
			this->set_envp(rhs.get_envp());
			return (*this);
		}

		Cgi(Cgi const & rhs)
		{
			this->set_cgi_env(rhs.get_cgi_env());
			this->set_envp(rhs.get_envp());
		}

		Cgi(std::string const & script_path, Request const & request)	
		{
			this->set_cgi_env(this->init_cgi_env("200", request.get_method(), script_path, request.get_params()));
			this->init_envp();
		}

		std::string		read_file(FILE * file) const
		{
			long int		size;
			char			*buf;
			std::string		content;

			fseek(file, 0, SEEK_END);
			size = ftell(file);
			rewind(file);
			buf = new char [size + 1];
			fread(buf, 1, size, file);
			buf[size] = '\0';
			content = std::string(buf);
			delete [] buf;
			return (content);
		}

		std::string		exec_script(void) const
		{
			int				pid;
			char			**argv = NULL;
			FILE			*buf = tmpfile();
			FILE			*err_log = tmpfile();
			int				fds[2];
			std::string		script_executed;

/*
			char **envp = this->get_envp();
			int i = 0;
			std::cout  << "ENVP = "  << std::endl;
			while (envp[i] != NULL)
			{
				std::cout << envp[i] << std::endl;
				i++;
			}
			*/
			fds[0] = fileno(buf);
			fds[1] = fileno(err_log);
			pid = fork();
			if (!pid)
			{
				dup2(fds[0], 1);
				dup2(fds[1], 2);
				if (execve("/usr/bin/php-cgi", argv, this->get_envp()) == -1)
					exit(0);
			}
			wait(NULL);
			script_executed = this->read_file(buf);
			script_executed.erase(0, 42);
			fclose(buf);
			fclose(err_log);
			return (script_executed);
		}

		// getters

		std::map<std::string, std::string>		get_cgi_env(void) const
		{
			return (this->cgi_env);
		}

		char**		get_envp(void) const
		{
			return (this->envp);
		}

		// setters

		void		set_cgi_env(std::map<std::string, std::string> const & cgi_env)
		{
			this->cgi_env = cgi_env;
		}

		void		set_envp(char **envp)
		{
			this->envp = envp;
		}

	private :

		// environment variables

		std::map<std::string, std::string>		cgi_env;

		char**			envp;

		std::map<std::string, std::string>		init_cgi_env(std::string const & redirect_status,
				std::string const & request_method, std::string const & script_filename, std::string const & query_string)
		{
			std::map<std::string, std::string>		cgi_env;

			cgi_env["REDIRECT_STATUS"] = redirect_status;
			cgi_env["REQUEST_METHOD"] = request_method;
			cgi_env["SCRIPT_FILENAME"] = script_filename;
			cgi_env["QUERY_STRING"] = query_string;
			return (cgi_env);
		}

		void			init_envp(void)
		{
			std::map<std::string, std::string>		cgi_env = this->get_cgi_env();
			char		**envp = new char *[cgi_env.size() + 1];
			size_t		i;

			i = 0;
			for (std::map<std::string, std::string>::const_iterator it = cgi_env.begin();
					it != cgi_env.end(); ++it)
			{
				envp[i] = new char [it->first.size() + it->second.size() + 1 + 1];
				strcpy(envp[i], (it->first + '=' + it->second).c_str());
				i++;
			}
			envp[i] = NULL;
			this->set_envp(envp);
		};
};
