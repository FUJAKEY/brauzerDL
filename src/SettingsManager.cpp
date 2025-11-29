#include "SettingsManager.h"

SettingsManager& SettingsManager::instance() {
    static SettingsManager instance;
    return instance;
}

SettingsManager::SettingsManager() : m_settings("SweetSoft", "SweetBrowser") {
    m_searchEngine = static_cast<SearchEngine>(m_settings.value("searchEngine", Google).toInt());
    m_language = static_cast<Language>(m_settings.value("language", English).toInt());
}

SettingsManager::SearchEngine SettingsManager::searchEngine() const {
    return m_searchEngine;
}

void SettingsManager::setSearchEngine(SearchEngine engine) {
    if (m_searchEngine != engine) {
        m_searchEngine = engine;
        save();
        emit settingsChanged();
    }
}

SettingsManager::Language SettingsManager::language() const {
    return m_language;
}

void SettingsManager::setLanguage(Language lang) {
    if (m_language != lang) {
        m_language = lang;
        save();
        emit settingsChanged();
    }
}

void SettingsManager::save() {
    m_settings.setValue("searchEngine", m_searchEngine);
    m_settings.setValue("language", m_language);
}
