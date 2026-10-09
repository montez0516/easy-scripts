#pragma once

#include "../core/Paths.h"

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
