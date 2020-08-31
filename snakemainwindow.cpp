#include "snakemainwindow.h"
#include "ui_snakemainwindow.h"

SnakeMainWindow::SnakeMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SnakeMainWindow)
{
    ui->setupUi(this);
    game_init();
}

SnakeMainWindow::~SnakeMainWindow()
{
    delete ui;
}


void SnakeMainWindow::game_init()
{
    resize(800 + button_bar,800 + tool_bar);
    //setStyleSheet("QWidget{background:white}");

    //button enable set

    this->menuBar()->actions().at(0)->menu()->actions().at(0)->setEnabled(false);  //save
    this->menuBar()->actions().at(0)->menu()->actions().at(1)->setEnabled(true);  //load
    this->menuBar()->actions().at(1)->menu()->actions().at(0)->setEnabled(true);  //Start
    this->menuBar()->actions().at(1)->menu()->actions().at(1)->setEnabled(false);  //Pause
    this->menuBar()->actions().at(1)->menu()->actions().at(2)->setEnabled(false);  //Resume
    this->menuBar()->actions().at(1)->menu()->actions().at(3)->setEnabled(false);  //Restart

    //
    b1_start=new QPushButton("START",this);
    b2_pause=new QPushButton("PAUSE",this);
    b3_resume=new QPushButton("RESUME",this);
    b4_save=new QPushButton("SAVE",this);
    b5_load=new QPushButton("LOAD",this);
    b6_restart=new QPushButton("RESTART",this);
    b7_quit=new QPushButton("QUIT",this);


    b1_start->move(800,tool_bar );
    b1_start->show();
    b2_pause->move(800,tool_bar + 40 );
    b2_pause->show();
    b3_resume->move(800,tool_bar + 40*2);
    b3_resume->show();
    b4_save->move(800,tool_bar + 40*3);
    b4_save->show();
    b5_load->move(800,tool_bar + 40*4);
    b5_load->show();
    b6_restart->move(800,tool_bar + 40*5);
    b6_restart->show();
    b7_quit->move(800,tool_bar + 40*6);
    b7_quit->show();

    b1_start->setFocusPolicy(Qt::NoFocus);
    b2_pause->setFocusPolicy(Qt::NoFocus);
    b3_resume->setFocusPolicy(Qt::NoFocus);
    b4_save->setFocusPolicy(Qt::NoFocus);
    b5_load->setFocusPolicy(Qt::NoFocus);
    b6_restart->setFocusPolicy(Qt::NoFocus);
    b7_quit->setFocusPolicy(Qt::NoFocus);

   connect(b1_start,SIGNAL(clicked()),this,SLOT(ClickButton_b1()));
   connect(b2_pause,SIGNAL(clicked()),this,SLOT(ClickButton_b2()));
   connect(b3_resume,SIGNAL(clicked()),this,SLOT(ClickButton_b3()));
   connect(b4_save,SIGNAL(clicked()),this,SLOT(ClickButton_b4()));
   connect(b5_load,SIGNAL(clicked()),this,SLOT(ClickButton_b5()));
   connect(b6_restart,SIGNAL(clicked()),this,SLOT(ClickButton_b6()));
   connect(b7_quit,SIGNAL(clicked()),this,SLOT(ClickButton_b7()));




   b4_save->setEnabled(false);  //save
   b5_load->setEnabled(true);  //load
   b1_start->setEnabled(true);  //Start
   b2_pause->setEnabled(false);  //Pause
   b3_resume->setEnabled(false);  //Resume
   b6_restart->setEnabled(false);  //Restart


    for(int i = 0 ; i < 40 ; i++)
        for(int j = 0 ; j < 40; j++)
        {
            BackGround.append(QRectF(20*j,20*i + tool_bar,20,20));
        }


    snake.append(QRectF(380,400 + tool_bar,snakeWidth,snakeHeight));
    move_Up();



    timer = new QTimer;
    timer2 = new QTimer;

    connect(timer, SIGNAL(timeout()),this,SLOT(timeOut()));
    connect(timer, SIGNAL(timeout()),this,SLOT(fruitTimeout()));

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));


    connect(timer2, SIGNAL(timeout()),this,SLOT(MenuTimeOut()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(update()));

}

