#include "C:\Lab UBB\Sem2\OOP\oop-a10-916-sandor-denisa\GUI\GUI\GUI.h"
#include <qlayout.h>
#include <qlabel.h>
#include <sstream>
#include <string>

TutorialTableModel::TutorialTableModel(WatchList* watch_list) {
	this->watch_list = watch_list;
}

int TutorialTableModel::rowCount(const QModelIndex& parent) const {
	return this->watch_list->get_watch_list_size();
}

int TutorialTableModel::columnCount(const QModelIndex& parent) const {
	return 5;
}

QVariant TutorialTableModel::data(const QModelIndex& index, int role) const {
	int row = index.row();
	Tutorial currentTutorial = this->watch_list->get_whole_watch_list()[row];
	int column = index.column();
	if (role == Qt::DisplayRole || role == Qt::EditRole) {
		switch (column) {
		case 0:
			return QString::fromStdString(currentTutorial.get_title());
		case 1:
			return QString::fromStdString(currentTutorial.get_presenter());
		case 2:
			return QString::fromStdString(std::to_string(currentTutorial.get_duration().first) + ":" + std::to_string(currentTutorial.get_duration().second));
		case 3:
			return QString::fromStdString(std::to_string(currentTutorial.get_likes()));
		case 4:
			return QString::fromStdString(currentTutorial.get_link());
		default:
			break;
		}
	}
	return QVariant();
}

QVariant TutorialTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
			case 0:
				return QString("Title");
			case 1:
				return QString("Presenter");
			case 2:
				return QString("Duration");
			case 3:
				return QString("Likes");
			case 4:
				return QString("Link");
			default:
				break;
			}
		}
	}
	return QVariant();
}

void TutorialTableModel::update() {
	QModelIndex topLeft = this->index(1, 1);
	QModelIndex bottomRight = this->index(this->rowCount(), this->columnCount());
	emit layoutChanged();
	emit dataChanged(topLeft, bottomRight);
}


mainGUI::mainGUI(Service& service, Validator& validator, WatchList* watchList, QWidget* parent) : service{ service }, validator{ validator }, watchList{watchList}, QMainWindow{parent}
{
	this->initGUI();
	this->mainConnect();
}

void mainGUI::initGUI()
{
	this->adminModeButton = new QPushButton{ "Admin Mode" };
	this->userModeButton = new QPushButton{ "User Mode" };
	this->openChart = new QPushButton{ "Open chart" };

	QVBoxLayout* mainLayout = new QVBoxLayout();

	QLabel* intro = new QLabel();
	intro->setText("Welcome to C++ Master");
	intro->setAlignment(Qt::AlignCenter);

	QLabel* space = new QLabel();
	space->setText("");

	mainLayout->addWidget(intro);
	mainLayout->addWidget(space);

	QGridLayout* buttonsLayout = new QGridLayout();
	buttonsLayout->addWidget(this->saveButton, 0, 0);
	buttonsLayout->addWidget(this->userModeButton, 1, 0);
	buttonsLayout->addWidget(this->adminModeButton, 2, 0);
	buttonsLayout->addWidget(this->openChart, 3, 0);

	mainLayout->addLayout(buttonsLayout);

	QWidget* mainWindow = new QWidget();
	mainWindow->setMinimumWidth(400);
	mainWindow->setLayout(mainLayout);
	setCentralWidget(mainWindow);
}

void mainGUI::startAdminMode()
{
	this->setCursor(Qt::WaitCursor);
	AdminGUI* adminGUI = new AdminGUI{ service, validator };
	adminGUI->show();
	this->setCursor(Qt::ArrowCursor);
}

void mainGUI::startUserMode()
{
	this->setCursor(Qt::WaitCursor);
	UserGUI* userGUI = new UserGUI{ service, validator, this->watchList };
	userGUI->show();
	this->setCursor(Qt::ArrowCursor);
}

