#pragma once
#include <QObject>
#include <QWebEngineDownloadRequest>

class DownloadManager : public QObject {
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);

public slots:
    void handleDownloadRequested(QWebEngineDownloadRequest *download);
};
