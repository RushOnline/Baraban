#include <QDebug>

#include <QSerialPortInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupJackClient();

    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(note_on()));

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        if ("CP2104 USB to UART Bridge Controller" == info.description()) {
            serial.setPort(info);
            serial.setBaudRate(QSerialPort::Baud57600);
            serial.setDataBits(QSerialPort::Data8);
            serial.setFlowControl(QSerialPort::NoFlowControl);
            serial.setParity(QSerialPort::NoParity);
            serial.setStopBits(QSerialPort::OneStop);
            if (serial.open(QIODevice::ReadWrite)) {
                connect(&serial, SIGNAL(readyRead()), this, SLOT(readPort()));
                qDebug() << "Port successfully found and opened.";
                break;
            }
        }
    }
}

MainWindow::~MainWindow()
{
    client_.deactivate();
    client_.disconnectFromServer();
    delete ui;
}

void MainWindow::readPort() {
    QString in = serial.readLine();
    qDebug() << in;
    note_on();
}

void MainWindow::note_on() {
    QtJack::MidiData note_on[]  = { 0x99, 55, 127};
    note_on[1] = ui->spinBox->value();
    buffer_.write(note_on, sizeof(note_on));
}

void MainWindow::note_off() {
    QtJack::MidiData note_off[] = { 0x89, 35, 0x00};
    buffer_.write(note_off, sizeof(note_off));
}

void MainWindow::setupJackClient() {
    client_.connectToServer("baraban");

    port_     = client_.registerMidiOutPort("out");

    client_.setProcessor(this);

    // Activate client.
    client_.activate();

    // Connect automatically to hydrogen or qsynth midi in port.
    if (!client_.connect(port_, client_.portByName("Hydrogen:midi/capture_1")))
        client_.connect(port_, client_.portByName("qsynth:midi"));
}

void MainWindow::process(int samples) {
    int available = buffer_.numberOfElementsAvailableForRead() / 3;
    QtJack::MidiBuffer out = port_.buffer(samples);
    out.clearEventBuffer();

    int sample = 0;

    if (available && samples) {
        qDebug() << "available: " << available << "; samples: " << samples;
    }

    while ( available && samples ) {
        QtJack::MidiData data[3];
        buffer_.read(data, 3);
        qDebug("%02X %02X %02X", data[0], data[1], data[2]);
        out.writeEvent(sample, data, 3);
        sample += 3;
        available--;
        samples -= 3;
    }

    buffer_.reset();
}
