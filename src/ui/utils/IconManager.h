#pragma once

#include "../../core/Paths.h"
#include "ThemeManager.h"

#include <QString>

#include <filesystem>

class IconManager
{
public:
    static QString getIcon(const QString &name);

private:
    static QString getIconFallback(const QString &name);
};