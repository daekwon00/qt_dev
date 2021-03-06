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
    current_stackedWidget = 0;
    // add page
    ui->stackedWidget->insertWidget(1, &_sub);

    //combobox
    ui->combobox_options->addItem("all");
    ui->combobox_options->addItem("directory");
    ui->combobox_options->addItem("files");
    ui->combobox_options->addItem("slice");
    ui->combobox_options->addItem("cfg");

    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
//    setListWidget();
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    if(chkConnectedUSB() > 0){
        bool_usb_connected = true;
        ui->pushButton_pc_usb->setText("USB");
        ui->pushButton_copy->setDisabled(false);
        setListWidget(USBPath);
    }else{
        bool_usb_connected = false;
        ui->pushButton_pc_usb->setText("PC");
        ui->pushButton_copy->setDisabled(true);
        setListWidget(PCDataPath);
    }
}

void MainWindow::setListWidget(QString data_path)
{
    ui->listWidget->clear();
    QDir dir(data_path);
    scanDir(dir, "all");
    ui->listWidget->repaint();
}

// signal
void MainWindow::moveHome()
{
    ui->stackedWidget->setCurrentIndex(0);
    current_stackedWidget = 0;
}

void MainWindow::on_pushButton_refresh_clicked()
{
    selectName = "";
    QString Taget_path ="";
    if (bool_usb_connected)
        Taget_path = USBPath;
    else
        Taget_path = PCDataPath;

    ui->listWidget->clear();
    QDir dir(Taget_path);
    int countList = scanDir(dir, ui->combobox_options->currentText());
    ui->listWidget->repaint();

    if (countList == 0) {
        QMessageBox::information(this, "Files", "No Files found");
    }
}

void MainWindow::on_pushButton_pc_usb_clicked()
{
    if (bool_usb_connected){
        bool_usb_connected = false;
        ui->pushButton_pc_usb->setText("PC");
        ui->pushButton_copy->setDisabled(true);
        setListWidget(PCDataPath);
    }else{
        bool_usb_connected = true;
        ui->pushButton_pc_usb->setText("USB");
        ui->pushButton_copy->setDisabled(false);
        setListWidget(USBPath);
    }
}

void MainWindow::on_pushButton_copy_clicked()
{
    if (selectName == "")
        QMessageBox::warning(this, "No item", "No Files found");
    else{
        ui->statusbar->showMessage("Copy :" + selectName, 0);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Copy", "copy " + selectName + " ??",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
        } else {
            qDebug() << "Yes was *not* clicked";
            return;
        }
    }

    QString target_path = USBPath + selectName;
    QFileInfo checkDir(target_path);

    if (checkDir.isDir()){
        qDebug() << "==> dir : " << selectName;
        copyDirectoryFiles(target_path, PCDataPath + selectName, true);
    }else if(checkDir.isFile()){
        qDebug() << "==> file : " << selectName;
        QFile::copy(target_path, PCDataPath + selectName);
    }
}

void MainWindow::on_pushButton_Delete_clicked()
{
    if (selectName == "")
        QMessageBox::warning(this, "No item", "No Files found");
    else{
        ui->statusbar->showMessage("delete : " + selectName, 0);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete", "Delete " + selectName + " ??",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            qDebug() << "Yes was clicked";
        } else {
            qDebug() << "Yes was *not* clicked";
            return;
        }
    }

    QString target_path="";
    if(bool_usb_connected)
        target_path = USBPath + selectName;
    else
        target_path = PCDataPath + selectName;

    QFileInfo checkDir(target_path);

    if (checkDir.isDir()){
        qDebug() << "==> dir : " << selectName;
        QDir dir(target_path);
        dir.removeRecursively();
    }else if(checkDir.isFile()){
        qDebug() << "==> file : " << selectName;
        QFile::remove(target_path);
    }
}

void MainWindow::on_pushButton_next_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
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
    //    QSettings Settings(PCDataPath + item->text(), QSettings::IniFormat);
    selectName = item->text();

    ui->statusbar->showMessage("selected : " + selectName, 0);
}

int MainWindow::scanDir(QDir dir, QString type)
{
    int listCount = 0;
    qDebug() << "Scanning: " << dir.path();

    if (type == "directory"){
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    } else if (type == "files") {
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    } else if (type == "slice") {
        dir.setNameFilters(QStringList("*.slice"));
        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    } else if (type == "cfg") {
        dir.setNameFilters(QStringList("*.cfg"));
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    } else {
        dir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    }

    QStringList string_List = dir.entryList();

    for (int i=0; i<string_List.size(); ++i)
    {
        QString file_name = string_List.at(i);
        ui->listWidget->addItem(file_name);
        listCount++;

        QString file_path = dir.filePath(string_List.at(i));
        QFileInfo checkDir(file_path);

        if (checkDir.isDir())
            qDebug() << "==> dir : " << file_name << ", " << file_path;
        else if(checkDir.isFile())
            qDebug() << "==> file : " << file_name << ", " << file_path;
    }

    return listCount;
}

int MainWindow::chkConnectedUSB()
{
    int listCount = 0;
    QDir dir(USBPath);
    dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList string_List = dir.entryList();

    for(int i = 0; i < string_List.count(); i++){
        listCount++;
    }
    return listCount;
}

void MainWindow::on_combobox_options_currentIndexChanged(int index)
{
    qDebug() << "ComboBox : " << index << ", " << ui->combobox_options->currentText();
}

bool MainWindow::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /* if directory don't exists, build it */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /* if it is directory, copy recursively*/
            if(!copyDirectoryFiles(fileInfo.filePath(),
                                   targetDir.filePath(fileInfo.fileName()),
                                   coverFileIfExist))
                return false;
        }
        else{            /* if coverFileIfExist == true, remove old file first */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            // files copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }
    return true;
}