void mainGUI::mainConnect()
{
	QObject::connect(this->adminModeButton, &QPushButton::clicked, this, &mainGUI::startAdminMode);
	QObject::connect(this->userModeButton, &QPushButton::clicked, this, &mainGUI::startUserMode);
	QObject::connect(this->openChart, &QPushButton::clicked, this, &mainGUI::openChartButtonHandler);
}

void mainGUI::openChartButtonHandler()
{
	this->chartWindow = new QWidget{};
	QBarSeries* series = new QBarSeries;

	QList<QColor> colors = {
		QColor("#fd7f6f"),
		QColor("#7eb0d5"),
		QColor("#b2e061"),
		QColor("#bd7ebe"),
		QColor("#ffb55a"),
		QColor("#ffee65"),
		QColor("#beb9db"),
		QColor("#fdcce5"),
		QColor("#8bd3c7"),
		QColor("#1B1A55"),
		QColor("#8f568b")
	};
	int colorIndex = 0;

	for (auto p : this->service.get_all_presenters_service()) {
		auto set = new QBarSet(QString::fromStdString(p));
		set->setColor(colors[colorIndex++]);
		if (colorIndex == colors.size())
			colorIndex = 0;
		*set << this->service.numberOfTutorialsPerPresenter(p);
		series->append(set);
	}
	series->setBarWidth(0.7);

	auto* chart = new QChart;
	chart->addSeries(series);
	chart->setAnimationOptions(QChart::SeriesAnimations);

	auto axisX = new QBarCategoryAxis;
	axisX->setTitleText("Presenters");
	chart->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);

	auto axisY = new QValueAxis;
	axisY->setRange(0, 4);
	axisY->setTitleText("Number of tutorials");
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	auto* chartLayout = new QVBoxLayout(this->chartWindow);

	chart->setTitle("Number of tutorials per presenter");

	auto* chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	chartLayout->addWidget(chartView);
	this->chartWindow->resize(1150, 720);
	this->chartWindow->show();
}


AdminGUI::AdminGUI(Service& service, Validator& validator) : service{ service }, validator{ validator }
{
	this->buildAdminGUI();
	this->adminConnect();
	this->populateList();
}

void AdminGUI::buildAdminGUI()
{
	QHBoxLayout* mainLayout = new QHBoxLayout{ this };
	this->tutorialListWidget = new QListWidget{};
	this->tutorialListWidget->setMinimumWidth(500);
	mainLayout->addWidget(tutorialListWidget);

	QGridLayout* rightLayout = new QGridLayout{};

	QLabel* titleLabel = new QLabel{ "Title" };
	this->titleEdit = new QLineEdit{};
	this->titleEdit->setMinimumSize(300, 20);
	QLabel* presenterLabel = new QLabel{ "Presenter" };
	this->presenterEdit = new QLineEdit{};
	this->presenterEdit->setMinimumSize(300, 20);
	QLabel* durationLabel = new QLabel{ "Duration (min:sec)" };
	this->durationEdit = new QLineEdit{};
	this->durationEdit->setMinimumSize(300, 20);
	QLabel* likesLabel = new QLabel{ "Likes" };
	this->likesEdit = new QLineEdit{};
	this->likesEdit->setMinimumSize(300, 20);
	QLabel* linkLabel = new QLabel{ "Link" };
	this->linkEdit = new QLineEdit{};
	this->linkEdit->setMinimumSize(300, 20);

	rightLayout->addWidget(titleLabel, 0, 0);
	rightLayout->addWidget(this->titleEdit, 0, 1);
	rightLayout->addWidget(presenterLabel, 1, 0);
	rightLayout->addWidget(this->presenterEdit, 1, 1);
	rightLayout->addWidget(durationLabel, 2, 0);
	rightLayout->addWidget(this->durationEdit, 2, 1);
	rightLayout->addWidget(likesLabel, 3, 0);
	rightLayout->addWidget(this->likesEdit, 3, 1);
	rightLayout->addWidget(linkLabel, 4, 0);
	rightLayout->addWidget(this->linkEdit, 4, 1);

	this->addButton = new QPushButton{ "Add" };
	this->deleteButton = new QPushButton{ "Delete" };
	this->updateButton = new QPushButton{ "Update" };
	this->undoButton = new QPushButton{ "Undo" };
	this->redoButton = new QPushButton{ "Redo" };

	rightLayout->addWidget(this->addButton, 5, 0);
	rightLayout->addWidget(this->deleteButton, 5, 1);
	rightLayout->addWidget(this->updateButton, 5, 2);
	rightLayout->addWidget(this->undoButton, 6, 0);
	rightLayout->addWidget(this->redoButton, 6, 1);

	rightLayout->setColumnStretch(0, 10);
	rightLayout->setColumnStretch(1, 10);

	this->undoShortcut = new QShortcut(QKeySequence("Ctrl+z"), this);

	this->redoShortcut = new QShortcut(QKeySequence("Ctrl+y"), this);

	mainLayout->addLayout(rightLayout);
}

