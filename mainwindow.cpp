#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setFixedSize(1400, 750);
    move(100, 20);
    pl.setColor(QPalette::Window, QColor(244,244,244));
    setPalette(pl);

    //полоска сверху
    stripe = new QWidget(this);
    stripe->setGeometry(0,0,1400,150);
    pl.setColor(QPalette::Window, QColor(44,103,115));
    stripe->setPalette(pl);
    stripe->setAutoFillBackground(true);
    stripe->show();

    //текст на полоске
    textTitle = new QLabel("Назначения", stripe);
    QFont font("Cascadia Code", 74);
    textTitle->setFont(font);
    textTitle->setStyleSheet("color: rgb(244, 244, 244);");

    textTitle->setAlignment(Qt::AlignVCenter);
    QHBoxLayout *stripeLayout = new QHBoxLayout(stripe);
    stripeLayout->addWidget(textTitle);
    stripeLayout->setAlignment(Qt::AlignCenter);


    //боковое меню
    SideMenu();


    matrixButton = PushButtonMenu( 35, 120, "Матрица");
    fileButton = PushButtonMenu( 35, 200, "Из файла");
    randomButton = PushButtonMenu( 35, 280, "Случайная \nгенерация");



    connect(matrixButton, &QPushButton::clicked, this, &MainWindow::Matrix);
    connect(fileButton, &QPushButton::clicked, this, &MainWindow::File);
    connect(randomButton, &QPushButton::clicked, this, &MainWindow::Random);








}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SideMenu(){
    menu = new QWidget(this);
    pl.setColor(QPalette::Window, QColor(44,154,176));
    menu->setPalette(pl);
    menu->setAutoFillBackground(true);
    menu->setGeometry(0, 150, 300,600);


    textMenu = new QLabel("Способ ввода \nданных:", menu);
    QFont font("Cascadia Code", 25 );
    textMenu->setFont(font);

    textMenu->setStyleSheet("color: rgb(244, 244, 244);");
    textMenu->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QHBoxLayout *stripeLayout = new QHBoxLayout(menu);
    stripeLayout->addWidget(textMenu);
    stripeLayout->setAlignment(textMenu, Qt::AlignLeft | Qt::AlignTop);


}


QPushButton* MainWindow::PushButtonMenu( int x, int y, const QString &text)
{
    QPushButton *button = new QPushButton(text, menu);
    QFont font("Cascadia Code", 20);
    button->setFont(font);

    button->setStyleSheet("color: rgb(244, 244, 244);");
    pl.setColor(QPalette::Button, QColor(44,154,176));
    button->setPalette(pl);
    button->setGeometry(x, y, 200, 70); // Задайте размеры кнопки (ширину и высоту) по вашему усмотрению
    return button;
}



void MainWindow::Matrix(){



    clearMenuExceptButtons();

    countLabel = new QLabel("Количество \nработников:", menu);
    countSpinBox = new QSpinBox(menu);
    countSpinBox->setMinimum(1);  // Минимальное значение
    countSpinBox->setMaximum(10);

    QFont font("Cascadia Code", 15);
    countLabel->setFont(font);
    countLabel->setStyleSheet("color: rgb(244, 244, 244);");
    countLabel->setGeometry(35, 350, 150, 80);
    countLabel->show();
    countSpinBox->setGeometry(200, 370, 50,40);
    countSpinBox->show();


    choice = true;
    DaleeButtom = this->DataEntryButton();
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::createMatrix);






}

void MainWindow::createMatrix()
{
    clearMenuExceptButtons();
    matrixTable = new QTableWidget(countSpinBox->value(), countSpinBox->value(), menu);
    for (int i = 0; i < countSpinBox->value(); ++i) {
        matrixTable->setColumnWidth(i, 200/countSpinBox->value()); // Adjust the width as needed
    }
    matrixTable->setGeometry(10, 350, 280, 200);
    matrixTable->show();
    DaleeButtom = this->DataEntryButton();
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::checkAllCellsFilled);


}

void MainWindow::checkAllCellsFilled()
{

    bool allFilled = true;

    for (int i = 0; i < matrixTable->rowCount(); ++i) {
        for (int j = 0; j < matrixTable->columnCount(); ++j) {
            QTableWidgetItem *item = matrixTable->item(i, j);
            if (!item ||item->text().isEmpty()) {
                allFilled = false;
                break;
            }
        }
        if (!allFilled) {
            break;
        }
    }

    if (allFilled){

        handleFilledMatrix();
    }


}

void MainWindow::handleFilledMatrix()
{
    sizeMatrix = matrixTable->rowCount();

    for (int i = 0; i < matrixTable->rowCount(); ++i) {
        for (int j = 0; j < matrixTable->rowCount(); ++j) {
            QTableWidgetItem *item = matrixTable->item(i, j);
            if (item) {
                valMatrix.append(item->text()) ;
            }
        }
    }


}

void MainWindow::File(){

    clearMenuExceptButtons();


    QLabel *text = new QLabel("Название файла:", menu);
    text->setFont(QFont("Cascadia Code", 15));
    text->setStyleSheet("color: rgb(244, 244, 244);");
    text->move(35, 370);
    text->show();

    line = new QLineEdit(menu);
    line->setGeometry(35, 400, 200, 30);
    line->show();
    this->choice = true;
    DaleeButtom = this->DataEntryButton();
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::ReadLine);


}
void MainWindow::ReadLine(){
    fileName = line->text();
}


void MainWindow::Random(){
    clearMenuExceptButtons();
    countLabel = new QLabel("Количество \nработников:", menu);
    countSpinBox = new QSpinBox(menu);
    countSpinBox->setMinimum(1);  // Минимальное значение
    countSpinBox->setMaximum(10);

    QFont font("Cascadia Code", 15);
    countLabel->setFont(font);
    countLabel->setStyleSheet("color: rgb(244, 244, 244);");
    countLabel->setGeometry(35, 350, 150, 80);
    countLabel->show();
    countSpinBox->setGeometry(200, 370, 50,40);
    countSpinBox->show();


    choice = true;
    DaleeButtom = this->DataEntryButton();
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::getSize);
}

void MainWindow::getSize(){
    sizeMatrix = countSpinBox->value();

}
QPushButton* MainWindow::DataEntryButton(){
    if (choice){
        QPushButton *button = new QPushButton("Далее", menu);
        QFont font("Cascadia Code", 17);
        button->setFont(font);
        button->setStyleSheet("color: rgb(244, 244, 244);");
        button->setGeometry(200, 550, 90, 40);
        pl.setColor(QPalette::Button, QColor(44,103,115));
        button->setPalette(pl);
        button->show();
        return button;

    }
    return nullptr;
}

void MainWindow::clearMenuExceptButtons()
{

    QList<QWidget*> children = this->findChildren<QWidget*>();

    for (QWidget *child : children) {

        if (child != matrixButton && child != fileButton && child != randomButton && child != textMenu && child != DaleeButtom && child != menu && child != stripe && child != textTitle) {
            child->hide(); /
            child->deleteLater();
        }
    }
}
