#pragma once
#include <iostream>
#include "repo.h"
#include "watch_list.h"
#include "CSVWatchList.h"
#include "HTMLWatchList.h"
#include "Action.h"
#include <stack>

class Service {
private:
	Repo* repo;
	WatchList* watch_list;

	std::vector<Action*> undoStackAdminMode;
	std::vector<Action*> redoStackAdminMode;
	std::vector<Action*> undoStackUserMode;
	std::vector<Action*> redoStackUserMode;
public:
	//constructor
	Service(Repo* repo, WatchList* watch_list);

	void undoLastAction(std::vector<Action*>& currentUndoStack, std::vector<Action*>& currentRedoStack);
	void redoLastAction(std::vector<Action*>& currentUndoStack, std::vector<Action*>& currentRedoStack);
	void undoAdminMode();
	void redoAdminMode();

	//function to add an element for the service layer
	void add_tutorial_admin(std::string title, std::string presenter, Duration duration, int likes, std::string link);

	//function to remove an element for the service layer
	void remove_tutorial_service(std::string title, std::string presenter);

	//function to update an element for the service layer
	void update_tutorial_service(std::string old_title, std::string old_presenter, Tutorial new_t);

	//function to get the number of elements for the service layer
	int get_size_service();

	//function to get a certain element
	Tutorial get_tutorial_service(std::string title, std::string presenter);

	//function to get all the elements from the repo
	std::vector<Tutorial> get_all_tutorials_service();

	//function to initialize the repository with 10 entities
	//void initialize_list_admin();

	//function to get the tutorials from the watch list
	std::vector<Tutorial> get_watch_list_service();

	//function to get the size of the watch list
	int get_watch_list_size();

	//function to add a tutorial to the watch list in the service
	void add_tutorial_to_watch_list(Tutorial t);

	//function to watch a tutorial (to remove it from the watch list after the user watches the tutorial)
	void watch_tutorial_service(std::string title, std::string presenter);

	//function to increment the number of likes by one if the user decides to
	void like_tutorial(std::string title, std::string presenter);

	//function to search through the tutorials by a given presenter
	std::vector<Tutorial> search_by_presenter_service(std::string presenter);

	//function to get all the titles
	std::vector<std::string> get_all_presenters_service();

	//function to compute the number of tutorials a presenter has
	int numberOfTutorialsPerPresenter(const std::string& presenter);

	void open();
};