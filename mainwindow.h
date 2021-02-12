#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <form_sub.h>
#include <QListWidgetItem>
#include <QSettings>
#include <QDebug>
#include <QDir>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

#ifdef QT_WINDOWS_PATH
    QString DataPath = "C:\\Carima\\Datas";
#else
    QString DataPath = QDir::homePath() + "/carima/Datas";
#endif

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void moveHome();

    void moveSub();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    void initConnect();
    void initForm();
    void setListWidget();
    void scanDir(QDir dir);
    void scanFile(QDir dir);

    Form_sub _sub;
};
#endif // MAINWINDOW_H
