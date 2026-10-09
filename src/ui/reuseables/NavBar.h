#pragma once

#include <QWidget>
#include <QBoxLayout>
#include <QSize>

class NavBar : public QWidget
{
public:
    NavBar(QBoxLayout::Direction direction, QWidget *parent = nullptr);
    void addWidget(QWidget *widget);

private:
    QBoxLayout *layout_;
};