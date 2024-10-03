#include "CSVWatchList.h"
#include "watch_list.h"
#include "repo_exception.h"

CSVWatchList::CSVWatchList(const std::string& filename) : WatchList(filename) {}

CSVWatchList::~CSVWatchList() {}

void CSVWatchList::write_to_file_user()
{
	std::ofstream file(this->user_filename);
	if (!file.is_open())
		throw FileException("The file could not be opened!\n");
	for (auto tutorial : this->data_user) {
		file << tutorial;
	}
	file.close();
}

void CSVWatchList::open()
{
	std::string command = "start excel \"" + this->user_filename + "\"";

	int result = system(command.c_str());
}