void AdminGUI::populateList()
{
	this->tutorialListWidget->clear();

	for (auto s : this->service.get_all_tutorials_service())
	{
		QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.get_title() + "  -  " + s.get_presenter()) };
		this->tutorialListWidget->addItem(item);
	}
}

void AdminGUI::addButtonHandler()
{
	try {
		std::string title = this->titleEdit->text().toStdString();
		std::string presenter = this->presenterEdit->text().toStdString();
		QString duration = this->durationEdit->text();
		QString likes = this->likesEdit->text();
		QString link = this->linkEdit->text();

		std::string duration_string = duration.toStdString();
		std::istringstream ss(duration_string);
		std::string minutes, seconds;

		std::getline(ss, minutes, ':');
		std::getline(ss, seconds, ':');

		this->validator.validate_duration(minutes, seconds);
		this->validator.validate_tutorial_presenter(title);
		this->validator.validate_tutorial_presenter(presenter);
		this->validator.validate_likes(likes.toStdString());
		this->validator.validate_link(link.toStdString());
		Duration d{ std::stoi(minutes), std::stoi(seconds) };
		this->service.add_tutorial_admin(title, presenter, d, likes.toInt(), link.toStdString());
		this->populateList();
		this->titleEdit->clear();
		this->presenterEdit->clear();
		this->durationEdit->clear();
		this->likesEdit->clear();
		this->linkEdit->clear();
	}
	catch (TutorialException& exc) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(exc.what());
		error->setWindowTitle("Invalid input!");
		error->exec();
	}
	catch (RepoException& re) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(re.what());
		error->setWindowTitle("Error while adding the tutorial!");
		error->exec();
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "Couldn't add any tutorial!");
		return;
	}
}

void AdminGUI::deleteButtonHandler()
{
	int index = this->getSelectedIndex();
	if (index < 0) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText("No tutorial was selected!");
		error->exec();
	}

	this->titleEdit->setText(QString::fromStdString(this->service.get_all_tutorials_service()[index].get_title()));
	this->presenterEdit->setText(QString::fromStdString(this->service.get_all_tutorials_service()[index].get_presenter()));
	int minutes = this->service.get_all_tutorials_service()[index].get_duration().first;
	int seconds = this->service.get_all_tutorials_service()[index].get_duration().second;
	this->durationEdit->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
	this->likesEdit->setText(QString::number(this->service.get_all_tutorials_service()[index].get_likes()));
	this->linkEdit->setText(QString::fromStdString(this->service.get_all_tutorials_service()[index].get_link()));

	try {
		this->service.remove_tutorial_service(this->service.get_all_tutorials_service()[index].get_title(), this->service.get_all_tutorials_service()[index].get_presenter());
		this->populateList();
		this->titleEdit->clear();
		this->presenterEdit->clear();
		this->durationEdit->clear();
		this->likesEdit->clear();
		this->linkEdit->clear();
	}
	catch (TutorialException& exc) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(exc.what());
		error->setWindowTitle("Invalid input!");
		error->exec();
	}
	catch (RepoException& re) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(re.what());
		error->setWindowTitle("Error while removing the tutorial!");
		error->exec();
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "Couldn't remove any tutorial!");
		return;
	}
}

