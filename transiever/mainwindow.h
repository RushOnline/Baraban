#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <QSerialPort>

#include <Client>
#include <Processor>
#include <RingBuffer>
#include <MidiPort>
#include <MidiBuffer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public QtJack::Processor
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void process(int samples);

public slots:
    void note_on();
    void note_off();
    void readPort();

private:
    Ui::MainWindow*     ui;
    QTimer              timer_;
    QtJack::Client      client_;
    QtJack::MidiPort    port_;
    QtJack::MidiRingBuffer buffer_;

    QSerialPort serial;

    void setupJackClient();
};

#endif // MAINWINDOW_H
