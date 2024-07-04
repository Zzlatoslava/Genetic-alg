#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QtWidgets>
#include <vector>


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
    QLabel *textError;

    //ответ
    std::vector<int> best {4,2,3,1,5};
    std::vector<int> good1 {1,3,3,4,1};
    std::vector<int> good2 {3,1,4,2,5};

    //граф
    QGraphicsView *view;
    QGraphicsScene *scene;

    //файл
    QString fileName = nullptr;
    QLineEdit*line = nullptr;

    //матрица
    QString valMatrix = nullptr;
    int sizeMatrix;
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

    void Random();
    void getSizeRand();

    void clearSideMenu();
    void clear(QWidget* parent);

    //решение
    void SettingMenu();
    void SolutionMenu();
    void Graph();
    QPushButton* PushButtonSolution(QWidget* parent, const QString &text, int x, int y, int width, int height);
    int iteration = 0;
    QWidget *other;
    //График
    void Plot();

    //считывание настроек
    QString verData = nullptr;
    QLineEdit* verLine = nullptr;
    QPushButton* NextButton();
    void ReadVer();



    //геттеры сеттеры
    int getSizeMatrix();
    QString* getValMatrix();

    QWidget *plashca;


};
#endif // MAINWINDOW_H
