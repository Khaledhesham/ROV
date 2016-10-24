/*
* VLC-Qt Simple Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/

#ifndef MainWindow_H_
#define MainWindow_H_

#include <QMainWindow>
#include <SFML/Window.hpp>
#include <QTimer>
#include <QDebug>
#include <QTcpSocket>
#include <QFileDialog>
#include <QInputDialog>
#include <QWidget>
#include <QHostAddress>
#include <QMap>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void initialize();
    void initializeJoystickValues();
    int  getAxisValue(QString axis);
    void connectTcp();
    ~MainWindow();

private slots:
    void updateStates();
    void readTcpData();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTcpSocket *_pSocket;
    QMap<QString, int> joystick;
    int Xprev;
    int Yprev;
    int Zprev;
};

#endif // MainWindow_H_
