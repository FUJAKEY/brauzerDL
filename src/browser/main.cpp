#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

void loadStyle(QApplication& app) {
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        app.setStyleSheet(stream.readAll());
        file.close();
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CandyBrowser");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("CandyCorp");

    loadStyle(app);

    MainWindow window;
    window.resize(1280, 800);
    window.show();

    return app.exec();
}
