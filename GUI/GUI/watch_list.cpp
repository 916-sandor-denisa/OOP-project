#include "watch_list.h"
#include "repo_exception.h"

WatchList::WatchList(const std::string& user_filename) {
	this->user_filename = user_filename;
}

WatchList::~WatchList() {}

void WatchList::add_to_watch_list(Tutorial t)
{
	for (auto tutorial : this->data_user)
		if (tutorial.get_title() == t.get_title() && tutorial.get_presenter() == t.get_presenter())
			throw DuplicateRepoException();
	this->data_user.push_back(t);
	this->write_to_file_user();
}

void WatchList::remove_from_watch_list(std::string title, std::string presenter)
{
	for (int i = 0; i < this->data_user.size(); i++) {
		if (this->data_user[i].get_title() == title && this->data_user[i].get_presenter() == presenter) {
			this->data_user.erase(this->data_user.begin() + i);
			this->write_to_file_user();
			return;
		}
	}
}

Tutorial WatchList::search_tutorial_watch_list(std::string title, std::string presenter)
{
	for (auto tutorial : this->data_user)
		if (tutorial.get_title() == title && tutorial.get_presenter() == presenter)
			return tutorial;
	return Tutorial{};
}

int WatchList::get_watch_list_size()
{
	return this->data_user.size();
}

std::vector<Tutorial> WatchList::get_whole_watch_list()
{
	return this->data_user;
}