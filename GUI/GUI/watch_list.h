#pragma once
#include <vector>
#include "domain.h"
#include <fstream>


class WatchList {
protected:
	std::vector<Tutorial> data_user;
	std::string user_filename;
public:
	//constructor
	WatchList(const std::string& filename);

	//destructor
	~WatchList();

	//adds a tutorial to the watch list
	void add_to_watch_list(Tutorial t);

	//removes a tutorial from the watch list
	void remove_from_watch_list(std::string title, std::string presenter);

	//searches a tutorial from the watch list by title and presenter
	Tutorial search_tutorial_watch_list(std::string title, std::string presenter);

	//gets the number of elements from the watch list
	int get_watch_list_size();

	//gets the entire watch list
	std::vector<Tutorial> get_whole_watch_list();

	virtual void open() = 0;
	virtual void write_to_file_user() = 0;
};
