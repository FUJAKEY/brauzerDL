#pragma once
#include <QDialog>
#include <QTabWidget>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>

class HistoryManager;

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(HistoryManager *historyMgr, QWidget *parent = nullptr);

private slots:
    void applySettings();
    void updateHistoryList();
    void clearAllHistory();
    void deleteSelectedHistory();
    void clearCookies();

private:
    HistoryManager *m_historyMgr;
    QTabWidget *m_tabs;

    // General Tab
    QComboBox *m_searchCombo;
    QComboBox *m_langCombo;

    // Privacy Tab
    QListWidget *m_historyList;
    QPushButton *m_clearHistoryBtn;
    QPushButton *m_deleteHistoryItemBtn;
    QPushButton *m_clearCookiesBtn;

    void setupUi();
    void loadCurrentSettings();
};
