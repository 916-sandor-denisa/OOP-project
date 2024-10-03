#include "repo_exception.h"

RepoException::RepoException() : exception{}, msg{ "" } {}

RepoException::RepoException(const std::string& msg) : msg{ msg } {}

RepoException::~RepoException() {}

const char* RepoException::what()
{
	return this->msg.c_str();
}

const char* DuplicateRepoException::what()
{
	return "\nThere is another tutorial with the same title and presenter!\n";
}

FileException::FileException(const std::string& msg) : msg(msg) {}

const char* FileException::what()
{
	return msg.c_str();
}
