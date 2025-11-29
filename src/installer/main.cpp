#include <QApplication>
#include "installwizard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CandyBrowserInstaller");

    // Apply some style
    app.setStyleSheet(
        "QWidget { background-color: #1e1e2e; color: #cdd6f4; font-family: 'Segoe UI'; font-size: 14px; }"
        "QPushButton { background-color: #cba6f7; color: #1e1e2e; border-radius: 5px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #b4befe; }"
        "QLineEdit { background: #313244; border: 1px solid #45475a; border-radius: 5px; padding: 5px; color: white; }"
    );

    InstallWizard wizard;
    wizard.show();

    return app.exec();
}
