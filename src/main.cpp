#include <QApplication>
#include "BrowserWindow.h"
#include <QFile>
#include <QTextStream>

void loadStyleSheet(QApplication &app) {
    // Load Liquid Glass Theme
    QFile file(":/liquid.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        app.setStyleSheet(stream.readAll());
        file.close();
    }
}

int main(int argc, char *argv[]) {
    // High DPI scaling is standard in Qt6, but good to ensure
    QApplication app(argc, argv);
    app.setApplicationName("SweetBrowser");
    app.setApplicationDisplayName("Sweet Browser");
    app.setOrganizationName("SweetSoft");
    app.setOrganizationDomain("sweetsoft.com");

    loadStyleSheet(app);

    BrowserWindow window;
    window.resize(1280, 720);
    window.show();

    return app.exec();
}
