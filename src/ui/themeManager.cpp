#include "ThemeManager.h"
#include "../core/Paths.h"

#include <spdlog/spdlog.h>

#include <QApplication>
#include <QString>
#include <QFile>

#include <filesystem>

QString ThemeManager::theme_;

void ThemeManager::loadThemeFile(Paths &paths, const QString &theme)
{
    std::filesystem::path themesDir = paths.themes();
    QString themesFile = QString::fromStdString((themesDir / (theme.toStdString() + ".qss")).string());

    QFile file(themesFile);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        spdlog::error("ThemeManager(loadThemeFile): failed to open theme file");
        return;
    }

    qApp->setStyleSheet(file.readAll());

    theme_ = theme;
}

QString ThemeManager::currentTheme()
{
    return theme_;
}