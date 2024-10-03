#pragma once
#include "domain.h"
#include "repo.h"
#include <exception>

class RepoException : public std::exception {
protected:
	std::string msg;

public:
	RepoException();
	RepoException(const std::string& msg);
	virtual ~RepoException();
	virtual const char* what();
};

class FileException : public std::exception {
protected:
	std::string msg;
public:
	FileException(const std::string& msg);
	virtual const char* what();
};


class DuplicateRepoException : public RepoException {
public:
	const char* what();
};