#pragma once

#define	END_RES_LINE "\r\n"

#include <map>
#include <iostream>
#include <string>
#include <sstream>

#include <dirent.h>

#include "FileData.hpp"
#include "Request.hpp"

class Autoindex
{
	public :

		Autoindex()
		{

		}

		virtual ~Autoindex()
		{

		}

		Autoindex(Autoindex const & rhs) : request(rhs.get_request()), index_path(rhs.get_index_path())
		{
		}

		Autoindex&		operator=(Autoindex const & rhs)
		{
			this->set_request(rhs.get_request());
			this->set_index_path(rhs.get_index_path());
			return (*this);
		}

		Autoindex(Request const & request, std::string const & index_path) : request(request), index_path(index_path)
		{

		}

		// Setters
		
		void				set_request(Request const & request)
		{
			this->request = request;
		}

		void				set_index_path(std::string const & index_path)
		{
			this->index_path = index_path;
		}

		// Getters

		std::string			get_index_path(void) const
		{
			return (this->index_path);
		}

		Request				get_request(void) const
		{
			return (this->request);
		}
	
		std::string		create_autoindex(void)
		{
				std::stringstream						ss;
				std::map<std::string, FileData>			content;
				const size_t							first_space_gap = 40;
				const size_t							second_space_gap = 60;

				content = get_content_dir(this->get_index_path());
				ss << "<html>" << END_RES_LINE;
				ss << "<head><title>Index of " << this->get_request().get_source() << "</title></head>" << END_RES_LINE;
				ss << "<body>" << END_RES_LINE;
				ss << "<h1>Index of " << this->get_request().get_source() << "</h1><hr><pre>" << END_RES_LINE;
				for (std::map<std::string, FileData>::iterator it = content.begin(); it != content.end(); ++it)
				{
					std::cout << "FIRST = " << it->first << std::endl;
					if (!this->is_dir(it->second.get_path()))
						continue ;
					ss << "<a href=" << this->link_autoindex(it->first) << ">";
					ss << this->index_in_autoindex(it->first, first_space_gap, true);
					if (it->first.length() < first_space_gap)
						ss << "/";
					ss << "</a>";
					ss << this->add_spaces(first_space_gap, it->second.get_name().length() + 1);
					ss << it->second.get_edit_date();
					ss << this->add_spaces(second_space_gap, it->second.get_edit_date().length());
					ss << "-" << END_RES_LINE;
				}

				for (std::map<std::string, FileData>::iterator it = content.begin(); it != content.end(); ++it)
				{
					if (this->is_dir(it->second.get_path()))
						continue ;
					ss << "<a href=" << this->link_autoindex(it->first) << ">";
					ss << this->index_in_autoindex(it->first, first_space_gap, false) << "</a>";
					ss << this->add_spaces(first_space_gap, it->second.get_name().length());
					ss << it->second.get_edit_date();
					ss << this->add_spaces(second_space_gap, it->second.get_edit_date().length());
					ss << it->second.get_size() << END_RES_LINE;
				}
				ss << "</pre><hr></body>" << END_RES_LINE;
				ss << "</html>" << END_RES_LINE;
				return (ss.str());
		}

	private :

		Request				request;
		std::string			index_path;

		std::map<std::string, FileData>			get_content_dir(std::string const & dir_name) const
		{
			std::map<std::string, FileData>		content_dir;
			DIR						*dir;
			struct dirent			*diread;

			dir = opendir(dir_name.c_str());
			if (dir)
			{
				while ((diread = readdir(dir)))
				{
					FileData					file(dir_name, diread->d_name);

					content_dir[diread->d_name] = file;
				}
			}
			return (content_dir);
		}

		std::string		add_spaces(unsigned int n, size_t offset)
		{
			std::string		spaces;

			if (n < offset)
				offset = n - 1;
			spaces.append(n - offset, ' ');

			return (spaces);
		}

		std::string		index_in_autoindex(std::string const & index, const size_t space_gap, bool dir)
		{
			std::string			trimmed_index;

			if (index.length() >= space_gap)
			{
				trimmed_index = index.substr(0, space_gap - 4);
				if (dir)
					trimmed_index.append("../");
				else
					trimmed_index.append("...");
				return (trimmed_index);
			}
			return (index);
		}

		std::string		link_autoindex(std::string const & dest) const
		{
			std::string			link;

			link = this->get_request().get_source();
			if (*(link.end() - 1) != '/')
				link.append(1, '/');
			link.append(dest);
			return (link);
		}

		bool	is_dir(std::string const & path) const
		{
			struct stat buffer;

			return (stat(path.c_str(), &buffer) == 0 && buffer.st_mode & S_IFDIR);
		}
};
