#ifndef INCLUDE_GUARD_MAIN_WINDOW_HPP
#define INCLUDE_GUARD_MAIN_WINDOW_HPP

#include <QMainWindow>

class CentralWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT;
private:
    CentralWidget* m_widget;
public:
    MainWindow();
};

#endif
