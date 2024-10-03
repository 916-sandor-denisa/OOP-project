#include "service.h"
#include <stdlib.h>
#include <stdio.h>

Service::Service(Repo* repo, WatchList* watch_list)
{
	this->repo = repo;
	this->watch_list = watch_list;
}

void Service::undoLastAction(std::vector<Action*>& currentUndoStack, std::vector<Action*>& currentRedoStack)
{
	if (currentUndoStack.empty()) {
		throw std::exception("No more undos!");
	}

	Action* currentAction = currentUndoStack.back();
	currentAction->executeUndo();
	currentRedoStack.push_back(currentAction);
	currentUndoStack.pop_back();
}

void Service::redoLastAction(std::vector<Action*>& currentUndoStack, std::vector<Action*>& currentRedoStack)
{
	if (currentRedoStack.size() == 0) {
		throw std::exception("No more redos!");
	}

	Action* currentAction = currentRedoStack.back();
	currentAction->executeRedo();
	currentUndoStack.push_back(currentAction);
	currentRedoStack.pop_back();
}

void Service::undoAdminMode()
{
	this->undoLastAction(this->undoStackAdminMode, this->redoStackAdminMode);
}

void Service::redoAdminMode()
{
	this->redoLastAction(this->undoStackAdminMode, this->redoStackAdminMode);
}

void Service::add_tutorial_admin(std::string title, std::string presenter, Duration duration, int likes, std::string link)
{
	Tutorial t{ title, presenter, duration, likes, link };
	this->repo->add_tutorial(t);
	Action* currentAction = new AddAction(this->repo, t);
	this->undoStackAdminMode.push_back(currentAction);
	this->redoStackAdminMode.clear();
}

void Service::remove_tutorial_service(std::string title, std::string presenter)
{
	Tutorial t = this->get_tutorial_service(title, presenter);
	Tutorial deleted{ t.get_title(), t.get_presenter(), t.get_duration(), t.get_likes(), t.get_link()};
	this->repo->remove_tutorial(title, presenter);
	Action* currentAction = new DeleteAction(this->repo, deleted);
	this->undoStackAdminMode.push_back(currentAction);
	this->redoStackAdminMode.clear();
}

void Service::update_tutorial_service(std::string old_title, std::string old_presenter, Tutorial new_t)
{
	Tutorial t = this->get_tutorial_service(old_title, old_presenter);
	Tutorial new_tutorial{ new_t.get_title(), new_t.get_presenter(), new_t.get_duration(), new_t.get_likes(), new_t.get_link() };
	Tutorial old_tutorial{ t.get_title(), t.get_presenter(), t.get_duration(), t.get_likes(), t.get_link() };
	this->repo->update_tutorial(old_title, old_presenter, new_t);
	Action* currentAction = new UpdateAction(this->repo, old_tutorial, new_tutorial);
	this->undoStackAdminMode.push_back(currentAction);
	this->redoStackAdminMode.clear();
}

int Service::get_size_service()
{
	return this->repo->get_repo_size();
}

Tutorial Service::get_tutorial_service(std::string title, std::string presenter)
{
	return this->repo->search_tutorial(title, presenter);
}

std::vector<Tutorial> Service::get_all_tutorials_service()
{
	return this->repo->get_all_tutorials();
}

std::vector<Tutorial> Service::get_watch_list_service()
{
	return this->watch_list->get_whole_watch_list();
}

int Service::get_watch_list_size()
{
	return this->watch_list->get_watch_list_size();
}

void Service::add_tutorial_to_watch_list(Tutorial t)
{
	this->watch_list->add_to_watch_list(t);
}

void Service::watch_tutorial_service(std::string title, std::string presenter)
{
	this->watch_list->remove_from_watch_list(title, presenter);
}

void Service::like_tutorial(std::string title, std::string presenter)
{
	Tutorial t = this->repo->search_tutorial(title, presenter);
	t.set_likes(t.get_likes() + 1);
	this->update_tutorial_service(title, presenter, t);
}

std::vector<Tutorial> Service::search_by_presenter_service(std::string presenter)
{
	return this->repo->get_tutorial_by_presenter(presenter);
}

std::vector<std::string> Service::get_all_presenters_service()
{
	std::vector<std::string> presenters;
	std::vector<std::string>::iterator it;
	bool found = false;
	for (auto t : this->get_all_tutorials_service()) {
		found = false;
		for (auto p : presenters)
			if (p == t.get_presenter())
				found = true;
		if (found == false)
			presenters.push_back(t.get_presenter());
	}
	return presenters;
}

int Service::numberOfTutorialsPerPresenter(const std::string& presenter)
{
	int count = 0;
	for (auto t : this->get_all_tutorials_service())
		if (t.get_presenter() == presenter)
			count++;
	return count;
}

void Service::open()
{
	this->watch_list->open();
}
