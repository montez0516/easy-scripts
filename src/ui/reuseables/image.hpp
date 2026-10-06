#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include <string>

class Image : public QWidget
{
public:
    Image(const std::string &src, QWidget *parent = nullptr);
    void setSource(const std::string &src);

private:
    std::string source_;
    QLabel *image_;
    QVBoxLayout *layout_;
}