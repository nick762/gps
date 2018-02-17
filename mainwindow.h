#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QFile>
#include <QtNetwork/QTcpSocket>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void serialFind();
    void on_startButton_clicked();
    //void on_stopButton_clicked();
    //void read(QString data);
    void Print(QString);
    void filter(QString);
    void toFile(QString);

    void on_logBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *port;
    bool serial = true;
    QString filter_txt;
    QTcpSocket *socket;
    bool log = false;

signals:
    void hasData(QList<QString> val);
    void savesettings(QString name);
    void writeData(QByteArray data);
    void error_(QString err);
};

#endif // MAINWINDOW_H