void AdminGUI::updateButtonHandler()
{
	try {
		int index = this->getSelectedIndex();
		if (index < 0) {
			auto* error = new QMessageBox();
			error->setIcon(QMessageBox::Critical);
			error->setText("No tutorial was selected!");
			error->exec();
		}
		else {
			std::string old_title = this->service.get_all_tutorials_service()[index].get_title();
			std::string old_presenter = this->service.get_all_tutorials_service()[index].get_presenter();
			std::string new_title = this->titleEdit->text().toStdString();
			std::string new_presenter = this->presenterEdit->text().toStdString();
			std::string new_duration = this->durationEdit->text().toStdString();
			std::string new_likes = this->likesEdit->text().toStdString();
			std::string new_link = this->linkEdit->text().toStdString();

			std::istringstream ss(new_duration);
			std::string minutes, seconds;

			std::getline(ss, minutes, ':');
			std::getline(ss, seconds, ':');

			this->validator.validate_duration(minutes, seconds);
			this->validator.validate_tutorial_presenter(new_title);
			this->validator.validate_tutorial_presenter(new_presenter);
			this->validator.validate_likes(new_likes);
			this->validator.validate_link(new_link);

			Duration d{ std::stoi(minutes), std::stoi(seconds) };

			Tutorial new_t{ new_title, new_presenter, d, std::stoi(new_likes), new_link };
			this->service.update_tutorial_service(old_title, old_presenter, new_t);
			this->populateList();
			this->titleEdit->clear();
			this->presenterEdit->clear();
			this->durationEdit->clear();
			this->likesEdit->clear();
			this->linkEdit->clear();
		}
	}
	catch (TutorialException& exc) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(exc.what());
		error->setWindowTitle("Invalid input!");
		error->exec();
	}
	catch (RepoException& re) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(re.what());
		error->setWindowTitle("Error while updating the tutorial!");
		error->exec();
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "Couldn't update any tutorial!");
		return;
	}
}

int AdminGUI::getSelectedIndex() const
{
	QModelIndexList selectedIndexes = this->tutorialListWidget->selectionModel()->selectedIndexes();
	if (selectedIndexes.empty()) {
		this->titleEdit->clear();
		this->presenterEdit->clear();
		this->durationEdit->clear();
		this->likesEdit->clear();
		this->linkEdit->clear();
		return -1;
	}
	int selectedIndex = selectedIndexes.at(0).row();
	return selectedIndex;
}

void AdminGUI::adminConnect()
{
	QObject::connect(this->tutorialListWidget, &QListWidget::itemSelectionChanged, [this]() {
		int selectedIndex = this->getSelectedIndex();
		if (selectedIndex < 0)
			return;
		Tutorial t = this->service.get_all_tutorials_service()[selectedIndex];
		this->titleEdit->setText(QString::fromStdString(t.get_title()));
		this->presenterEdit->setText(QString::fromStdString(t.get_presenter()));
		this->durationEdit->setText(QString::fromStdString(std::to_string(t.get_duration().first) + ":" + std::to_string(t.get_duration().second)));
		this->likesEdit->setText(QString::fromStdString(std::to_string(t.get_likes())));
		this->linkEdit->setText(QString::fromStdString(t.get_link()));
		});

	QObject::connect(this->addButton, &QPushButton::clicked, this, &AdminGUI::addButtonHandler);
	QObject::connect(this->deleteButton, &QPushButton::clicked, this, &AdminGUI::deleteButtonHandler);
	QObject::connect(this->updateButton, &QPushButton::clicked, this, &AdminGUI::updateButtonHandler);
	QObject::connect(this->undoButton, &QPushButton::clicked, this, &AdminGUI::undo);
	QObject::connect(this->redoButton, &QPushButton::clicked, this, &AdminGUI::redo);
	QObject::connect(this->undoShortcut, &QShortcut::activated, this, &AdminGUI::undo);
	QObject::connect(this->redoShortcut, &QShortcut::activated, this, &AdminGUI::redo);
}

