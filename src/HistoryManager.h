#pragma once
#include <QObject>
#include <QStringList>
#include <QDateTime>

struct HistoryItem {
    QString title;
    QString url;
    QDateTime timestamp;
};

class HistoryManager : public QObject {
    Q_OBJECT
public:
    explicit HistoryManager(QObject *parent = nullptr);
    void addHistoryItem(const QString &title, const QString &url);
    QList<HistoryItem> getHistory() const;
    void clearHistory();
    void removeHistoryItem(int index);

signals:
    void historyChanged();

private:
    void loadHistory();
    void saveHistory();
    QList<HistoryItem> m_history;
    QString m_historyFile;
};
