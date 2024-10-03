#include "domain.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>

Tutorial::Tutorial()
{
	this->title = "";
	this->presenter = "";
	this->duration.first = 0;
	this->duration.second = 0;
	this->likes = 0;
	this->link = "";
}

Tutorial::Tutorial(std::string title, std::string presenter, Duration duration, int likes, std::string link)
{
	this->title = title;
	this->presenter = presenter;
	this->duration = duration;
	this->likes = likes;
	this->link = link;
}

Tutorial::~Tutorial() {}

std::string Tutorial::get_title() const
{
	return this->title;
}

std::string Tutorial::get_presenter() const
{
	return this->presenter;
}

Duration Tutorial::get_duration() const
{
	return this->duration;
}

int Tutorial::get_likes() const
{
	return this->likes;
}

std::string Tutorial::get_link() const
{
	return this->link;
}

void Tutorial::set_title(std::string& new_title)
{
	this->title = new_title;
}

void Tutorial::set_presenter(std::string& new_presenter)
{
	this->presenter = new_presenter;
}

void Tutorial::set_duration(Duration new_duration)
{
	this->duration = new_duration;
}

void Tutorial::set_likes(int new_likes)
{
	this->likes = new_likes;
}

void Tutorial::set_link(std::string& new_link)
{
	this->link = new_link;
}

bool Tutorial::operator==(Tutorial t)
{
	if (this->title != t.title || this->presenter != t.presenter || this->likes != t.likes || this->link != t.link || this->duration != t.duration)
		return false;
	return true;
}

Tutorial& Tutorial::operator=(const Tutorial& t)
{
	this->title = t.title;
	this->presenter = t.presenter;
	this->likes = t.likes;
	this->link = t.link;
	this->duration = t.duration;
	return *this;
}

std::ostream& operator<<(std::ostream& output, const Tutorial& tutorial)
{
	output << tutorial.title << "," << tutorial.presenter << "," << tutorial.duration.first << "," << tutorial.duration.second << ","
		<< tutorial.likes << "," << tutorial.link << "\n";
	return output;
}

std::vector<std::string> tokenize(const std::string& str, char delimiter) {
	std::vector<std::string> res;
	std::stringstream s(str);
	std::string token;
	while (getline(s, token, delimiter))
		res.push_back(token);

	return res;
}

std::istream& operator>>(std::istream& input, Tutorial& t)
{
	std::string line;
	std::getline(input, line);
	std::vector<std::string> tokens;
	if (line.empty())
		return input;
	tokens = tokenize(line, ',');
	t.title = tokens[0];
	t.presenter = tokens[1];
	t.duration.first = std::stoi(tokens[2]);
	t.duration.second = std::stoi(tokens[3]);
	t.likes = std::stoi(tokens[4]);
	t.link = tokens[5];
	return input;
}
