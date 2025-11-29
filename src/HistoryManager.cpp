#include "HistoryManager.h"
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

HistoryManager::HistoryManager(QObject *parent) : QObject(parent) {
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    if (!dir.exists()) dir.mkpath(".");

    m_historyFile = dataPath + "/history.txt";
    loadHistory();
}

void HistoryManager::addHistoryItem(const QString &title, const QString &url) {
    if (url.isEmpty()) return;

    HistoryItem item;
    item.title = title;
    item.url = url;
    item.timestamp = QDateTime::currentDateTime();

    m_history.prepend(item);
    // Limit history size
    if (m_history.size() > 1000) m_history.removeLast();

    saveHistory();
    emit historyChanged();
}

void HistoryManager::clearHistory() {
    m_history.clear();
    saveHistory();
    emit historyChanged();
}

void HistoryManager::removeHistoryItem(int index) {
    if (index >= 0 && index < m_history.size()) {
        m_history.removeAt(index);
        saveHistory();
        emit historyChanged();
    }
}

QList<HistoryItem> HistoryManager::getHistory() const {
    return m_history;
}

void HistoryManager::loadHistory() {
    QFile file(m_historyFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split("|");
        if (parts.size() >= 3) {
            HistoryItem item;
            item.timestamp = QDateTime::fromString(parts[0], Qt::ISODate);
            item.url = parts[1];
            item.title = parts.mid(2).join("|"); // Rejoin title if it contained pipe
            m_history.append(item);
        }
    }
}

void HistoryManager::saveHistory() {
    QFile file(m_historyFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) return;

    QTextStream out(&file);
    for (const auto &item : m_history) {
        out << item.timestamp.toString(Qt::ISODate) << "|"
            << item.url << "|"
            << item.title << "\n";
    }
}