void AdminGUI::undo()
{
	try {
		this->service.undoAdminMode();
		this->populateList();
	}
	catch (...)
	{
		QMessageBox::critical(this, "Error", "There are no more undos");
		return;
	}
}

void AdminGUI::redo()
{
	try {
		this->service.redoAdminMode();
		this->populateList();
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "There are no more redos!");
		return;
	}
}

UserGUI::UserGUI(Service& service, Validator& validator, WatchList* watchList) : service{ service }, validator{ validator }, watchList{ watchList }
{
	this->position = 0;
	this->continueIteration = true;
	this->buildUserGUI();
	this->userConnect();
}

void UserGUI::buildUserGUI()
{
	this->tutorialListWidget = new QListWidget{};
	this->tutorialListWidget->setMinimumWidth(800);

	this->displayAllButton = new QPushButton{ "Show all tutorials" };
	this->displayWatchListButton = new QPushButton{ "Display the watch list" };
	this->filterButton = new QPushButton{ "Filter by presenter" };
	this->watchfilteredButton = new QPushButton{ "Watch the filtered tutorials" };
	this->openButton = new QPushButton{ "Open in app" };
	this->tableMode = new QPushButton{ "Open table view mode" };

	QLabel* filter = new QLabel{ "Search" };
	this->filterEdit = new QLineEdit{};

	QLabel* tutorialText = new QLabel{};
	tutorialText->setText("Watch List");
	tutorialText->setAlignment(Qt::AlignCenter);

	QGridLayout* buttonsLayout = new QGridLayout();

	buttonsLayout->addWidget(this->displayAllButton, 0, 0);
	buttonsLayout->addWidget(this->displayWatchListButton, 1, 0);
	buttonsLayout->addWidget(this->filterButton, 2, 0);
	buttonsLayout->addWidget(this->watchfilteredButton, 3, 0);
	buttonsLayout->addWidget(this->openButton, 4, 0);
	buttonsLayout->addWidget(this->tableMode, 5, 0);

	QVBoxLayout* userLayout = new QVBoxLayout(this);
	userLayout->addWidget(tutorialText);
	userLayout->addWidget(this->tutorialListWidget);
	userLayout->addLayout(buttonsLayout);

	QFormLayout* tutorialDetails = new QFormLayout();
	tutorialDetails->addRow(filter, filterEdit);
	userLayout->addLayout(tutorialDetails);
}

void UserGUI::displayAllButtonHandler()
{
	this->tutorialListWidget->clear();

	for (auto s : this->service.get_all_tutorials_service())
	{
		QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.get_title() + " , " + s.get_presenter() + " , " + std::to_string(s.get_duration().first) + ":" + std::to_string(s.get_duration().second) +
			+" , " + std::to_string(s.get_likes()) + " , " + s.get_link()) };
		this->tutorialListWidget->addItem(item);
	}
}

void UserGUI::displayWatchListButtonHandler()
{
	if (this->service.get_watch_list_service().size() == 0) {
		QMessageBox::critical(this, "Error", "The watch list is currently empty!");
		return;
	}

	this->tutorialListWidget->clear();
	for (auto s : this->service.get_watch_list_service())
	{
		QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.get_title() + " , " + s.get_presenter() + " , " + std::to_string(s.get_duration().first) + ":" + std::to_string(s.get_duration().second) +
			+" , " + std::to_string(s.get_likes()) + " , " + s.get_link()) };
		this->tutorialListWidget->addItem(item);
	}
}

void UserGUI::openButtonHandler()
{
	if (this->service.get_watch_list_size() == 0) {
		QMessageBox::critical(this, "Error", "The watch list is currently empty!");
		return;
	}
	this->service.open();
}

