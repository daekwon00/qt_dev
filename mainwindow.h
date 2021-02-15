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
    QString PCDataPath = "C:\\Carima\\Datas\\";
    QString USBPath = "F:\\";
#else
    QString PCDataPath = QDir::homePath() + "/carima/Datas/";
    QString USBPath = "/media/dkyoo/8FA1-B98F/";  // /media/dkyoo/8FA1-B98F
#endif
    QString selectName = "";

protected:
    void showEvent(QShowEvent* event);

private slots:
    void moveHome();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_refresh_clicked();

    void on_pushButton_pc_usb_clicked();

    void on_pushButton_copy_clicked();

    void on_pushButton_Delete_clicked();

    void on_pushButton_next_clicked();

    void on_combobox_options_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    void initConnect();
    void initForm();
    void setListWidget(QString data_path);
    int scanDir(QDir dir, QString type);
    int chkConnectedUSB();
    bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist);

    Form_sub _sub;
    int current_stackedWidget = 0;
    bool bool_usb_connected = false;
};
#endif // MAINWINDOW_H
