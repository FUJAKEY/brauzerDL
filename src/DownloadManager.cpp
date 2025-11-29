#include "DownloadManager.h"
#include <QFileDialog>
#include <QStandardPaths>

DownloadManager::DownloadManager(QObject *parent) : QObject(parent) {}

void DownloadManager::handleDownloadRequested(QWebEngineDownloadRequest *download) {
    // Determine path
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + download->downloadDirectory();

    // For simplicity in this "Sweet" browser, we just accept the download to default downloads folder
    // But ideally we ask the user.
    // Let's autosave to Downloads folder for better UX (less clicking).

    QString path = download->downloadDirectory();
    QString filename = download->downloadFileName();

    // Actually, let's use the system default downloads location
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fullPath = downloadPath + "/" + filename;

    download->setDownloadDirectory(downloadPath);
    download->setDownloadFileName(filename);
    download->accept();
}
