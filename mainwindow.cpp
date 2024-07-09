#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <iostream>
#include <fstream>
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


    //plashca = new QWidget(solution);
    //plashca->setGeometry(450, 50, 650,550);
    //plashca->setStyleSheet("background-color: rgb(244, 244, 244)");





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


    if (scene != nullptr){
           scene->clear();
    }
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
   // clearSideMenu();

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
    bool ok;
    sizeMatrix = matrixTable->rowCount();
    costMatrix = CostMatrix(sizeMatrix);
    for (int i = 0; i < matrixTable->rowCount(); ++i) {
        for (int j = 0; j < matrixTable->rowCount(); ++j) {
            QTableWidgetItem *item = matrixTable->item(i, j);
            if (item) {
                costMatrix.SetCost(i,j,item->text().toInt(&ok));
                if (!ok){
                    Error(2);
                    break;
                }
            }
        }


    }
    if (ok){
        textError = new QLabel("Матрица заполнена",menu);
        textError->setStyleSheet("color: rgb(244, 244, 244);");
        font.setPointSize(12);
        textError->setFont(font);
        textError->setGeometry(10,305, 280, 50);
        textError->show();
        SettingMenu();
    }



}

void MainWindow::File(){
    if (scene != nullptr){
        scene->clear();
    }
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


        //функция чтения
        readMatrixFromFile();
        SettingMenu();
    }
}



void MainWindow::Random(){
    if (scene != nullptr){
        scene->clear();
    }
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


    fill_random_num();
    SettingMenu();

}


void MainWindow::fill_random_num(){
    costMatrix = CostMatrix(sizeMatrix);
    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix; ++j) {
            costMatrix.SetCost(i,j,rand()%100);
        }
    }

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
    sizeMatrix = 0;
    fileName = nullptr;
    costMatrix = CostMatrix(sizeMatrix);
    iteration = 0;
    allFilled = false;
    choice = false;




}

void MainWindow::clear(QWidget* parent)
{

    QList<QWidget*> children = parent->findChildren<QWidget*>();

    for (QWidget *child : children) {
        if (child != setting && child != graph && child != view &&child != matrixButton && child != fileButton && child != randomButton && child != textMenu  ) {
            child->hide();
        }
    }
    setting->setStyleSheet("background-color: rgb(244, 244, 244)");
   // setting->hide();

    //plashca->show();

}


void MainWindow::SolutionMenu(){
    other = new QWidget(this);
    other->setGeometry(300, 150, 1150,600);
    solution = new QWidget(this);
    solution->setGeometry(300, 150, 1150,600);
    setting = new QWidget(solution);
    setting->setGeometry(0, 0, 310,180);

    graph = new QWidget(solution);
    graph->setGeometry(450, 50, 650,500);
    graph->setStyleSheet("background-color: rgb(140,178,188);");

    view = new QGraphicsView(graph);
    scene = new QGraphicsScene(graph);
    view->setGeometry(450, 50, 650, 550);
    view->setScene(scene);

    layout = new QVBoxLayout(graph);
    layout->addWidget(view);
    setLayout(layout);


}

void MainWindow::SettingMenu(){

    DaleeButtom->setEnabled(false);
    QLabel *textVer = new QLabel("Вероятность мутации:", setting);
    QLabel *textIter = new QLabel("Кол-во итераций:", setting);
    QLabel *textPop = new QLabel("Размер популяции:", setting);
    QLabel * percent = new QLabel("%", setting);
    verLine = new QLineEdit(setting);
    iterLine = new QLineEdit(setting);
    popLine = new QLineEdit(setting);

    setting->setStyleSheet("background-color: rgb(140,178,188);");

    font.setPointSize(14);

    textVer->setFont(font);
    textIter->setFont(font);
    textPop->setFont(font);

    textVer->setStyleSheet("color: rgb(24, 24, 24);");
    textPop->setStyleSheet("color: rgb(24, 24, 24);");
    textIter->setStyleSheet("color: rgb(24, 24, 24);");


    textIter->setGeometry(10,50, 220,25);
    textVer->setGeometry(10,10, 220,25);
    textPop->setGeometry(10,90, 220,25);

    textIter->show();
    textPop->show();
    textVer->show();

    percent->setFont(font);
    percent->setStyleSheet("color: rgb(24, 24, 24);");
    percent->setGeometry(290,15, 25,25);
    percent->show();


    verLine->setGeometry(235, 10, 50, 30);
    verLine->setStyleSheet("color: rgb(224, 224, 224);"
                           "background-color: rgb(45, 45, 45)");


    iterLine->setGeometry(235, 50, 50, 30);
    iterLine->setStyleSheet("color: rgb(224, 224, 224);"
                            "background-color: rgb(45, 45, 45)");

    popLine->setGeometry(235, 90, 50, 30);
    popLine->setStyleSheet("color: rgb(224, 224, 224);"
                           "background-color: rgb(45, 45, 45)");
    verLine->show();
    iterLine->show();
    popLine->show();


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
    button->setGeometry(260,140, 50,40);
    button->show();




    return button;


}

