#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QtWidgets>

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
    //QSignalMapper *signalMapper;
    QWidget *stripe;
    QLabel *textTitle;
    QWidget *menu;
    QLabel *textMenu;
    QPushButton *matrixButton;
    QPushButton *fileButton;
    QPushButton *randomButton;
    QPushButton *DaleeButtom;

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




    void createMatrix();
    void Matrix();
    void checkAllCellsFilled();
    void handleFilledMatrix();


    void File();
    void ReadLine();

    void Random();
    void getSize();

    void clearMenuExceptButtons();


};
#endif // MAINWINDOW_H