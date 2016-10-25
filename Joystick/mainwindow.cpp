/*
* VLC-Qt Simple Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
*/



#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initialize();
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
    delete _pSocket;
}

void MainWindow::initialize()
{
    this->connectTcp();
    this->initializeJoystickValues();
    timer = new QTimer();
    timer->start(30);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStates()));
}

void MainWindow::initializeJoystickValues()
{
    joystick["x"] = 0;
    joystick["y"] = 0;
    joystick["z"] = 0;
    for (int i = 1; i <= 5; ++i) {
        joystick[QString::number(i)] = 0;
    }
}

int MainWindow::getAxisValue(QString axis) {
    int value = 0;
    if (axis == "x")
    {
        value = sf::Joystick::getAxisPosition(0,sf::Joystick::X);
    }
    else if (axis =="y")
    {
        value = sf::Joystick::getAxisPosition(0,sf::Joystick::Y);
    }
    else if (axis =="z")
    {
        value = sf::Joystick::getAxisPosition(0,sf::Joystick::Z);
    }
    return value;
}

void MainWindow::updateStates() {
    sf::Joystick::update();
    if (sf::Joystick::isConnected(0)) {
        QMap<QString, int>::const_iterator itt = joystick.constBegin();
        while (itt != joystick.constEnd()) {
            bool isNumeric;
            int key = itt.key().toInt(&isNumeric);
            if (isNumeric)
            {
                if (sf::Joystick::isButtonPressed(0, key) && !(itt.value())) {
                    joystick[itt.key()] = 1;
                    QString button = itt.key() + QString(" ") + QString::number(itt.value());
                    QByteArray button_bytes;
                    button_bytes.insert(0,button);
                    _pSocket->write( button_bytes);
                    _pSocket->waitForBytesWritten();
                }
                else if (!sf::Joystick::isButtonPressed(0, key) && itt.value()) {
                    joystick[itt.key()] = 0;
                }
            }
            else {
                int AxisValue = getAxisValue(itt.key());
                if (AxisValue != itt.value())
                {
                    joystick[itt.key()] = AxisValue;
                    QString axis = itt.key() + QString(" ") + QString::number(itt.value());
                    QByteArray axis_bytes;
                    axis_bytes.insert(0,axis);
                    _pSocket->write( axis_bytes);
                    _pSocket->waitForBytesWritten();
                }
            }
            ++itt;

        }
    }
}

void MainWindow::connectTcp()
{
    _pSocket = new QTcpSocket( this ); // <-- needs to be a member variable: QTcpSocket * _pSocket;
    connect( _pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
    _pSocket->connectToHost(QHostAddress("127.0.0.1"), 9000);
    _pSocket->waitForConnected();
    _pSocket->write(QByteArray("hello"));
}

void MainWindow::readTcpData()
{
    QByteArray data = _pSocket->readAll();
    qDebug() << data;
}
