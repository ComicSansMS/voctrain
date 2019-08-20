#include <main_window.hpp>

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication the_app(argc, argv);

    MainWindow main_window;
    main_window.show();

    return the_app.exec();
}

