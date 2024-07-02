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
    font.setPointSize(74);
    textTitle->setFont(font);
    textTitle->setStyleSheet("color: rgb(244, 244, 244);");

    textTitle->setAlignment(Qt::AlignVCenter);
    QHBoxLayout *stripeLayout = new QHBoxLayout(stripe);
    stripeLayout->addWidget(textTitle);
    stripeLayout->setAlignment(Qt::AlignCenter);


    //боковое меню
    SideMenu();

    //решение



    matrixButton = PushButtonMenu( 35, 80, "Матрица");
    fileButton = PushButtonMenu( 35, 160, "Из файла");
    randomButton = PushButtonMenu( 35, 240, "Случайная \nгенерация");



    connect(matrixButton, &QPushButton::clicked, this, &MainWindow::Matrix);
    connect(fileButton, &QPushButton::clicked, this, &MainWindow::File);
    connect(randomButton, &QPushButton::clicked, this, &MainWindow::Random);
    //SolutionMenu();


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
    font.setPointSize(21);
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
    font.setPointSize(20);
    button->setFont(font);

    button->setStyleSheet("color: rgb(244, 244, 244);");
    pl.setColor(QPalette::Button, QColor(44,154,176));
    button->setPalette(pl);
    button->setGeometry(x, y, 200, 70);
    return button;
}



void MainWindow::Matrix(){



    clearMenuExceptButtons();

    countLabel = new QLabel("Количество \nработников:", menu);
    countSpinBox = new QSpinBox(menu);
    countSpinBox->setMinimum(2);  // Минимальное значение
    countSpinBox->setMaximum(10);

    font.setPointSize(15);
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


    if (countSpinBox->value() < 7){
        matrixTable = new QTableWidget(countSpinBox->value(), countSpinBox->value(), menu);
        matrixTable->setGeometry(10, 350, 280, 200);
        for (int i = 0; i < countSpinBox->value(); ++i) {
            matrixTable->setColumnWidth(i, 260/countSpinBox->value());
            matrixTable->setRowHeight(i, 175/countSpinBox->value());
        }
    }
    else {
        matrixTable = new QTableWidget(countSpinBox->value(), countSpinBox->value());
        matrixTable->setGeometry(550, 300,countSpinBox->value()*40+30, countSpinBox->value()*30+35);
        for (int i = 0; i < countSpinBox->value(); ++i) {
            matrixTable->setColumnWidth(i, 40);
            matrixTable->setRowHeight(i, 30);
        }
    }
    //matrixTable->setStyleSheet("color: rgb(44,103,115);"
    //                           "background-color: rgb(244, 244, 244);");
    matrixTable->show();
    DaleeButtom = this->DataEntryButton();
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::closeMatrix);
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
            QLabel *textError = new QLabel("Ошибка в заполнении\nВыберите способ заново",menu);
            textError->setStyleSheet("color: red;");
            font.setPointSize(12);
            textError->setFont(font);
            textError->setGeometry(10,305, 280, 50);
            textError->show();
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
    QLabel *textError = new QLabel("Матрица заполнена",menu);
    textError->setStyleSheet("color: rgb(244, 244, 244);");
    font.setPointSize(12);
    textError->setFont(font);
    textError->setGeometry(10,305, 280, 50);
    textError->show();


}

void MainWindow::File(){

    clearMenuExceptButtons();


    QLabel *text = new QLabel("Название:", menu);
    font.setPointSize(15);
    text->setStyleSheet("color: rgb(244, 244, 244);");
    text->setFont(font);
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
    QLabel *textError = new QLabel("Файл считан",menu);
    textError->setStyleSheet("color: rgb(244, 244, 244);");
    font.setPointSize(12);
    textError->setFont(font);
    textError->setGeometry(10,305, 280, 50);
    textError->show();
}


void MainWindow::Random(){
    clearMenuExceptButtons();
    countLabel = new QLabel("Количество \nработников:", menu);
    countSpinBox = new QSpinBox(menu);
    countSpinBox->setMinimum(2);
    countSpinBox->setMaximum(10);

    font.setPointSize(15);
    countLabel->setFont(font);
    countLabel->setStyleSheet("color: rgb(244, 244, 244);");
    countLabel->setGeometry(35, 350, 150, 80);
    countLabel->show();
    countSpinBox->setGeometry(200, 370, 50,40);
    countSpinBox->show();


    choice = true;
    DaleeButtom = this->DataEntryButton();
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::getSizeRand);
}

void MainWindow::getSizeRand(){
    sizeMatrix = countSpinBox->value();
    QLabel *textError = new QLabel("Матрица сгенерирована",menu);
    textError->setStyleSheet("color: rgb(244, 244, 244);");
    font.setPointSize(12);
    textError->setFont(font);
    textError->setGeometry(35,305, 280, 50);
    textError->show();
    //SolutionMenu();

}

QPushButton* MainWindow::DataEntryButton(){
    if (choice){
        QPushButton *button = new QPushButton("Далее", menu);
        font.setPointSize(17);
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
            child->hide();
            child->deleteLater();
        }
    }
}


void MainWindow::SolutionMenu(){
    clearMenuExceptButtons();
    solution = new QWidget(this);
    solution->setGeometry(300, 150, 1150,600);
    SettingMenu();


}

void MainWindow::SettingMenu(){

    setting = new QWidget(solution);
    textMenu = new QLabel("Вероятность мутации:", setting);

    pl.setColor(QPalette::Window, QColor(140,178,188));
    setting->setPalette(pl);
    setting->setAutoFillBackground(true);
    setting->setGeometry(0, 0, 300,150);




    font.setPointSize(14);
    textMenu->setFont(font);
    textMenu->setStyleSheet("color: rgb(24, 24, 24);");
    //textMenu->setGeometry(10, 10, 300,25);
    textMenu->show();

}



int MainWindow::getSizeMatrix(){
    if (sizeMatrix >0 ){
        return sizeMatrix;
    }
    return 0;

}
QString* MainWindow::getValMatrix(){
    if (valMatrix != nullptr ){
        return &valMatrix;
    }
    return 0;
}

void MainWindow::closeMatrix(){
    matrixTable->close();
}
