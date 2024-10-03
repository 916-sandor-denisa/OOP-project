#pragma once

#include <iostream>
#include <fstream>
#include "domain.h"
#include "repo_exception.h"
#include <vector>

class Repo {
private:
	std::vector<Tutorial> data;
	std::string filename;
public:
	//default constructor
	Repo(const std::string& filename);

	//destructor
	~Repo();

	//function to add an element to the repository
	void add_tutorial(const Tutorial& t);

	//function to remove an element from the repository
	void remove_tutorial(std::string title, std::string presenter);

	//function to uppdate an element in the repository
	void update_tutorial(std::string title, std::string presenter, Tutorial new_tutorial);

	//function to search for an element in the repository
	Tutorial search_tutorial(std::string title, std::string presenter);

	//function to get the size of the repository
	int get_repo_size();

	//function to get the capacity of the repository
	int get_repo_capacity();

	//function to get all the elements of the repository
	std::vector<Tutorial> get_all_tutorials();

	//function to get all the tutorials with a given presenter
	std::vector<Tutorial> get_tutorial_by_presenter(std::string presenter);

	void read_from_file();

	void write_to_file();

	std::string get_filename() const;
};