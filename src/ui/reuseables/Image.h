#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QString>
#include <QPixmap>

#include <string>

class Image : public QWidget
{
    Q_OBJECT

public:
    explicit Image(const QString &src, QWidget *parent = nullptr);
    void changeSource(const QString &src);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QString source_;
    QPixmap pixmap_;
    QLabel *image_;
    QVBoxLayout *layout_;

    void updatePixmap();
};