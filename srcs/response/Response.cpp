#include "Response.hpp"

std::vector<std::string>		Response::default_methods = init_default_methods();
std::vector<int>				Response::redirection_status = init_redirection_status();
std::map<int, std::string>		Response::status_messages = init_status_messages();