void UserGUI::filterButtonHandler()
{
	try {
		this->position = 0;
		std::string presenter = this->filterEdit->text().toStdString();
		if (presenter.empty()) {
			this->filtered = this->service.get_all_tutorials_service();
			this->tutorialListWidget->clear();
			for (auto s : this->service.get_all_tutorials_service())
			{
				QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.get_title() + " , " + s.get_presenter() + " , " + std::to_string(s.get_duration().first) + ":" + std::to_string(s.get_duration().second) +
					+" , " + std::to_string(s.get_likes()) + " , " + s.get_link()) };
				this->tutorialListWidget->addItem(item);
			}
		}
		else {
			this->validator.validate_tutorial_presenter(presenter);
			this->tutorialListWidget->clear();
			for (auto s : this->service.search_by_presenter_service(presenter))
			{
				this->filtered.push_back(s);
				QListWidgetItem* item = new QListWidgetItem{ QString::fromStdString(s.get_title() + " , " + s.get_presenter() + " , " + std::to_string(s.get_duration().first) + ":" + std::to_string(s.get_duration().second) +
					+" , " + std::to_string(s.get_likes()) + " , " + s.get_link()) };
				this->tutorialListWidget->addItem(item);
			}
		}
	}
	catch (TutorialException& exc) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(exc.what());
		error->setWindowTitle("Invalid input!");
		error->exec();
	}
	catch (RepoException& re) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(re.what());
		error->setWindowTitle("Error while searching!");
		error->exec();
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "Couldn't find any presenter!");
		return;
	}
}

void UserGUI::watchfilteredButtonHandler()
{
	this->continueIteration = true;
	for (auto& t : this->filtered) {
		if (!continueIteration)
			break;
		else
			this->watchTutorial();
	}
}

void UserGUI::addToWatchListButtonHandler()
{
	try {
		Tutorial t = this->service.get_tutorial_service(this->filtered[this->position].get_title(), this->filtered[this->position].get_presenter());
		this->service.add_tutorial_to_watch_list(t);
		QMessageBox::information(nullptr, "Success", "The tutorial was added to the watch list!");
	}
	catch (RepoException& re) {
		auto* error = new QMessageBox();
		error->setIcon(QMessageBox::Critical);
		error->setText(re.what());
		error->setWindowTitle("Error while adding!");
		error->exec();
	}
	catch (...) {
		QMessageBox::critical(this, "Error", "Couldn't add the tutorial to the watch list!");
		return;
	}
}

void UserGUI::likeButtonHandler()
{
	this->service.like_tutorial(this->filtered[this->position].get_title(), this->filtered[this->position].get_presenter());
	QMessageBox::information(nullptr, "Success", "Your like was added successfully!");
}

void UserGUI::userConnect()
{
	QObject::connect(this->displayAllButton, &QPushButton::clicked, this, &UserGUI::displayAllButtonHandler);
	QObject::connect(this->displayWatchListButton, &QPushButton::clicked, this, &UserGUI::displayWatchListButtonHandler);
	QObject::connect(this->filterButton, &QPushButton::clicked, this, &UserGUI::filterButtonHandler);
	QObject::connect(this->watchfilteredButton, &QPushButton::clicked, this, &UserGUI::watchfilteredButtonHandler);
	QObject::connect(this->openButton, &QPushButton::clicked, this, &UserGUI::openButtonHandler);
	QObject::connect(this->tableMode, &QPushButton::clicked, this, &UserGUI::createTable);
}

