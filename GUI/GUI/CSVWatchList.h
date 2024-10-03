#pragma once
#include "watch_list.h"

class CSVWatchList : public WatchList {
public:
	CSVWatchList(const std::string& user_filename);
	~CSVWatchList();
	void write_to_file_user() override;
	void open() override;
};