void MainWindow::ReadVer(){


    bool ok1  = true;
    bool ok2  = true;
    bool ok3  = true;


    //verData = verLine->text().toInt(&ok1);
    //iterData = iterLine->text().toInt(&ok2);
    //popData = popLine->text().toInt(&ok3);

    if (!ok1  || !ok2 || !ok3 || verData < 0 || iterData < 0|| popData < 0 ){
        Error(1);
    }
    else{
        nextButton->hide();
        SetSolution();
        Solution();
        Graph();

    }



}

//graph



void MainWindow::Graph(){
   // clear(other);

    //scene->clear();



   scene->clear();




    QLabel *answer = new QLabel("Решение:",other);
    font.setPointSize(20);
    answer->setFont(font);
    answer->setGeometry(500,25, 280, 25);
    answer->setStyleSheet("color: rgb(24, 24, 24);");
    answer->show();

    int k = 0;
    for (int it = 0 ; it< best.size() ; ++it){

        QLabel *qanswer = new QLabel(QString::number(best[it]+1) ,other);
        font.setPointSize(20);
        qanswer->setFont(font);
        qanswer->setGeometry(670+k,25, 280, 25);
        qanswer->setStyleSheet("color: rgb(24, 24, 24);");
        qanswer->show();
        k+= 15;
    }
    k+= 15;
    for (int it = 0 ; it< best.size() ; ++it){

        QLabel *qanswer = new QLabel(QString::number(good1[it]+1) ,other);
        font.setPointSize(20);
        qanswer->setFont(font);
        qanswer->setGeometry(670+k,25, 280, 25);
        qanswer->setStyleSheet("color: rgb(24, 24, 24);");
        qanswer->show();
        k+= 15;
    }

    k+= 15;
    for (int it = 0 ; it< best.size() ; ++it){

        QLabel *qanswer = new QLabel(QString::number(good2[it]+1) ,other);
        font.setPointSize(20);
        qanswer->setFont(font);
        qanswer->setGeometry(670+k,25, 280, 25);
        qanswer->setStyleSheet("color: rgb(24, 24, 24);");
        qanswer->show();
        k+= 15;
    }



    QLabel *iter = new QLabel("Итерация: "+QString::number(iteration),other);
    setting->setStyleSheet("background-color: rgb(140,178,188);");
    font.setPointSize(20);
    iter->setFont(font);
    iter->setGeometry(20,240, 200, 25);
    iter->setStyleSheet("color: rgb(24, 24, 24);");
    iter->show();

    //iteration++;
    int numVertices = best.size();

    //plashca->hide();



    //view->show();
    //graph->show();







    finishButton = PushButtonSolution(solution, "Перейти в конец", 10, 545, 250, 45);
    continueButton = PushButtonSolution(solution, "Далее", 1000, 555, 90, 40);


    const int radius = 30;
    const int spacing = (490)/numVertices;


    for (int i = 0; i < numVertices; ++i) {

        int x1 = 50 + radius / 2;
        int y1 = 50 + i * spacing + radius / 2;

        int x2 = 250 + radius / 2;
        int y2 = 50 + (best[i]) * spacing + radius / 2;
        scene->addLine(x1, y1, x2, y2, QPen(QColor(33, 75, 86), 5));

    }
    for (int i = 0; i < numVertices; ++i) {

        int x1 = 50 + radius / 2;
        int y1 = 50 + i * spacing + radius / 2;

        int x2 = 250 + radius / 2;
        int y2 = 50 + (good1[i]) * spacing + radius / 2;
        scene->addLine(x1, y1, x2, y2, QPen(QColor(62, 152, 34), 5, Qt::DashLine));

    }
    for (int i = 0; i < numVertices; ++i) {

        int x1 = 50 + radius / 2;
        int y1 = 50 + i * spacing + radius / 2;

        int x2 = 250 + radius / 2;
        int y2 = 50 + (good2[i]) * spacing + radius / 2;
        scene->addLine(x1, y1, x2, y2, QPen(QColor(234, 144, 42), 5, Qt::DotLine));

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




    //Plot();
    build_graph();
    connect(finishButton, &QPushButton::clicked, this, &MainWindow::FinishSolution);
    connect(continueButton, &QPushButton::clicked, this, &MainWindow::NextSolution);







}



QPushButton* MainWindow::PushButtonSolution(QWidget* parent, const QString &text, int x, int y, int width, int height){

    QPushButton *button = new QPushButton(text, parent);
    font.setPointSize(17);
    button->setFont(font);
    button->setStyleSheet("color: rgb(24, 24, 24);");
    pl.setColor(QPalette::Button, QColor(34, 88, 101));
    button->setPalette(pl);
    button->setGeometry(x, y, width, height);
    button->show();
    return button;
}

void MainWindow::NextSolution(){
    textError = new QLabel("Работает",setting);
    textError->setStyleSheet("color: rgb(24, 24, 24);");
    textError->setGeometry(10,120, 280, 50);
    textError->show();
    if(iteration<=iterData){
        Solution();
        Graph();
    }
    else{
        continueButton->hide();
    }

}

void MainWindow::FinishSolution(){
    textError = new QLabel("Работает",setting);
    textError->setStyleSheet("color: rgb(224, 224, 224);");
    textError->setGeometry(10,120, 280, 50);
    textError->show();

    while(iteration <= iterData){
        Solution();
    }

    Graph();

}

QString* MainWindow::getValMatrix(){
    //  if (matrix.GetCost(i,j)!=nullptr ){
    // return matrix;
    //}
    return 0;
}

void MainWindow::closeMatrix(){
    matrixTable->close();
}





void MainWindow::SetSolution(){

    bool maximise= false ;

    _population = new Population(popData,sizeMatrix,maximise);
    //_population->Evaluate(matrix,best,good1,good2);

    //CostMatrix costMatrix(taskSize);

    // Инициализация популяции
  //  Population population(populationSize, taskSize, maximise);
}
void MainWindow::Solution(){
    clear(other);




        //best.clear();
        //good1.clear();
        //good2.clear();
        //_population->Evaluate(costMatrix,sizeMatrix,  best, good1, good2);

        // Сохраняем лучшую хромосому
        //_population->StoreBestSolution(sizeMatrix);

        // Применяем операторы генетического алгоритма
       // _population->ApplyCrossover(sizeMatrix);
        //_population->Mutate(verData);
        //_population->SeedBestSolution();

        // Проводим селекцию
       // _population->Selection();


    cost = _population->Evaluate(costMatrix,best, good1, good2);
        _population->StoreBestSolution(sizeMatrix);
        _population->Mutate(verData);
        _population->ApplyCrossover(sizeMatrix);
        _population->SeedBestSolution(sizeMatrix);

        _population->Selection();
        iteration++;





}


void MainWindow::readMatrixFromFile() {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Error(3);
        clearSideMenu();
        return;
    }

    QTextStream in(&file);
    std::vector<std::vector<int>> tempMatrix; // Временная матрица для чтения данных
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList values = line.split(' ');
        std::vector<int> row;
        for (const QString &value : values) {
            bool ok;
            int num = value.toInt(&ok);
            if (!ok) {
                Error(4);
                return;
            }
            row.push_back(num);
        }
        tempMatrix.push_back(row);
    }

    file.close();

    sizeMatrix = tempMatrix.size();
    costMatrix = CostMatrix(sizeMatrix);

    for (int i = 0; i < sizeMatrix; ++i) {
        for (int j = 0; j < sizeMatrix; ++j) {
            costMatrix.SetCost(i, j, tempMatrix[i][j]);
        }
    }



}