void SnakeMainWindow::timeOut()
{

    int flag = 1;

    for(int i=0; i<fruits.length(); i++){
        if(fruits.at(i).contains(snake.at(0).topLeft()+QPointF(snakeWidth/2,snakeHeight/2))){
            //if(snake.at(0).contains(fruits.at(i).x()+ fruits.at(i).width()/2, fruits.at(i).y()+ fruits.at(i).height()/2)){
            if(fruits.at(i).width()>snakeWidth){//额外奖励
                flag += 2;
            }
            snake_add += 3;//正常奖励
            fruits.removeAt(i);
            break;
        }
    }
    while(flag--)
    {
        switch (Direction) {
        case Up:
            move_Up();
            break;
        case Down:
            move_Down();
            break;
        case Right:
            move_Right();
            break;
        case Left:
            move_Left();
            break;
        default:
            break;
        }


    }
    if(snake_add == 0 && !gameOver)
        deleteLastRectF();
    else snake_add --;

}



void SnakeMainWindow::MenuTimeOut()
{
    if(gameStart)
    {
        this->menuBar()->actions().at(0)->menu()->actions().at(0)->setEnabled(false);  //save
        this->menuBar()->actions().at(0)->menu()->actions().at(1)->setEnabled(false);  //load
        this->menuBar()->actions().at(1)->menu()->actions().at(0)->setEnabled(false);  //Start
        this->menuBar()->actions().at(1)->menu()->actions().at(1)->setEnabled(true);  //Pause
        this->menuBar()->actions().at(1)->menu()->actions().at(2)->setEnabled(false);  //Resume
        this->menuBar()->actions().at(1)->menu()->actions().at(3)->setEnabled(false);  //Restart

        b4_save->setEnabled(false);  //save
        b5_load->setEnabled(false);  //load
        b1_start->setEnabled(false);  //Start
        b2_pause->setEnabled(true);  //Pause
        b3_resume->setEnabled(false);  //Resume
        b6_restart->setEnabled(false);  //Restart


    }
    else if(gamePause)
    {
        this->menuBar()->actions().at(0)->menu()->actions().at(0)->setEnabled(true);  //save
        this->menuBar()->actions().at(0)->menu()->actions().at(1)->setEnabled(false);  //load
        this->menuBar()->actions().at(1)->menu()->actions().at(0)->setEnabled(false);  //Start
        this->menuBar()->actions().at(1)->menu()->actions().at(1)->setEnabled(false);  //Pause
        this->menuBar()->actions().at(1)->menu()->actions().at(2)->setEnabled(true);  //Resume
        this->menuBar()->actions().at(1)->menu()->actions().at(3)->setEnabled(true);  //Restart

        b4_save->setEnabled(true);  //save
        b5_load->setEnabled(false);  //load
        b1_start->setEnabled(false);  //Start
        b2_pause->setEnabled(false);  //Pause
        b3_resume->setEnabled(true);  //Resume
        b6_restart->setEnabled(true);  //Restart

    }
    else if(gameOver)
    {
        this->menuBar()->actions().at(0)->menu()->actions().at(0)->setEnabled(true);  //save
        this->menuBar()->actions().at(0)->menu()->actions().at(1)->setEnabled(false);  //load
        this->menuBar()->actions().at(1)->menu()->actions().at(0)->setEnabled(false);  //Start
        this->menuBar()->actions().at(1)->menu()->actions().at(1)->setEnabled(false);  //Pause
        this->menuBar()->actions().at(1)->menu()->actions().at(2)->setEnabled(false);  //Resume
        this->menuBar()->actions().at(1)->menu()->actions().at(3)->setEnabled(true);  //Restart

        b4_save->setEnabled(true);  //save
        b5_load->setEnabled(false);  //load
        b1_start->setEnabled(false);  //Start
        b2_pause->setEnabled(false);  //Pause
        b3_resume->setEnabled(false);  //Resume
        b6_restart->setEnabled(true);  //Restart
        timer->stop();

    }
    else if(gameRestart)
    {

    }

}

