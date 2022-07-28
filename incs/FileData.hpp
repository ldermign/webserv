#pragma once

#include <string>
#include <ctime>
#include <sys/stat.h>

class FileData
{
	public :

		FileData()
		{

		}

		FileData(std::string path, std::string name) : path(path + name), name(name)
		{
			this->set_spec(this->init_spec());
			this->set_edit_date(this->init_edit_date());
			this->set_size(this->init_size());
		}

		FileData(FileData const & src)
		{
			this->set_name(src.get_name());
			this->set_edit_date(src.get_edit_date());
			this->set_size(src.get_size());
			this->set_path(src.get_path());
		}

		FileData&		operator=(FileData const & rhs)
		{
			this->set_name(rhs.get_name());
			this->set_edit_date(rhs.get_edit_date());
			this->set_size(rhs.get_size());
			this->set_path(rhs.get_path());
			return (*this);
		}

		virtual ~ FileData()
		{

		}

		void		set_name(std::string const & name)
		{
			this->name = name;
		}

		void		set_edit_date(std::string const & edit_date)
		{
			this->edit_date = edit_date;
		}

		void		set_size(size_t size)
		{
			this->size = size;
		}

		void		set_spec(struct stat const & spec)
		{
			this->spec = spec;
		}

		void		set_path(std::string const & path)
		{
			this->path = path;
		}


		std::string		get_name(void) const
		{
			return (this->name);
		}

		std::string		get_edit_date(void) const
		{
			return (this->edit_date);
		}

		std::string		get_path(void) const
		{
			return (this->path);
		}

		size_t			get_size(void) const
		{
			return (this->size);
		}

		struct stat get_spec(void) const
		{
			return (this->spec);
		}

		struct stat init_spec(void) const
		{
			struct stat		spec;

			stat(this->get_path().c_str(), &spec);
			return (spec);
		}

		std::string		init_edit_date(void) const
		{
			std::string		edit_date;

			edit_date = std::ctime(&spec.st_mtime);
			edit_date.erase(edit_date.end() - 1);
			return (edit_date);
		}

		size_t		init_size(void)
		{
			return (spec.st_size);
		}

	private :

		std::string		path;
		std::string		name;
		std::string		edit_date;
		size_t			size;
		struct stat		spec;
};
