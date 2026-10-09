#pragma once

#include "FlowLayout.h"

#include <QWidget>
#include <QGridLayout>
#include <QEvent>

class GridWidget : public QWidget
{
public:
    GridWidget(QWidget *parent = nullptr);
    void addWidget(QWidget *widget);

private:
    FlowLayout *layout_;
    QVector<QWidget *> widgets_;
};