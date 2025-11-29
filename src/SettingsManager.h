#pragma once
#include <QObject>
#include <QSettings>

class SettingsManager : public QObject {
    Q_OBJECT
public:
    enum SearchEngine {
        Google,
        Yandex,
        DuckDuckGo
    };

    enum Language {
        English,
        Russian,
        Ukrainian
    };

    static SettingsManager& instance();

    SearchEngine searchEngine() const;
    void setSearchEngine(SearchEngine engine);

    Language language() const;
    void setLanguage(Language lang);

    void save();

signals:
    void settingsChanged();

private:
    SettingsManager();
    QSettings m_settings;
    SearchEngine m_searchEngine;
    Language m_language;
};
