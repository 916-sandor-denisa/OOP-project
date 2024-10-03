#pragma once

#include <iostream>

typedef std::pair<int, int> Duration;

class Tutorial {
private:
	std::string title;
	std::string presenter;
	Duration duration;
	int likes = 0;
	std::string link;

public:
	//Default constructor
	Tutorial();
	//Constructor with parameters
	Tutorial(std::string title, std::string presenter, Duration duration, int likes, std::string link);
	//Destructor
	~Tutorial();

	//Getters
	std::string get_title() const;
	std::string get_presenter() const;
	Duration get_duration() const;
	int get_likes() const;
	std::string get_link() const;

	//Setters
	void set_title(std::string& new_title);
	void set_presenter(std::string& new_presenter);
	void set_duration(Duration new_duration);
	void set_likes(int likes);
	void set_link(std::string& new_link);

	//Function that overloads the operartor ==
	bool operator==(Tutorial t);

	//Function that overloads the operartor =
	Tutorial& operator=(const Tutorial& t);

	friend std::ostream& operator<<(std::ostream& output, const Tutorial& tutorial);

	friend std::istream& operator>>(std::istream& input, Tutorial& tutorial);
};
