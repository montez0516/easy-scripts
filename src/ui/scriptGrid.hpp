#ifndef SCRIPT_GRID_H
#define SCRIPT_GRID_H

#include "scriptCard.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QEvent>

class ScriptGrid : public QWidget
{
public:
    ScriptGrid(QWidget *parent = nullptr);
    void addScript(ScriptCard *scriptCard);
    void updateGrid();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGridLayout *layout_;
    QVector<ScriptCard *> scripts_;
};

#endif