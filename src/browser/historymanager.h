#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QObject>
#include <QUrl>
#include <QDateTime>
#include <QVector>

struct HistoryItem {
    QString title;
    QUrl url;
    QDateTime timestamp;
};

class HistoryManager : public QObject
{
    Q_OBJECT

public:
    explicit HistoryManager(QObject *parent = nullptr);
    void addHistoryItem(const QString &title, const QUrl &url);
    void showHistoryDialog();

signals:
    void requestOpenUrl(const QUrl &url);

private:
    void loadHistory();
    void saveHistory();
    QVector<HistoryItem> m_history;
};

#endif // HISTORYMANAGER_H
