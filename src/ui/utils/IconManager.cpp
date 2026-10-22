#include "IconManager.h"
#include "ThemeManager.h"
#include "../../core/Paths.h"

#include <QString>

#include <filesystem>

QString IconManager::getIcon(const QString &name)
{
    QString currentTheme = ThemeManager::currentTheme();
    std::filesystem::path iconFolder = Paths::icons() / currentTheme.toStdString();

    QString iconName = name + "-" + currentTheme + ".svg";
    std::filesystem::path iconPath = iconFolder / iconName.toStdString();

    if (!std::filesystem::exists(iconPath))
        return getIconFallback(name);

    return QString::fromStdString(iconPath.string());
}

QString IconManager::getIconFallback(const QString &name)
{
    std::filesystem::path iconFolder = Paths::icons();

    if (!std::filesystem::exists(iconFolder))
        return "";

    QString iconName = name + ".svg";
    std::filesystem::path iconPath = iconFolder / iconName.toStdString();

    if (!std::filesystem::exists(iconPath))
        return "";

    return QString::fromStdString(iconPath.string());
}