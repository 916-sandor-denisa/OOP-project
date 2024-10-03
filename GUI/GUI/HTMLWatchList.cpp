#include "HTMLWatchList.h"
#include "watch_list.h"
#include "repo_exception.h"
#include <Windows.h>
#include <shellapi.h>

HTMLWatchList::HTMLWatchList(const std::string& filename) : WatchList(filename) {}

HTMLWatchList::~HTMLWatchList() {}

void HTMLWatchList::write_to_file_user()
{
	std::ofstream f(this->user_filename);

	f << "<!DOCTYPE html>" << "\n";
	f << "<html>" << "\n";

	f << "<head>" << "\n";
	f << "<title>Tutorial Watch List</title>" << "\n";
	f << "</head>" << "\n";

	f << "<body>" << "\n";

	f << "<table border=\"1\">" << "\n";

	f << "<tr>" << "\n";
	f << "<td>Title</td>" << "\n";
	f << "<td>Presenter</td>" << "\n";
	f << "<td>Duration</td>" << "\n";
	f << "<td>Likes</td>" << "\n";
	f << "<td>Link</td>" << "\n";
	f << "</tr>" << "\n";

	for (auto i : this->data_user)
	{
		f << "<tr>" << "\n";

		f << "<td>" << i.get_title() << "</td>" << "\n";
		f << "<td>" << i.get_presenter() << "</td>" << "\n";
		f << "<td>" << i.get_duration().first << i.get_duration().second << "</td>" << "\n";
		f << "<td>" << i.get_likes() << "</td>" << "\n";
		f << "<td><a href =" << i.get_link() << ">Link</a></td>";

		f << "</tr>" << "\n";
	}

	f << "</table>" << "\n";

	f << "</body>" << "\n";

	f << "</html>" << "\n";
	f.close();
}

void HTMLWatchList::open()
{
	std::string link = std::string("start ").append(this->user_filename.c_str());
	system(link.c_str());
}
