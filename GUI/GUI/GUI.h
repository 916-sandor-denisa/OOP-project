#pragma once

#include "service.h"
#include "validator.h"
#include <algorithm>
#include <cmath>
#include <windows.h>
#include <qwidget.h>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qbarcategoryaxis.h>
#include <QMainWindow>
#include <QComboBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QDialog>
#include <QTimer>
#include <QVBoxLayout>
#include <QStringList>
#include <QEventLoop>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QPushButton>
#include <QCoreApplication>
#include <QtCore>
#include <QtCharts>
#include <QPainter>
#include <qchart.h>
#include <qstring.h>
#include <QString>
#include <QStringList>
#include <QShortcut>
#include <QKeySequence>

class TutorialTableModel : public QAbstractTableModel {
private:
	WatchList* watch_list;
public:
	explicit TutorialTableModel(WatchList* watch_list);

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	void update();
};

class mainGUI : public QMainWindow {
	Q_OBJECT
private:
	Service& service;
	Validator& validator;
	WatchList* watchList;

	QPushButton* adminModeButton;
	QPushButton* userModeButton;
	QPushButton* saveButton;
	QPushButton* openChart;

	QWidget* chartWindow;

	void initGUI();
public:
	mainGUI(Service& service, Validator& validator, WatchList* watchList , QWidget* parent = 0);

	void startAdminMode();
	void startUserMode();
	void mainConnect();

	void openChartButtonHandler();
};


class AdminGUI : public QDialog
{
private:
	Service& service;
	Validator& validator;

	QListWidget* tutorialListWidget;
	QLineEdit* titleEdit;
	QLineEdit* presenterEdit;
	QLineEdit* durationEdit;
	QLineEdit* likesEdit;
	QLineEdit* linkEdit;

	QPushButton* addButton;
	QPushButton* deleteButton;
	QPushButton* updateButton;
	QPushButton* undoButton;
	QPushButton* redoButton;

	QShortcut* undoShortcut;
	QShortcut* redoShortcut;

public:
	AdminGUI(Service& service, Validator& validator);
	void buildAdminGUI();
	void populateList();

	void addButtonHandler();
	void deleteButtonHandler();
	void updateButtonHandler();
	int getSelectedIndex() const;
	void adminConnect();

	void undo();
	void redo();
};


class UserGUI : public QDialog {
private:
	Service& service;
	Validator& validator;
	WatchList* watchList;

	TutorialTableModel* tutorialTableModel;
	QTableView* tutorialTable;

	int position;
	bool continueIteration;
	std::vector<Tutorial> filtered;

	QListWidget* tutorialListWidget;
	QListWidget* watchListWidget;
	QLineEdit* filterEdit;

	QGridLayout* listAndTableLayout;

	QPushButton* displayAllButton;
	QPushButton* displayWatchListButton;
	QPushButton* filterButton;
	QPushButton* watchfilteredButton;
	QPushButton* openButton;
	QPushButton* likeButton;
	QPushButton* nextButton;
	QPushButton* addToWatchList;
	QPushButton* exitWatchMode;
	QPushButton* tableMode;

public:
	UserGUI(Service& service, Validator& validator, WatchList* watchList);
	void buildUserGUI();
	void displayAllButtonHandler();
	void displayWatchListButtonHandler();
	void openButtonHandler();
	void filterButtonHandler();
	void watchfilteredButtonHandler();
	void addToWatchListButtonHandler();
	void likeButtonHandler();
	void userConnect();
	void watchTutorial();
	void createTable();
};

class ChoiceDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChoiceDialog(QWidget* parent = nullptr);
	QString getChoice() const;

private slots:
	void chooseCsv();
	void chooseHtml();

private:
	QPushButton* csvButton;
	QPushButton* htmlButton;
	QString choice;
};