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
    //view = new QGraphicsView(graph);
    //scene = new QGraphicsScene(graph);
    //решение



    matrixButton = PushButtonMenu( 35, 80, "Матрица");
    fileButton = PushButtonMenu( 35, 160, "Из файла");
    randomButton = PushButtonMenu( 35, 240, "Случайная \nгенерация");

    SolutionMenu();



    connect(matrixButton, &QPushButton::clicked, this, &MainWindow::Matrix);
    connect(fileButton, &QPushButton::clicked, this, &MainWindow::File);
    connect(randomButton, &QPushButton::clicked, this, &MainWindow::Random);


    plashca = new QWidget(solution);
    plashca->setGeometry(450, 50, 650,550);
    plashca->setStyleSheet("background-color: rgb(244, 244, 244)");




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



    clearSideMenu();
    clear(setting);
    clear(other);
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
    connect(DaleeButtom, &QPushButton::clicked, this, &MainWindow::createMatrix);

}

void MainWindow::createMatrix()
{
    clearSideMenu();



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
    textError = new QLabel("Матрица заполнена",menu);
    textError->setStyleSheet("color: rgb(244, 244, 244);");
    font.setPointSize(12);
    textError->setFont(font);
    textError->setGeometry(10,305, 280, 50);
    textError->show();

    SettingMenu();


}

void MainWindow::File(){

    clearSideMenu();
    clear(setting);
    clear(other);
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
    if (fileName != nullptr){
    textError = new QLabel("Файл считан",menu);
    textError->setStyleSheet("color: rgb(244, 244, 244);");
    font.setPointSize(12);
    textError->setFont(font);
    textError->setGeometry(10,305, 280, 50);
    textError->show();
    SettingMenu();
    }
}


void MainWindow::Random(){
    clearSideMenu();
    clear(setting);
    clear(other);
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
    textError = new QLabel("Матрица сгенерирована",menu);
    textError->setStyleSheet("color: rgb(244, 244, 244);");
    font.setPointSize(12);
    textError->setFont(font);
    textError->setGeometry(35,305, 280, 50);
    textError->show();
    SettingMenu();

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

void MainWindow::clearSideMenu()
{

    QList<QWidget*> children = menu->findChildren<QWidget*>();

    for (QWidget *child : children) {

        if (child != matrixButton && child != fileButton && child != randomButton && child != textMenu
            ) {
            child->hide();
            child->deleteLater();
        }
    }

    fileName = nullptr;
    valMatrix = nullptr;
    sizeMatrix = 0 ;
    allFilled = false;
    verData = nullptr;
}
void MainWindow::clear(QWidget* parent)
{

    QList<QWidget*> children = parent->findChildren<QWidget*>();

    for (QWidget *child : children) {
        if (child != setting && child != graph && child != view &&child != matrixButton && child != fileButton && child != randomButton && child != textMenu ) {
            child->hide();
        }
    }
    setting->setStyleSheet("background-color: rgb(244, 244, 244)");
    plashca->show();

}


void MainWindow::SolutionMenu(){
    other = new QWidget(this);
    other->setGeometry(300, 150, 1150,600);
    solution = new QWidget(this);
    solution->setGeometry(300, 150, 1150,600);
    setting = new QWidget(solution);
    setting->setGeometry(0, 0, 300,150);

    graph = new QWidget(solution);
    graph->setGeometry(450, 50, 650,500);


}

void MainWindow::SettingMenu(){

    DaleeButtom->setEnabled(false);
    textSetting = new QLabel("Вероятность мутации:", setting);
    QLabel * percent = new QLabel("%", setting);
    verLine = new QLineEdit(setting);

    setting->setStyleSheet("background-color: rgb(140,178,188);");


    font.setPointSize(14);
    textSetting->setFont(font);
    textSetting->setStyleSheet("color: rgb(24, 24, 24);");
    textSetting->setGeometry(0,10, 220,25);
    textSetting->show();

    percent->setFont(font);
    percent->setStyleSheet("color: rgb(24, 24, 24);");
    percent->setGeometry(280,15, 25,25);
    percent->show();


    verLine->setGeometry(225, 10, 50, 30);
    verLine->setStyleSheet("color: rgb(224, 224, 224);"
                           "background-color: rgb(45, 45, 45)");


    verLine->show();
    nextButton = NextButton();
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::ReadVer);


}

//setiingMenu
QPushButton* MainWindow::NextButton(){

    QPushButton *button = new QPushButton(">>", setting);
    font.setPointSize(20);
    button->setFont(font);
    button->setStyleSheet("color: rgb(24, 24, 24);"
                          "background-color: rgb(140,178,188);");
    button->setGeometry(250,110, 50,40);
    button->show();
    return button;


}

