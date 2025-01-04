#include <iostream>
#include "repo.h"

Repo::Repo(const std::string& filename) {
	this->filename = filename;
	this->read_from_file();
}

Repo::~Repo() {}

void Repo::add_tutorial(const Tutorial& t)
{
	if (this->search_tutorial(t.get_title(), t.get_presenter()).get_title() != "")
		throw DuplicateRepoException();
	this->data.push_back(t);
	this->write_to_file();
}

void Repo::remove_tutorial(std::string title, std::string presenter)
{
	for (int i = 0; i < this->data.size(); i++) {
		if (this->data[i].get_title() == title && this->data[i].get_presenter() == presenter) {
			this->data.erase(this->data.begin() + i);
			this->write_to_file();
			return;
		}
	}
	throw RepoException("\nThe tutorial provided does not exist!\n");
}

void Repo::update_tutorial(std::string title, std::string presenter, Tutorial new_tutorial)
{
	for (auto& tutorial : this->data) {
		if (tutorial.get_presenter() == presenter && tutorial.get_title() == title) {
			tutorial = new_tutorial;
			this->write_to_file();
			return;
		}
	}
	throw RepoException("\nThe tutorial provided does not exist!\n");
}

Tutorial Repo::search_tutorial(std::string title, std::string presenter)
{
	for (auto tutorial : this->data)
		if (tutorial.get_title() == title && tutorial.get_presenter() == presenter)
			return tutorial;
	return Tutorial{};
}

int Repo::get_repo_size()
{
	return this->data.size();
}

int Repo::get_repo_capacity()
{
	return this->data.capacity();
}

std::vector<Tutorial> Repo::get_all_tutorials()
{
	if (this->data.size() == 0)
		throw RepoException("\nThere are no tutorials to display!\n");
	return this->data;
}

void Repo::read_from_file()
{
	std::ifstream file(this->filename);
	if (!file.is_open())
		throw FileException("The file could not be opened!\n");
	Tutorial t;
	while (file >> t) {
		this->data.push_back(t);
	}
	file.close();
}

void Repo::write_to_file()
{
	std::ofstream file(this->filename);
	if (!file.is_open())
		throw FileException("The file could not be opened!\n");
	for (auto tutorial : this->data) {
		file << tutorial;
	}
	file.close();
}

std::string Repo::get_filename() const
{
	return this->filename;
}

std::vector<Tutorial> Repo::get_tutorial_by_presenter(std::string presenter)
{
	std::vector<Tutorial> dv;
	for (auto tutorial : this->data) {
		if (tutorial.get_presenter() == presenter) {
			dv.push_back(tutorial);
		}
	}
	if (dv.size() == 0)
		throw RepoException("\nThere are no tutorials with the provided presenter!\n");
	return dv;
}
