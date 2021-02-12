#ifndef FORM_SUB_H
#define FORM_SUB_H

#include <QWidget>

namespace Ui {
class Form_sub;
}

class Form_sub : public QWidget
{
    Q_OBJECT

public:
    explicit Form_sub(QWidget *parent = nullptr);
    ~Form_sub();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::Form_sub *ui;

signals:
    void HomeClicked();
};

#endif // FORM_SUB_H
