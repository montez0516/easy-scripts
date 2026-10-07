#include "createPage.hpp"
#include "../../api/register.hpp"

#include <QWidget>
#include <QVBoxLayout>

CreatePage::CreatePage(APIRegister &apiRegister, QWidget *parent) : QWidget(parent), apiRegister_(apiRegister)
{
    setObjectName("createPage");
    layout_ = new QVBoxLayout(this);
}