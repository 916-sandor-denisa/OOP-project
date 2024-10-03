#include "Action.h"

AddAction::AddAction(Repo* repo, Tutorial addedTutorial) : repo{repo}
{
	this->addedTutorial = addedTutorial;
}

void AddAction::executeUndo()
{
	this->repo->remove_tutorial(this->addedTutorial.get_title(), this->addedTutorial.get_presenter());
}

void AddAction::executeRedo()
{
	this->repo->add_tutorial(this->addedTutorial);
}

DeleteAction::DeleteAction(Repo* repo, Tutorial deletedTutorial) : repo{repo}
{
	this->deletedTutorial = deletedTutorial;
}

void DeleteAction::executeUndo()
{
	this->repo->add_tutorial(this->deletedTutorial);
}

void DeleteAction::executeRedo()
{
	this->repo->remove_tutorial(this->deletedTutorial.get_title(), this->deletedTutorial.get_presenter());
}

UpdateAction::UpdateAction(Repo* repo, Tutorial oldTutorial, Tutorial newTutorial) : repo{repo}
{
	this->oldTutorial = oldTutorial;
	this->newTutorial = newTutorial;
}

void UpdateAction::executeUndo()
{
	this->repo->update_tutorial(this->newTutorial.get_title(), this->newTutorial.get_presenter(), this->oldTutorial);
}

void UpdateAction::executeRedo()
{
	this->repo->update_tutorial(this->oldTutorial.get_title(), this->oldTutorial.get_presenter(), this->newTutorial);
}
