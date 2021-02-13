#include "form_sub.h"
#include "ui_form_sub.h"

Form_sub::Form_sub(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_sub)
{
    ui->setupUi(this);
}

Form_sub::~Form_sub()
{
    delete ui;
}

void Form_sub::on_pushButton_previous_clicked()
{
    emit HomeClicked();
}