void SnakeMainWindow::fruitTimeout()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(fruits.length() < 1){
        int inornot = true;
        int x = qrand()%(800/20)*20;
        int y = qrand()%(800/20)*20 + tool_bar;
        for(int i = 0 ; i < snake.length() ; i++){
            if(snake.at(i).contains(QRectF(x,y,20,20))) inornot = false;
        }
        for(int i = 0 ; i < obstacle.length() ; i++){
            if(obstacle.at(i).contains(QRectF(x,y,20,20))) inornot = false;
        }

        if(inornot)
            fruits.append(QRectF(x,y,snakeWidth,snakeWidth));

        /*      if(qrand()%5 == 3){
           fruits.append(QRectF(qrand()%(this->width()/20)*20-5,qrand()%(this->height()/20)*20-5,snakeWidth*2,snakeWidth*2));
        }
*/
    }
}

void SnakeMainWindow::move_Up()
{
    if(snake.at(0).y()-snakeHeight < tool_bar){       //碰到边界
        gameOver = true;


    }else{
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeHeight),snake.at(0).topRight()));
    }
}

void SnakeMainWindow::move_Down()
{
    if(snake.at(0).y() + snakeHeight * 2  > this->height()){       //碰到边界
        gameOver = true;

    }else{
        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeHeight)));
    }
}
void SnakeMainWindow::move_Right()
{
    if(snake.at(0).x() + snakeWidth * 2  > this->width()){       //碰到边界

        gameOver = true;
    }else{
        snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeWidth,0)));
    }
}
void SnakeMainWindow::move_Left()
{
    if(snake.at(0).x() - snakeWidth   < 0){       //碰到边界
        gameOver = true;

    }else{
        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeWidth,0),snake.at(0).bottomLeft()));
    }
}
void SnakeMainWindow::deleteLastRectF()
{
    time_count++;
    snake.removeLast();
}

void SnakeMainWindow::paintEvent(QPaintEvent * ev)
{



    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font("微软雅黑",20,QFont::ExtraLight,false);

    painter.setRenderHint(QPainter::Antialiasing);

    pen.setWidth(1);
    pen.setColor(Qt::white);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::lightGray);

    painter.setPen(pen);
    painter.setBrush(brush);

    for(int i=0; i<BackGround.length(); i++){
        painter.drawRect(BackGround.at(i));
    }



    brush.setColor(Qt::darkGray);
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.setBrush(brush);

    for(int i=0; i<snake.length(); i++){
        painter.drawRect(snake.at(i));
        brush.setColor(Qt::lightGray);
        painter.setBrush(brush);
    }




    brush.setColor(Qt::red);
    painter.setBrush(brush);
    for(int i=0; i<fruits.length(); i++){
        painter.drawEllipse(fruits.at(i));
    }

    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(20,30+tool_bar,QString("TIME:")+QString("%1").arg(time_count));

    if(snakeStrike() || gameOver == true){

        QFont font("微软雅黑",30,QFont::Bold,false);
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2 + tool_bar,QString("GAME OVER!"));
        gameStart = false;

    }

    brush.setColor(Qt::green);
    painter.setBrush(brush);
    for(int i = 0 ; i < obstacle.length(); i++) painter.drawRect(obstacle.at(i));


    QWidget::paintEvent(ev);

}

void SnakeMainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Up:
        if(Direction != Down){
            Direction = Up;
        }
        break;
    case Qt::Key_Down:
        if( Direction != Up){
            Direction = Down;
        }
        break;
    case Qt::Key_Right:
        if(Direction != Left){
            Direction = Right;
        }
        break;
    case Qt::Key_Left:
        if(Direction != Right){
            Direction = Left;
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        break;
    case Qt::Key_Space:
        break;
    default:
        break;
    }
}

