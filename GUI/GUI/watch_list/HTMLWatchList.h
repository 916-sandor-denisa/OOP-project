#pragma once
#include "watch_list.h"

class HTMLWatchList : public WatchList {
public:
	HTMLWatchList(const std::string& user_filename);
	~HTMLWatchList();
	void write_to_file_user() override;
	void open() override;
};
