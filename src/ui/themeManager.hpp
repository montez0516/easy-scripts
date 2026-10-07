#pragma once

#include <QApplication>
#include <QString>

class ThemeManager
{
public:
    static void loadThemeFile(const QString &theme);
    static QString currentTheme();

private:
    static QString theme_;
};