void MainWindow::ReadVer(){
    verData = verLine->text();
    if (!verData.isEmpty()) {
        //clear(other);
        nextButton->hide();
        Graph();


    }

}

//graph

void MainWindow::Graph(){

    QLabel *answer = new QLabel("Решение:",other);
    font.setPointSize(20);
    answer->setFont(font);
    answer->setGeometry(500,25, 280, 25);
    answer->setStyleSheet("color: rgb(24, 24, 24);");
    answer->show();

    QLabel *iter = new QLabel("Итерация: "+QString::number(iteration),other);
    //setting->setStyleSheet("background-color: rgb(140,178,188);");
    font.setPointSize(20);
    iter->setFont(font);
    iter->setGeometry(20,250, 200, 25);
    iter->setStyleSheet("color: rgb(24, 24, 24);");
    iter->show();

    iteration++;
    int numVertices = best.size();
    plashca->hide();


    view = new QGraphicsView(graph);
    scene = new QGraphicsScene(graph);
    view->setGeometry(450, 50, 650, 550);
    view->setScene(scene);


    QVBoxLayout *layout = new QVBoxLayout(graph);
    layout->addWidget(view);
    setLayout(layout);



    const int radius = 30;
    const int spacing = 550/numVertices;


    for (int i = 0; i < numVertices; ++i) {

        int x1 = 50 + radius / 2;
        int y1 = 50 + i * spacing + radius / 2;

        int x2 = 250 + radius / 2;
        int y2 = 50 + (best[i]-1) * spacing + radius / 2;
        scene->addLine(x1, y1, x2, y2, QPen(QColor(33, 75, 86), 5));

    }
    for (int i = 0; i < numVertices; ++i) {

        int x1 = 50 + radius / 2;
        int y1 = 50 + i * spacing + radius / 2;

        int x2 = 250 + radius / 2;
        int y2 = 50 + (good1[i]-1) * spacing + radius / 2;
        scene->addLine(x1, y1, x2, y2, QPen(QColor(82, 122, 132), 5, Qt::DashLine));

    }
    for (int i = 0; i < numVertices; ++i) {

        int x1 = 50 + radius / 2;
        int y1 = 50 + i * spacing + radius / 2;

        int x2 = 250 + radius / 2;
        int y2 = 50 + (good2[i]-1) * spacing + radius / 2;
        scene->addLine(x1, y1, x2, y2, QPen(QColor(140, 178, 188), 5, Qt::DotLine));

    }

    // Создаем вершины множества U
    for (int i = 0; i < numVertices; ++i) {
        int x = 50;
        int y = 50 + i * spacing;

        QGraphicsEllipseItem *vertex = scene->addEllipse(x, y, radius, radius, QPen(QColor(34, 88, 101), 2), QBrush(QColor(34, 88, 101), Qt::SolidPattern));
        font.setPointSize(17);
        QGraphicsTextItem *name = scene->addText(QString::number(i + 1), font);

        name->setDefaultTextColor(QColor(224, 224, 224));
        name->setPos(x + radius / 8, y - radius / 8);
    }

    // Создаем вершины множества V
    for (int i = 0; i < numVertices; ++i) {
        int x = 250;
        int y = 50 + i * spacing;
        QGraphicsEllipseItem *vertex = scene->addEllipse(x, y, radius, radius, QPen(QColor(34, 88, 101), 2), QBrush(QColor(34, 88, 101), Qt::SolidPattern));
        QGraphicsTextItem *name = scene->addText(QString::number(i + 1), font);
        name->setDefaultTextColor(QColor(224, 224, 224));
        font.setPointSize(17);
        name->setPos(x + radius / 8, y - radius / 8);
    }
    Plot();

    QPushButton *finishButton = PushButtonSolution(other, "Перейти в конец", 10, 545, 250, 45);
    QPushButton *continueButton = PushButtonSolution(other, "Далее", 1000, 545, 90, 30);


}

void MainWindow::Plot(){

    QLabel *imageLabel = new QLabel(other);
    imageLabel->setGeometry(20, 290, 500, 250);
    QPixmap pixmap;
    pixmap.load("D:/Genetic/untitled1/images/img.png");
    pixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(pixmap);
    imageLabel->show();
}

QPushButton* MainWindow::PushButtonSolution(QWidget* parent, const QString &text, int x, int y, int width, int height){

    QPushButton *button = new QPushButton(text, parent);
    font.setPointSize(17);
    button->setFont(font);
    button->setStyleSheet("color: rgb(24, 24, 24);");
    pl.setColor(QPalette::Button, QColor(244,244,244));
    button->setPalette(pl);
    button->setGeometry(x, y, width, height);
    button->show();
    return button;
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
