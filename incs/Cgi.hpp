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
#include "ResponseHeader.hpp"
#include "Location.hpp"

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

		Cgi(std::string const & script_path, Request const & request, Location const & location) : request(request), location(location)
		{
			this->set_cgi_env(this->init_cgi_env("200", request.get_method(),
						script_path, request));
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

		std::string			create_exe_name(void)
		{
			std::string		php_exe = "php-cgi";

			if (*(this->location.getCgi().second.end() - 1) != '/')
				php_exe.insert(php_exe.begin(), '/');
			return (php_exe);
		}

		std::string		exec_script(void)
		{
			int				pid;
			char			**argv = NULL;
			FILE			*stdout_buf = tmpfile();
			FILE			*stdin_buf = tmpfile();
			FILE			*stderr_buf = tmpfile();
			int				std_streams_fds[3];
			std::string		script_executed;

			std_streams_fds[0] = fileno(stdout_buf);
			std_streams_fds[1] = fileno(stdin_buf);
			std_streams_fds[2] = fileno(stderr_buf);
			std::cout << "body = " << this->get_request().get_body().c_str() << " SIZE = " <<sizeof(this->get_request().get_body().c_str()) << std::endl;
			fwrite(this->get_request().get_body().c_str(), sizeof(char), this->get_request().get_body().size(), stdout_buf);
			fseek(stdout_buf, 0, SEEK_SET);
			pid = fork();
			if (!pid)
			{
				dup2(std_streams_fds[0], 0);
				dup2(std_streams_fds[1], 1);
				dup2(std_streams_fds[2], 2);
				execve((this->get_location().getCgi().second + create_exe_name()).c_str(), argv, this->get_envp());
				std::cout << "error: cannot execute cgi" << std::endl;
				exit(0);
			}
			wait(NULL);
			script_executed = this->read_file(stdin_buf);
			script_executed.erase(0, 42);
			fclose(stdin_buf);
			fclose(stdout_buf);
			fclose(stderr_buf);
			return (script_executed);
		}

		// getters
		
		Request									get_request(void) const
		{
			return (this->request);
		}

		Location								get_location(void) const
		{
			return (this->location);
		}

		std::map<std::string, std::string>		get_cgi_env(void) const
		{
			return (this->cgi_env);
		}

		char**		get_envp(void) const
		{
			return (this->envp);
		}

		// setters
		
		void		set_request(Request const & request)
		{
			this->request = request;
		}

		void		set_location(Location const & location)
		{
			this->location = location;
		}

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

		Request									request;
		Location								location;

		std::map<std::string, std::string>		cgi_env;

		char**			envp;

		std::map<std::string, std::string>		init_cgi_env(std::string const & redirect_status,
				std::string const & request_method, std::string const & script_filename,
				Request const & request)
		{
			std::map<std::string, std::string>		cgi_env;
			std::stringstream						ss;

			cgi_env["REDIRECT_STATUS"] = redirect_status;
			cgi_env["REQUEST_METHOD"] = request_method;
			cgi_env["SCRIPT_FILENAME"] = script_filename;
			cgi_env["QUERY_STRING"] = request.get_params();

			ss << request.get_content_length();

			cgi_env["CONTENT_LENGTH"] = ss.str();
			cgi_env["CONTENT_TYPE"] = request.get_content_type();

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
