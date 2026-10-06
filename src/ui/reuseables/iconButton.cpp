#include "iconButton.hpp"

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QIcon>
#include <QSize>
#include <Qt>

IconButton::IconButton(const QString &icon, QWidget *parent) : QPushButton(parent)
{
    setIcon(QIcon(icon));
    setCursor(Qt::PointingHandCursor);
    updateIconSize();
}

void IconButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
    updateIconSize();
}

void IconButton::updateIconSize()
{
    int dimension = std::min(width(), height());
    int iconDimension = dimension / 2;

    setIconSize(QSize(iconDimension, iconDimension));
}