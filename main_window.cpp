#include <main_window.hpp>

#include <central_widget.hpp>

MainWindow::MainWindow()
    :QMainWindow(), m_widget(new CentralWidget(this))
{
    setWindowTitle("Trainer");
    setCentralWidget(m_widget);
}

