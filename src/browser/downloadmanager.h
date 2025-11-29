#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QWebEngineDownloadRequest>
#include <QVector>

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(QObject *parent = nullptr);
    void showDownloadsDialog();

public slots:
    void handleDownloadRequested(QWebEngineDownloadRequest *download);

private:
    QVector<QWebEngineDownloadRequest*> m_downloads;
};

#endif // DOWNLOADMANAGER_H
