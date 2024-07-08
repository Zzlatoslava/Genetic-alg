#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>

//#include <vector>
#include "alg.cpp"
#include "graph.cpp"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool choice = false ;


private:
    //общее
    Ui::MainWindow *ui;
    QPalette pl;
    QFont font = QFont("Cascadia Code");
    QWidget *stripe;
    QLabel *textTitle;
    QWidget *menu;
    QWidget *setting;
    QWidget *solution;
    QWidget *graph;
    QLabel *textMenu;
    QLabel *textSetting;
    QPushButton *matrixButton;
    QPushButton *fileButton;
    QPushButton *randomButton;
    QPushButton *DaleeButtom;
    QPushButton *nextButton;
    QLabel *textError;
    QPushButton *finishButton;
    QPushButton *continueButton;

    //ответ
    std::vector<int> best {0,1,2,3,4,5,6,7,8,9};
    std::vector<int> good1  {0,1,2,3,4,5,6,7,8,9};
    std::vector<int> good2  {0,1,2,3,4,5,6,7,8,9};

    //граф
    QGraphicsView *view;
    QGraphicsScene *scene = nullptr;
    QVBoxLayout *layout;

    //файл
    QString fileName = nullptr;
    QLineEdit*line = nullptr;

    //матрица
    int sizeMatrix = 0;
    CostMatrix costMatrix = CostMatrix(0);
    QSpinBox* countSpinBox;
    QLabel *countLabel;
    QTableWidget *matrixTable;
    bool allFilled = false;

    //общее
    void SideMenu();
    QPushButton* PushButtonMenu( int x, int y, const QString &text);
    QPushButton* DataEntryButton();



    void Matrix();
    void closeMatrix();
    void createMatrix();
    void checkAllCellsFilled();
    void handleFilledMatrix();


    void File();
    void ReadLine();
    void inputmf(std::ifstream &f,int n, int m,  QString fileName);

    void Random();
    void getSizeRand();
    void fill_random_num();

    void clearSideMenu();
    void clear(QWidget* parent);

    //решение
    void SettingMenu();
    void SolutionMenu();
    void Graph();
    void Solution();
    void NextSolution();
    void FinishSolution();
    void build_graph();



    QPushButton* PushButtonSolution(QWidget* parent, const QString &text, int x, int y, int width, int height);
    int iteration = 0;
    QWidget *other;
    //График
    void Plot();

    //считывание настроек
    int verData  = 33;
    int iterData = 10;
    int popData = 111;
    QLineEdit* verLine ;
    QLineEdit* iterLine;
    QLineEdit* popLine;
    QPushButton* NextButton();
    void ReadVer();


    void Error(int numError);
    //геттеры сеттеры
    int getSizeMatrix();
    QString* getValMatrix();

    QWidget *plashca;
    void SetSolution();
    Population *_population;

    int cost;
    void readMatrixFromFile();


};
#endif // MAINWINDOW_H
