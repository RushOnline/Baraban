#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

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
    void boom();

private:
    Ui::MainWindow*     ui;
    QTimer              timer;
    QtJack::Client      _client;
    QtJack::MidiPort    _outMidiPort;
    QtJack::MidiRingBuffer _buffer;

    void setupJackClient();
};

#endif // MAINWINDOW_H