bool SnakeMainWindow::snakeStrike()
{
    for(int i=0; i<snake.length(); i++){
        for(int j=i+1; j<snake.length(); j++){
            if(snake.at(i) == snake.at(j)){
                gameOver = true;
                return true;
            }
        }
    }
    for(int i = 0 ; i < obstacle.length() ; i++)
        if(obstacle.at(i).contains(snake.at(0).topLeft()+QPointF(snakeWidth/2,snakeHeight/2)))
        {
            gameOver = true;
            return true;
        }
    return false;

}

void SnakeMainWindow::mousePressEvent(QMouseEvent *event)
{
    if(!gameStart)
    {
        int x = event->x();
        int y = event->y();
        if(event->button() == Qt::LeftButton)
        {

            for(int i = 0 ; i < 40 ; i++)
                for(int j = 0 ; j < 40; j++)
                {
                    if(QRectF(20*j,20*i + tool_bar,20,20).contains(x,y))
                    {
                        if(obstacle.contains(QRectF(20*j,20*i+tool_bar,20,20)))
                        {
                            obstacle.removeOne(QRectF(20*j,20*i+tool_bar,20,20));
                        }
                        else
                            obstacle.append(QRectF(20*j,20*i+tool_bar,20,20));

                    }
                }
        }
        update();
    }
}


void SnakeMainWindow::on_action_Start_triggered()
{
    gameStart = true;
    timer->start(100);
    timer2->start(100);

}

void SnakeMainWindow::on_actionPAUSE_triggered()
{
    timer->stop();
    gamePause = true;
    gameStart = false;
}

void SnakeMainWindow::on_actionRESUME_triggered()
{
    timer->start(100);

    gamePause = false;
    gameStart = true;
}

void SnakeMainWindow::on_actionQUIT_triggered()
{
    this->close();
}

void SnakeMainWindow::on_actionRESTART_triggered()
{
    snake_add = 0;
    gameOver = false;
    time_count = 0;
    gameStart = false;
    gamePause = false;
    gameRestart = true;
    snake.clear();
    fruits.clear();
    obstacle.clear();
    game_init();
}


void SnakeMainWindow::on_actionSaveGame_triggered()
{
    savelocation = QFileDialog::getSaveFileName(this,tr("SAVE GAME FILE"),tr("snakeprogress.txt"),tr("TEXT (*.txt)"));

    QFile *savefile = new QFile(savelocation);

    savefile->open(QIODevice::WriteOnly | QIODevice::Text );
    QTextStream file_writeLine(savefile);
    quint16 num = snake.size();
    quint16 num2 = fruits.size();
    quint16 num3 = obstacle.size();

    for(int i=0;i < num ; i++)
    {
        file_writeLine<<QString::number(snake.at(i).x()) <<"\n" << QString::number(snake.at(i).y()) << "\n";

    }
    file_writeLine<< -1111 << "\n";
    for(int i=0;i < num2 ; i++)
    {
        file_writeLine<<QString::number(fruits.at(i).x()) <<"\n" << QString::number(fruits.at(i).y()) << "\n";

    }
    file_writeLine<< -2222 << "\n";

    for(int i=0;i < num3 ; i++)
    {
        file_writeLine<<QString::number(obstacle.at(i).x()) <<"\n" << QString::number(obstacle.at(i).y()) << "\n";

    }
    file_writeLine<< -3333 << "\n";

    file_writeLine << QString::number(time_count)<<"\n";
    file_writeLine<< -4444 << "\n";

    file_writeLine << QString::number(Direction)<<"\n";
    file_writeLine<< -5555 << "\n";


    savefile->close();

}