void MainWindow::Error(int numError){
    QWidget *newWindow = new QWidget();
    QLabel * error = new QLabel(newWindow);
    newWindow->setGeometry(500,250, 300,200);

    font.setPointSize(15);
    error->setFont(font);
    QVBoxLayout *layout = new QVBoxLayout(newWindow);
    layout->addWidget(error);
    //setLayout(layout);
    switch(numError){
    case 1:
        error->setText("Введено неверное значение.\nПопробуйте снова");
        break;
    case 2:
        error->setText("Введено нечисловое значение.\nПопробуйте снова");
        break;
    case 3:
        error->setText("Неудалось открыть файл");
        break;
    case 4:
        error->setText("Некорректное значение в файле");
        break;
    }
    error->show();
    newWindow->show();
}


void MainWindow:: build_graph(){
    // Create chart view and set its geometry
    for (int i = 0; i < 5; ++i)
    {
        // Создаём представление графика
        QChartView *chartView = new QChartView(other);
        chartView->setGeometry(20, 280, 500, 250);
        // Добавляем его в горизонтальный Layout
       // ui->horizontalLayout->addWidget(chartView);
        // Создаём
        int seed = cost;
        QLineSeries *series = new QLineSeries();
        int k = 0;
        while (k <= 100)
        {
            *series << QPointF(k, seed);
            ++k;
        }

        // Создаём график и добавляем в него функцию
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->legend()->hide();
        chart->setTitle("Graphic");

        // Добавим всплывающую подсказку для графика
        chart->setToolTip(QString("График №%1\n"
                                  "Количество отсчётов %2").arg(i + 1).arg(k));

        // Настройка осей графика
        QValueAxis *axisX = new QValueAxis();
        axisX->setTitleText("x, м");
        axisX->setLabelFormat("%i");
        axisX->setTickCount(1);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("t, мс");
        axisY->setLabelFormat("%g");
        axisY->setTickCount(5);
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        // Устанавливаем график в представление
        chartView->setChart(chart);
        chartView->show();
    }
}


void MainWindow::Plot(){

    QLabel *imageLabel = new QLabel(other);
    imageLabel->setGeometry(20, 280, 500, 250);
    QPixmap pixmap;
    pixmap.load("D:/Genetic/untitled1/images/img.png");
    pixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    imageLabel->setPixmap(pixmap);
    imageLabel->show();
}
