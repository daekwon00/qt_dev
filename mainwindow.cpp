#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initConnect();
    initForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect()
{
    connect(&_sub, SIGNAL(HomeClicked()), this, SLOT(moveHome()));
}

void MainWindow::initForm()
{
    // add page
    ui->stackedWidget->insertWidget(1, &_sub);

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setListWidget();
}

void MainWindow::setListWidget()
{
    ui->listWidget->clear();
    QDir dir(DataPath);
    scanDir(dir);
    ui->listWidget->repaint();
}

// signal
void MainWindow::moveHome()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->clear();
    QDir dir(DataPath);
    scanDir(dir);
    ui->listWidget->repaint();
}

void MainWindow::moveSub()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->clear();
    QDir dir(DataPath);
    scanFile(dir);
    ui->listWidget->repaint();
}

void MainWindow::on_actionNew_triggered()
{
    QMessageBox::information(this, "title", "New");
}

void MainWindow::on_actionOpen_triggered()
{
    QMessageBox::information(this, "title", "open");
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << "Form_data itemClicked" << item->text();
#ifdef QT_WINDOWS_PATH
    QSettings Settings(DataPath + "\\" + item->text(), QSettings::IniFormat);
#else
    QSettings Settings(DataPath + "/" + item->text(), QSettings::IniFormat);
#endif
}

void MainWindow::scanDir(QDir dir)
{
    qDebug() << "Scanning: " << dir.path();
    dir.setNameFilters(QStringList("*.slice"));
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList string_List = dir.entryList();
    for (int i=0; i<string_List.size(); ++i)
    {
        QFileInfo info_list(dir.filePath(string_List.at(i))); // use Qfileinfo
        ui->listWidget->addItem(info_list.fileName());    // add to listwidget
//        qDebug() << "path : " << dir.filePath(dirList.at(i)) << ", name : " << dirinfo.fileName();
    }
}

void MainWindow::scanFile(QDir dir)
{
    qDebug() << "Scanning: " << dir.path();
//    dir.setNameFilters(QStringList("*.cfg"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList string_List = dir.entryList();
    for (int i=0; i<string_List.size(); ++i)
    {
        QFileInfo info_list(dir.filePath(string_List.at(i))); // use Qfileinfo
        ui->listWidget->addItem(info_list.fileName());    // add to listwidget
    }
}