void SnakeMainWindow::on_actionLoadGame_triggered()
{
    fileName =QFileDialog::getOpenFileName(this,tr("打开文件"));
    QFile *Load  = new QFile(fileName);

    bool isOk = Load->open(QIODevice::ReadOnly);
    snake.clear();
    int x;
    int y;
    if(isOk){
        QTextStream file_readLine(Load);


        while(1)
        {
            file_readLine >> x;
            if(x == -1111)break;
            file_readLine >> y;
            snake.append(QRectF(x,y,snakeWidth,snakeHeight));
        }
        while(1)
        {
            file_readLine >> x;
            if(x == -2222)break;
            file_readLine >> y;
            fruits.append(QRectF(x,y,snakeWidth,snakeHeight));
        }
        while(1)
        {
            file_readLine >> x;
            if(x == -3333)break;
            file_readLine >> y;
            obstacle.append(QRectF(x,y,snakeWidth,snakeHeight));
        }
        while(1)
        {
            file_readLine >> x;
            if(x == -4444)break;
            time_count = x;

        }

        while(1)
        {
            file_readLine >> x;
            if(x == -5555)break;
            Direction = x;

        }

    }





    Load->close();

}
void SnakeMainWindow::ClickButton_b1()
{
    gameStart = true;
    timer->start(100);
    timer2->start(100);
}
void SnakeMainWindow::ClickButton_b2()
{
    timer->stop();
    gamePause = true;
    gameStart = false;
}
void SnakeMainWindow::ClickButton_b3()
{
    timer->start(100);

    gamePause = false;
    gameStart = true;
}
void SnakeMainWindow::ClickButton_b4()
{
    savelocation = QFileDialog::getSaveFileName(this,tr("SAVE GAME FILE"),tr("snakeprogress.txt"),tr("TEXT (*.txt)"));

    QFile *savefile = new QFile(savelocation);

    savefile->open(QIODevice::WriteOnly | QIODevice::Text );
    QTextStream file_writeLine(savefile);
    quint16 num = snake.size();
    quint16 num2 = fruits.size();
    quint16 num3 = obstacle.size();

    for(int i=0;i < num ; i++)
    {
        file_writeLine<<QString::number(snake.at(i).x()) <<"\n" << QString::number(snake.at(i).y()) << "\n";

    }
    file_writeLine<< -1111 << "\n";
    for(int i=0;i < num2 ; i++)
    {
        file_writeLine<<QString::number(fruits.at(i).x()) <<"\n" << QString::number(fruits.at(i).y()) << "\n";

    }
    file_writeLine<< -2222 << "\n";

    for(int i=0;i < num3 ; i++)
    {
        file_writeLine<<QString::number(obstacle.at(i).x()) <<"\n" << QString::number(obstacle.at(i).y()) << "\n";

    }
    file_writeLine<< -3333 << "\n";

    file_writeLine << QString::number(time_count)<<"\n";
    file_writeLine<< -4444 << "\n";

    file_writeLine << QString::number(Direction)<<"\n";
    file_writeLine<< -5555 << "\n";


    savefile->close();
}

void SnakeMainWindow::ClickButton_b5()
{
    fileName =QFileDialog::getOpenFileName(this,tr("打开文件"));
    QFile *Load  = new QFile(fileName);

    bool isOk = Load->open(QIODevice::ReadOnly);
    snake.clear();
    int x;
    int y;
    if(isOk){
        QTextStream file_readLine(Load);


        while(1)
        {
            file_readLine >> x;
            if(x == -1111)break;
            file_readLine >> y;
            snake.append(QRectF(x,y,snakeWidth,snakeHeight));
        }
        while(1)
        {
            file_readLine >> x;
            if(x == -2222)break;
            file_readLine >> y;
            fruits.append(QRectF(x,y,snakeWidth,snakeHeight));
        }
        while(1)
        {
            file_readLine >> x;
            if(x == -3333)break;
            file_readLine >> y;
            obstacle.append(QRectF(x,y,snakeWidth,snakeHeight));
        }
        while(1)
        {
            file_readLine >> x;
            if(x == -4444)break;
            time_count = x;

        }

        while(1)
        {
            file_readLine >> x;
            if(x == -5555)break;
            Direction = x;

        }

    }





    Load->close();
}
void SnakeMainWindow::ClickButton_b6()
{
    snake_add = 0;
    gameOver = false;
    time_count = 0;
    gameStart = false;
    gamePause = false;
    gameRestart = true;
    snake.clear();
    fruits.clear();
    obstacle.clear();
    game_init();
}
void SnakeMainWindow::ClickButton_b7()
{
     this->close();
}
