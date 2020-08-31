#ifndef SNAKEMAINWINDOW_H
#define SNAKEMAINWINDOW_H

#include <iostream>
#include <QMainWindow>

#include <QPainter>
#include <QKeyEvent>
#include <QRectF>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QLineEdit>
#include <QPushButton>


using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class SnakeMainWindow; }
QT_END_NAMESPACE

class SnakeMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SnakeMainWindow(QWidget *parent = nullptr);
    ~SnakeMainWindow();

private:
    Ui::SnakeMainWindow *ui;
    QList <QRectF> snake; // the snake
    QList <QRectF> fruits;
    QList <QRectF> obstacle;
    QList <QRectF> BackGround;
    QList <QPushButton> Buttons;
    QLineEdit *filepath;
    QString fileName;
    QLineEdit *bookname;

    QString savelocation;

    QPushButton *b1_start;
    QPushButton *b2_pause;
    QPushButton *b3_resume;
    QPushButton *b4_save;
    QPushButton *b5_load;
    QPushButton *b6_restart;
    QPushButton *b7_quit;

    int snakeWidth = 20;
    int snakeHeight = 20;
    int snake_add = 0;
    QTimer *timer;

    QTimer *timer2;

    int time = 100;
    int Direction = Up ;
    int time_count = 0;
    int tool_bar = 50;
    int button_bar = 100;
    int actionnum ;
    // state value
    bool gameOver = false;
    bool gameStart = false;
    bool gamePause = false;
    bool snakestrike =false;
    bool gameRestart =false;

private:
    bool snakeStrike();
    void move_Up();  // 延长长度
    void move_Down();
    void move_Left();
    void move_Right();
    enum Move{Left,Right,Up,Down};
    void deleteLastRectF(); // 删除原来位置的蛇

    void game_init();

protected:
    void paintEvent(QPaintEvent*);
    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);


protected slots:
    void timeOut();
    void fruitTimeout();
    void MenuTimeOut();
private slots:

    void on_action_Start_triggered();
    void on_actionPAUSE_triggered();
    void on_actionRESUME_triggered();
    void on_actionQUIT_triggered();
    void on_actionRESTART_triggered();
    void on_actionSaveGame_triggered();
    void on_actionLoadGame_triggered();

    void ClickButton_b1();
    void ClickButton_b2();
    void ClickButton_b3();
    void ClickButton_b4();
    void ClickButton_b5();
    void ClickButton_b6();
    void ClickButton_b7();
};
#endif // SNAKEMAINWINDOW_H
