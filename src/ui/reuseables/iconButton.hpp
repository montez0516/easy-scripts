#pragma once

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QEvent>

class IconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit IconButton(const QString &icon, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateIconSize();
};