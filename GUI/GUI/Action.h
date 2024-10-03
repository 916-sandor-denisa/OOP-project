#pragma once
#include "repo.h"
#include "watch_list.h"

class Action {
public:
    virtual void executeUndo() = 0;
    virtual void executeRedo() = 0;
};

class AddAction : public Action {

private:
    Repo* repo;
    Tutorial addedTutorial;

public:
    AddAction(Repo* repo, Tutorial addedTutorial);
    void executeUndo() override;
    void executeRedo() override;

};

class DeleteAction : public Action {

private:
    Repo* repo;
    Tutorial deletedTutorial;

public:
    DeleteAction(Repo* Repo, Tutorial deletedTutorial);
    void executeUndo() override;
    void executeRedo() override;

};

class UpdateAction : public Action {

private:
    Repo* repo;
    Tutorial oldTutorial;
    Tutorial newTutorial;

public:
    UpdateAction(Repo* Repo, Tutorial oldTutorial, Tutorial newTutorial);
    void executeUndo() override;
    void executeRedo() override;

};