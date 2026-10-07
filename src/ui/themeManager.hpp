#pragma once

#include "../core/paths.hpp"

#include <QApplication>
#include <QString>

class ThemeManager
{
public:
    static void loadThemeFile(Paths &paths, const QString &theme);
    static QString currentTheme();

private:
    static QString theme_;
};
