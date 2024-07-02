#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QtWidgets>

#include "windows.h"

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
    QLabel *textMenu;
    QLabel *textSetting;
    QPushButton *matrixButton;
    QPushButton *fileButton;
    QPushButton *randomButton;
    QPushButton *DaleeButtom;
    bool dataFlag = false;
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

    void clearMenuExceptButtons();

    //решение
    void SettingMenu();
    void SolutionMenu();
    QPushButton* NextButton();
    //геттеры сеттеры
    int getSizeMatrix();
    QString* getValMatrix();



};
#endif // MAINWINDOW_H
