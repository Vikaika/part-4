#include <QPainter>
#include <QTime>
#include <QtGlobal>

#include "game.h"

//описание конструктора
Snake::Snake(QWidget *parent) : QWidget(parent) {

    //чёрный цвет фона
    setStyleSheet("background-color:black;");

    //игра начата с направлением вправо
    in_game = true;
    right = true;
    left = false;
    up = false;
    down = false;

    //устанавливаем размеры виджета
    resize(Width, Height);
    Images();
    init_game();
}

//метод, который переопределяет стандартное поведение события таймера
//вызывается каждый раз, когда срабатывает таймер, связанный с объектом Snake
void Snake::timerEvent(QTimerEvent *e) {
    Q_UNUSED(e); //параметр e не используется в функци

    if (in_game) {
        check_apple();
        check();
        move();
    }
    repaint(); //перерисовка виджета
}

//метод управляет направлением движения змеи в ответ на нажатия клавиш
void Snake::keyPressEvent(QKeyEvent *e) {

    int key = e->key(); //получение кода нажатой клавиши из объекта QKeyEvent
    //нажата ли клавиша "влево" и не движется ли змея вправо
    if ((key == Qt::Key_Left) && (!right)) {
        left = true;
        up = false;
        down = false;
    }
    //нажата ли клавиша "вправо" и не движется ли змея влево
    if ((key == Qt::Key_Right) && (!left)) {
        right = true;
        up = false;
        down = false;
    }
    //нажата ли клавиша "вверх" и не движется ли змея вниз
    if ((key == Qt::Key_Up) && (!down)) {
        up = true;
        right = false;
        left = false;
    }
    //нажата ли клавиша "вниз" и не движется ли змея вверх
    if ((key == Qt::Key_Down) && (!up)) {
        down = true;
        right = false;
        left = false;
    }
    QWidget::keyPressEvent(e); //метод для обработки событий нажатия клавиш
}

//метод отвечает за отрисовку игрового поля и элементов игры
void Snake::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e); //параметр e не используется в функции
    draw();
}

//метод выполняет инициализацию параметров игры
void Snake::init_game() {

    num_dots = 3; // количество сегментов змеи=3
    for (int i = 0; i < num_dots; i++) {
        //инициализация сегментов
        x[i] = 50 - i * 10;
        y[i] = 50;
    }
    rand_apple();
    timer = startTimer(spd); // для контроля частоты обновления игры
}

//загрузка изображений
void Snake::Images() {
    blok.load("D:\\Snake\\blok.png");
    head.load("D:\\Snake\\head.png");
    apple.load("D:\\Snake\\apple.png");
}

//метод отвечает за рисование элементов игры
void Snake::draw() {
    QPainter qp(this);

    //игра продолжается:
    if (in_game) {
        //рисование изображения яблока на координатах
        qp.drawImage(apple_x, apple_y, apple);
        for (int i = 0; i < num_dots; i++) {
            if (i == 0) {
                //сли это голова змеи, то рисуется изображение головы на координатах
                qp.drawImage(x[i], y[i], head);
            }
            else {
                //сли это тело (не голова), рисуется изображение блока тела на тех же координатах
                qp.drawImage(x[i], y[i], blok);
            }
        }
    }
    else {
        game_over(qp);
    }
}

void Snake::game_over(QPainter &qp) {

    QString message = "Game over";
    QFont font("Courier", 15, QFont::DemiBold);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(message);

    qp.setPen(QColor(Qt::white));
    qp.setFont(font);
    int h = height();
    int w = width();

    qp.translate(QPoint(w/2, h/2));
    qp.drawText(-textWidth/2, 0, message);
}

//метод проверяет, съела ли змея яблоко
void Snake::check_apple() {
    //совпадают ли координаты головы змеи с координатами яблока
    //если да, то яблоко съедино
    if ((x[0] == apple_x) && (y[0] == apple_y)) {
        num_dots++;
        rand_apple();
    }
}

//метода отвечает за перемещение змеи
void Snake::move() {
    //цикл для обновления координат каждого сегмента с хвоста земли
    for (int i = num_dots; i > 0; i--) {
        x[i] = x[(i - 1)];
        y[i] = y[(i - 1)];
    }
    //перемещаем голову змеи влево
    if (left)  {x[0] -= one_dot; }
    //перемещаем голову змеи вправо
    if (right) {x[0] += one_dot; }
    //перемещаем голову змеи вверх
    if (up)    {y[0] -= one_dot; }
    //перемещаем голову змеи вниз
    if (down)  {y[0] += one_dot; }
}

//метод отвечает за проверку состояния игры
void Snake::check() {
    //цикл проходит по всем сегментам змеи с конца
    for (int i = num_dots; i > 0; i--) {
        //чтобы змея не столкнулась сама с собой
        if ((i > 4) && (x[0] == x[i]) && (y[0] == y[i])) {
            //игра окончена
            in_game = false;
        }
    }
    //вышла ли голова змеи за нижнюю границу игрового поля
    //если координата y головы больше или равна высоте игрового поля, то игра заканчивается
    if (y[0] >= Height) {in_game = false; }
    //вышла ли голова змеи за верхнюю границу игрового поля
    //если координата y головы меньше 0, то игра заканчивается
    if (y[0] < 0)       {in_game = false; }
    //вышла ли голова змеи за правую границу игрового поля
    //если координата x головы больше или равна ширине игрового поля, игра заканчивается
    if (x[0] >= Width)  {in_game = false; }
    //вышла ли голова змеи за левую границу игрового поля
    //если координата x головы меньше 0, игра заканчивается
    if (x[0] < 0)       {in_game = false; }
    //конец игры
    if(!in_game)        {killTimer(timer); }
}

//метода отвечает за генерацию случайных координат для яблока на игровом поле
void Snake::rand_apple() {
    //объект time, который получает текущее время для генерации случайного числа
    QTime time = QTime::currentTime();
    //Инициализация генератора случайных чисел
    srand((uint) time.msec());
    //число r будет использоваться для определения координат яблока
    //Установка координат х и y
    int r = rand() % position;
    apple_x = (r * one_dot);
    r = rand() % position;
    apple_y = (r * one_dot);
}
