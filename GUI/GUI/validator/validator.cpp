#include "validator.h"
#include <string>

Validator::Validator() {}

Validator::~Validator() {}

bool Validator::validate_number(const std::string& num)
{
	for (char c : num)
		if (isdigit(c) == false)
			return false;
	return true;
}

void Validator::validate_tutorial_presenter(const std::string& input)
{
	std::string errors;
	if (input.empty() == true)
		errors += std::string("\nThe fields must not be empty!\n");
	if (errors.size() != 0)
		throw TutorialException(errors);
}

void Validator::validate_duration(const std::string& minutes, const std::string& seconds)
{
	std::string errors;
	if (validate_number(minutes) == false || validate_number(seconds) == false) {
		errors += std::string("\nThe number of minutes and seconds must be an integers!\n");
	}
	else if (std::stoi(minutes) < 0 || std::stoi(seconds) > 60 || std::stoi(minutes) < 0)
		errors += std::string("\nThe duration must have a valid number of minutes and seconds!\n");
	if (!errors.empty())
		throw TutorialException(errors);
}

void Validator::validate_likes(const std::string& input)
{
	std::string errors;
	if (validate_number(input) == false) {
		errors += std::string("\nThe number of likes must be an integer!\n");
	}
	else if (std::stoi(input) < 0)
		errors += std::string("\nThe number of likes must be positive!\n");
	if (errors.size() != 0)
		throw TutorialException(errors);
}

void Validator::validate_link(const std::string& input)
{
	std::string errors;
	if (input.empty() == true)
		errors += std::string("\nThe link must not be an empty field!\n");
	if (errors.size() != 0)
		throw TutorialException(errors);
}

const char* TutorialException::what()
{
	return msg.c_str();
}