void UserGUI::watchTutorial()
{
	QDialog dialog(this);
	dialog.resize(300, 200);
	this->continueIteration = true;
	QVBoxLayout* mainLayout = new QVBoxLayout(&dialog);

	QGridLayout* upperLayout = new QGridLayout{};

	QLabel* titleLabel = new QLabel{};
	titleLabel->setText("Title: " + QString::fromStdString(this->filtered[this->position].get_title()));
	QLabel* presenterLabel = new QLabel{ };
	presenterLabel->setText("Presenter: " + QString::fromStdString(this->filtered[this->position].get_presenter()));
	QLabel* durationLabel = new QLabel{ };
	durationLabel->setText("Duration: " + QString::fromStdString(std::to_string(this->filtered[this->position].get_duration().first) + ":" + std::to_string(this->filtered[this->position].get_duration().second)));
	QLabel* likesLabel = new QLabel{ };
	likesLabel->setText("Likes: " + QString::number(this->filtered[this->position].get_likes()));
	QLabel* linkLabel = new QLabel{ };
	linkLabel->setText("Link: " + QString::fromStdString(this->filtered[this->position].get_link()));

	upperLayout->addWidget(titleLabel, 0, 0);
	upperLayout->addWidget(presenterLabel, 1, 0);
	upperLayout->addWidget(durationLabel, 2, 0);
	upperLayout->addWidget(likesLabel, 3, 0);
	upperLayout->addWidget(linkLabel, 4, 0);

	QLabel* intro = new QLabel();
	intro->setText("\nCurrent tutorial:\n");
	intro->setAlignment(Qt::AlignCenter);

	QPushButton* likeButton = new QPushButton("Like", &dialog);
	QPushButton* nextButton = new QPushButton("Next", &dialog);
	QPushButton* addToWatchListButton = new QPushButton("Add to watch list", &dialog);
	QPushButton* exitWatchMode = new QPushButton("Quit watching", &dialog);

	connect(likeButton, &QPushButton::clicked, this, &UserGUI::likeButtonHandler);
	connect(nextButton, &QPushButton::clicked, this, [&]() {
		if (this->position == this->filtered.size() - 1)
			this->position = 0;
		else
			this->position++;
		dialog.close();
		this->watchTutorial();
		});
	connect(addToWatchListButton, &QPushButton::clicked, this, &UserGUI::addToWatchListButtonHandler);
	connect(exitWatchMode, &QPushButton::clicked, [&]() {
		this->continueIteration = false;
		dialog.close();
		});

	mainLayout->addWidget(intro);

	mainLayout->addLayout(upperLayout);

	mainLayout->addWidget(likeButton);
	mainLayout->addWidget(nextButton);
	mainLayout->addWidget(addToWatchListButton);
	mainLayout->addWidget(exitWatchMode);

	ShellExecuteA(NULL, NULL, "chrome.exe", this->filtered[this->position].get_link().c_str(), NULL, SW_SHOWMINIMIZED);

	dialog.exec();
}

void UserGUI::createTable()
{
	this->tutorialTableModel = new TutorialTableModel{ this->watchList};
	this->tutorialTable = new QTableView{};
	this->tutorialTable->setModel(this->tutorialTableModel);
	this->tutorialTable->resize(400, 400);
	this->tutorialTable->resizeColumnsToContents();
	this->tutorialTable->resizeRowsToContents();
	this->tutorialTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	this->tutorialTable->show();
}

ChoiceDialog::ChoiceDialog(QWidget* parent)
	: QDialog(parent), choice("")
{
	QLabel* intro = new QLabel();
	intro->setText("\nWelcome to C++ Master\n");
	intro->setAlignment(Qt::AlignCenter);

	QLabel* space = new QLabel();
	space->setText("");

	csvButton = new QPushButton("Save as CSV", this);
	htmlButton = new QPushButton("Save as HTML", this);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(intro);
	layout->addWidget(space);
	layout->addWidget(csvButton);
	layout->addWidget(htmlButton);

	connect(csvButton, &QPushButton::clicked, this, &ChoiceDialog::chooseCsv);
	connect(htmlButton, &QPushButton::clicked, this, &ChoiceDialog::chooseHtml);
}

void ChoiceDialog::chooseCsv()
{
	choice = "CSV";
	accept();
}

void ChoiceDialog::chooseHtml()
{
	choice = "HTML";
	accept();
}

QString ChoiceDialog::getChoice() const
{
	return choice;
}