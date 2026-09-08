#ifndef THEME_MANAGER_H
#define THEME_MANAGER_H

#include <QApplication>
#include <QString>

class ThemeManager
{
public:
    static void loadThemeFile(const QString &theme);
};

#endif