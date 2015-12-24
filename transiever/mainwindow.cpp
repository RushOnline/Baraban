#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupJackClient();
    timer.setInterval(200);
    timer.connect(&timer, SIGNAL(timeout()), this, SLOT(boom()));
    timer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::boom() {
    QtJack::MidiData note_on[]  = { 0x90, 0x1E, 0x45};
    QtJack::MidiData note_off[] = { 0x90, 0x1E, 0x00};

    _buffer.write(note_on, sizeof(note_on));
    _buffer.write(note_off, sizeof(note_off));
}

void MainWindow::setupJackClient() {
    // Connect to JACK server
    _client.connectToServer("baraban");

    // Create a 1000 seconds buffer (at 44,1kHz)
    // Register two output ports

    _outMidiPort     = _client.registerMidiOutPort("out");

    _client.setProcessor(this);

    // Activate client.
    _client.activate();

    // Connect automatically to qsynth playback.
    _client.connect(_outMidiPort, _client.portByName("qsynth:midi"));
}

void MainWindow::process(int samples) {
//    _buffer.copyTo(_outMidiPort.buffer(samples));
//    _buffer.clear();

//    _buffer.read()
//    _buffer.reset();

    int available = _buffer.numberOfElementsAvailableForRead() / 3;
    QtJack::MidiBuffer out = _outMidiPort.buffer(samples);
    int sample = 0;

    while ( available && samples ) {
        QtJack::MidiData data[3];
        _buffer.read(data, 3);
        out.writeEvent(sample++, data, 3);
        available--;
        samples--;
    }

    _buffer.reset();
}
