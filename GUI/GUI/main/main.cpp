#include <QtWidgets/QApplication>
#include "GUI.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    ChoiceDialog choiceDialog;
    if (choiceDialog.exec() == QDialog::Accepted) {
        QString choice = choiceDialog.getChoice();

        std::string filename_admin = "tutorials.txt";
        Repo repo(filename_admin);
        WatchList* watch_list;

        if (choice == "CSV") {
            watch_list = new CSVWatchList("CSVWatchList.csv");
        }
        else {
            watch_list = new HTMLWatchList("HTMLWatchList.html");
        }

        Service service(&repo, watch_list);
        Validator validator;
        mainGUI maingui{ service, validator, watch_list };
        maingui.show();
        return a.exec();
    }
    else {
        return 0;
    }
}
