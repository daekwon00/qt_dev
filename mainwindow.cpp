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
    if(chkConnectedUSB() > 0)
        setListWidget(USBPath);
    else
        setListWidget(PCDataPath);
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
        ui->pushButton_pc_usb->setText("PC Search");
        ui->pushButton_Delete->setDisabled(false);
    }else{
        ui->pushButton_pc_usb->setText("USB Search");
        bool_usb_connected = true;
        ui->pushButton_Delete->setDisabled(true);
    }
}

void MainWindow::on_pushButton_copy_clicked()
{

}

void MainWindow::on_pushButton_Delete_clicked()
{

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
#ifdef QT_WINDOWS_PATH
    QSettings Settings(PCDataPath + item->text(), QSettings::IniFormat);
#else
    QSettings Settings(PCDataPath + item->text(), QSettings::IniFormat);
#endif
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
        QFileInfo info_list(dir.filePath(string_List.at(i))); // use Qfileinfo
        ui->listWidget->addItem(info_list.fileName());    // add to listwidget
        listCount++;
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
