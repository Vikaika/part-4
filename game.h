#pragma once

#include <QWidget>
#include <QKeyEvent>

class Snake : public QWidget {

public:
    //конструктор класса, принимающий указатель на родительский виджет
    Snake(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *); //метод для обработки событий перерисовки виджета
    void timerEvent(QTimerEvent *); //метод для обработки событий таймера
    void keyPressEvent(QKeyEvent *); //метода для обработки событий нажатия клавиш

private:
    bool in_game;   //работает ли игра
    int timer;      //таймер
    int num_dots;   //кол-во точек (частей) змейки

    //координаты яблока
    int apple_x;
    int apple_y;

    //изображения для отображения частей тела змейки, головы и яблока
    QImage blok;
    QImage head;
    QImage apple;

    //размеры поля
    static const int Width = 400;
    static const int Height = 400;
    static const int one_dot = 10;   //размеры яблока и частей тела змеи
    static const int max = 1600;     //максимальное количество возможных точек на доске
    static const int position = 29;  //рандомная позиция яблока
    static const int spd = 140;

    //координаты яблока
    int x[max];
    int y[max];

    //переменные для отслеживания направления движения змейки
    bool left;
    bool right;
    bool up;
    bool down;

    void init_game();          //нач условия
    void check();              //проверка столкновений
    void rand_apple();         //рандомная позиция яблока
    void check_apple();        //есть ли ещё яблоко
    void move();               //перемещения змейки в зависимости от направления
    void Images();             //загрузка избр-ий
    void draw();
    void game_over(QPainter &);
};
