#include "downloadmanager.h"
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QFileDialog>

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
}

void DownloadManager::handleDownloadRequested(QWebEngineDownloadRequest *download)
{
    // Auto-accept for now or ask user.
    // Let's ask path or save to downloads
    download->accept();
    m_downloads.append(download);
}

void DownloadManager::showDownloadsDialog()
{
    QDialog *dialog = new QDialog(qobject_cast<QWidget*>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle("Downloads");
    dialog->resize(400, 300);
    dialog->setStyleSheet("background-color: #1e1e2e; color: #cdd6f4;");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QListWidget *list = new QListWidget(dialog);

    for (auto download : m_downloads) {
        list->addItem(download->downloadFileName());
    }

    layout->addWidget(list);
    dialog->show();
}
