#include "SettingsDialog.h"
#include "SettingsManager.h"
#include "HistoryManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>

SettingsDialog::SettingsDialog(HistoryManager *historyMgr, QWidget *parent)
    : QDialog(parent), m_historyMgr(historyMgr)
{
    setWindowTitle("SweetBrowser Settings");
    resize(600, 400);
    setupUi();
    loadCurrentSettings();
    updateHistoryList();

    connect(m_historyMgr, &HistoryManager::historyChanged, this, &SettingsDialog::updateHistoryList);
}

void SettingsDialog::setupUi() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    m_tabs = new QTabWidget(this);
    mainLayout->addWidget(m_tabs);

    // --- General Tab ---
    QWidget *generalTab = new QWidget();
    QVBoxLayout *genLayout = new QVBoxLayout(generalTab);

    // Search Engine
    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(new QLabel("Search Engine:"));
    m_searchCombo = new QComboBox();
    m_searchCombo->addItem("Google");
    m_searchCombo->addItem("Yandex");
    m_searchCombo->addItem("DuckDuckGo");
    searchLayout->addWidget(m_searchCombo);
    genLayout->addLayout(searchLayout);

    // Language
    QHBoxLayout *langLayout = new QHBoxLayout();
    langLayout->addWidget(new QLabel("Language:"));
    m_langCombo = new QComboBox();
    m_langCombo->addItem("English");
    m_langCombo->addItem("Russian");
    m_langCombo->addItem("Ukrainian");
    langLayout->addWidget(m_langCombo);
    genLayout->addLayout(langLayout);

    genLayout->addStretch();

    // Apply Button
    QPushButton *applyBtn = new QPushButton("Apply");
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::applySettings);
    genLayout->addWidget(applyBtn);

    m_tabs->addTab(generalTab, "General");

    // --- Privacy Tab ---
    QWidget *privacyTab = new QWidget();
    QVBoxLayout *privLayout = new QVBoxLayout(privacyTab);

    // Cookies
    QHBoxLayout *cookieLayout = new QHBoxLayout();
    m_clearCookiesBtn = new QPushButton("Clear All Cookies");
    connect(m_clearCookiesBtn, &QPushButton::clicked, this, &SettingsDialog::clearCookies);
    cookieLayout->addWidget(new QLabel("Cookies:"));
    cookieLayout->addWidget(m_clearCookiesBtn);
    privLayout->addLayout(cookieLayout);

    // History
    privLayout->addWidget(new QLabel("History:"));
    m_historyList = new QListWidget();
    privLayout->addWidget(m_historyList);

    QHBoxLayout *histBtnLayout = new QHBoxLayout();
    m_clearHistoryBtn = new QPushButton("Clear All History");
    m_deleteHistoryItemBtn = new QPushButton("Delete Selected");

    connect(m_clearHistoryBtn, &QPushButton::clicked, this, &SettingsDialog::clearAllHistory);
    connect(m_deleteHistoryItemBtn, &QPushButton::clicked, this, &SettingsDialog::deleteSelectedHistory);

    histBtnLayout->addWidget(m_clearHistoryBtn);
    histBtnLayout->addWidget(m_deleteHistoryItemBtn);
    privLayout->addLayout(histBtnLayout);

    m_tabs->addTab(privacyTab, "Privacy");

    // --- About Tab ---
    QWidget *aboutTab = new QWidget();
    QVBoxLayout *aboutLayout = new QVBoxLayout(aboutTab);
    QLabel *aboutLabel = new QLabel("<h2>SweetBrowser v2.0</h2><p>Designed with Liquid Glass Style.</p>");
    aboutLabel->setAlignment(Qt::AlignCenter);
    aboutLayout->addWidget(aboutLabel);
    m_tabs->addTab(aboutTab, "About");
}

void SettingsDialog::loadCurrentSettings() {
    auto &settings = SettingsManager::instance();
    m_searchCombo->setCurrentIndex(settings.searchEngine());
    m_langCombo->setCurrentIndex(settings.language());
}

void SettingsDialog::applySettings() {
    auto &settings = SettingsManager::instance();
    settings.setSearchEngine(static_cast<SettingsManager::SearchEngine>(m_searchCombo->currentIndex()));
    settings.setLanguage(static_cast<SettingsManager::Language>(m_langCombo->currentIndex()));
    accept();
}

void SettingsDialog::updateHistoryList() {
    m_historyList->clear();
    auto items = m_historyMgr->getHistory();
    for (const auto &item : items) {
        m_historyList->addItem(item.timestamp.toString("yyyy-MM-dd HH:mm") + " - " + item.title + " (" + item.url + ")");
    }
}

void SettingsDialog::clearAllHistory() {
    if (QMessageBox::question(this, "Confirm", "Delete all history?") == QMessageBox::Yes) {
        m_historyMgr->clearHistory();
    }
}

void SettingsDialog::deleteSelectedHistory() {
    int row = m_historyList->currentRow();
    if (row >= 0) {
        m_historyMgr->removeHistoryItem(row);
    }
}

void SettingsDialog::clearCookies() {
    if (QMessageBox::question(this, "Confirm", "Delete all cookies?") == QMessageBox::Yes) {
        QWebEngineProfile::defaultProfile()->cookieStore()->deleteAllCookies();
        QMessageBox::information(this, "Info", "Cookies cleared.");
    }
}
