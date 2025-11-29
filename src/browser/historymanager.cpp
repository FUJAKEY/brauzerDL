#include "historymanager.h"
#include <QDialog>
#include <QListWidget>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QSettings>
#include <QStandardPaths>

HistoryManager::HistoryManager(QObject *parent) : QObject(parent)
{
    loadHistory();
}

void HistoryManager::addHistoryItem(const QString &title, const QUrl &url)
{
    if (url.isEmpty()) return;

    // Simple duplicate check for the last item
    if (!m_history.isEmpty() && m_history.last().url == url) {
        return;
    }

    HistoryItem item;
    item.title = title.isEmpty() ? url.toString() : title;
    item.url = url;
    item.timestamp = QDateTime::currentDateTime();

    m_history.append(item);
    saveHistory();
}

void HistoryManager::saveHistory()
{
    QSettings settings("CandyCorp", "CandyBrowser");
    settings.beginWriteArray("history");
    for (int i = 0; i < m_history.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("title", m_history.at(i).title);
        settings.setValue("url", m_history.at(i).url);
        settings.setValue("timestamp", m_history.at(i).timestamp);
    }
    settings.endArray();
}

void HistoryManager::loadHistory()
{
    QSettings settings("CandyCorp", "CandyBrowser");
    int size = settings.beginReadArray("history");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        HistoryItem item;
        item.title = settings.value("title").toString();
        item.url = settings.value("url").toUrl();
        item.timestamp = settings.value("timestamp").toDateTime();
        m_history.append(item);
    }
    settings.endArray();
}

void HistoryManager::showHistoryDialog()
{
    QDialog *dialog = new QDialog(qobject_cast<QWidget*>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle("History");
    dialog->resize(400, 500);
    dialog->setStyleSheet("background-color: #1e1e2e; color: #cdd6f4;");

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    QListWidget *list = new QListWidget(dialog);
    list->setStyleSheet("QListWidget { background: #181825; border: 1px solid #313244; } QListWidget::item { padding: 10px; } QListWidget::item:selected { background: #313244; }");

    for (const auto &item : m_history) {
        QString text = QString("[%1] %2").arg(item.timestamp.toString("hh:mm"), item.title);
        QListWidgetItem *listItem = new QListWidgetItem(text);
        listItem->setData(Qt::UserRole, item.url);
        list->addItem(listItem);
    }

    connect(list, &QListWidget::itemDoubleClicked, [this, dialog](QListWidgetItem *item){
        QUrl url = item->data(Qt::UserRole).toUrl();
        emit requestOpenUrl(url);
        dialog->accept();
    });

    layout->addWidget(list);
    dialog->show();
}
