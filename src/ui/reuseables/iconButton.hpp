#pragma once

#include "image.hpp"

#include <QPushButton>
#include <QString>
#include <QEvent>
#include <QLabel>
#include <QVBoxLayout>

class IconButton : public QPushButton
{
    Q_OBJECT
public:
    explicit IconButton(const QString &text, const QString &icon, QWidget *parent = nullptr);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Image *icon_;
    QLabel *text_;
    QVBoxLayout *layout_;
};