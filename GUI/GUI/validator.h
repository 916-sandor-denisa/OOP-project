#pragma once
#include "domain.h"
#include <vector>
#include <exception>

class Validator {
public:
	Validator();
	~Validator();

	void validate_duration(const std::string& minutes, const std::string& seconds);
	bool validate_number(const std::string& num);
	void validate_tutorial_presenter(const std::string& input);
	void validate_likes(const std::string& input);
	void validate_link(const std::string& input);
};

class TutorialException : public std::exception {
protected:
	std::string msg;

public:
	TutorialException(std::string& msg) : msg{ msg } {};
	~TutorialException() {};
	const char* what();
};