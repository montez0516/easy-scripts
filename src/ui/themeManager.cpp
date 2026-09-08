#include "themeManager.hpp"

#include <spdlog/spdlog.h>

#include <QApplication>
#include <QString>
#include <QFile>

void ThemeManager::loadThemeFile(const QString &theme)
{
    QString fileName = "./themes/" + theme + ".qss";
    spdlog::debug("ThemeManager: {}", fileName.toStdString());

    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        spdlog::error("ThemeManager(loadThemeFile): failed to open theme file");
        return;
    }

    qApp->setStyleSheet(file.readAll());
}
