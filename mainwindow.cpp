#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "port.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serialFind();
    QThread *thread_New = new QThread;
    Port *PortNew = new Port();
    PortNew->moveToThread(thread_New);
    PortNew->thisPort.moveToThread(thread_New);
    connect(PortNew, SIGNAL(error_(QString)), this, SLOT(Print(QString)));
    connect(thread_New, SIGNAL(started()), PortNew, SLOT(process_Port()));
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(quit()));
    connect(thread_New, SIGNAL(finished()), PortNew, SLOT(deleteLater()));
    connect(PortNew, SIGNAL(finished_Port()), thread_New, SLOT(deleteLater()));
    connect(this,SIGNAL(savesettings(QString)),PortNew,SLOT(Write_Settings_Port(QString)));
    connect(ui->startButton, SIGNAL(clicked()),PortNew,SLOT(ConnectPort()));
    connect(ui->stopButton, SIGNAL(clicked()),PortNew,SLOT(DisconnectPort()));
    connect(PortNew, SIGNAL(outPort(QString)), this, SLOT(filter(QString)));
    //onnect(PortNew, SIGNAL(outPort(QString)), this, SLOT(toFile(QString)));
    //connect(this,SIGNAL(writeData(QByteArray)),PortNew,SLOT(WriteToPort(QByteArray)));
    thread_New->start();
}

MainWindow::~MainWindow(){
    port->close();
    delete ui;
}

void MainWindow::serialFind(){
    QSerialPortInfo info;
    foreach (info ,QSerialPortInfo::availablePorts()) {
        ui->chooseSerial->addItem(info.portName());
        qDebug() << info.portName();
    }
}

void MainWindow::on_startButton_clicked()
{
    if(serial==true){
        savesettings(ui->chooseSerial->currentText());
        serial=false;
    }
}

void MainWindow::Print(QString  str)
{
    qDebug()<<str<<endl;
}

void MainWindow::toFile(QString state)
{
    QFile file("coords.txt");
    if(!file.exists()){
        if(file.open(QIODevice::ReadWrite|QIODevice::Text)){
            QTextStream out(&file);
            out<<state<<"\n";
        }
    }else{
        if(file.open(QIODevice::Append|QIODevice::Text)){
            QTextStream out(&file);
            out<<state<<"\n";
        }
    }
    file.close();
}

void MainWindow::filter(QString data)
{
    int g = 0;
    int gl = 0;
    if(data.contains("\n")){        
        QStringList txt_a=data.split("\n");        

        for(int i=0;i<txt_a.count()-1;i++){
            filter_txt+=txt_a[i];
            //filter_txt.clear();
            if(filter_txt.contains("$GPGSA")){
                ui->Source->setText("GPS");
            }else if(filter_txt.contains("$GNGSA")){
                ui->Source->setText("ГЛОНАСС");
            }
            if(filter_txt.contains("$GPGGA")){
                if(log == true)
                    toFile(filter_txt);
                ui->log->append(filter_txt);
                //qDebug()<<filter_txt;
                QStringList data=filter_txt.split(",");                
                if(data[6]=="0"){
                    ui->State->setText("Позиция не определена");
                    filter_txt.clear();
                }else{
                    if(data[6]=="1"){
                        ui->State->setText("Позиция определена, режм GPS SPS");
                    }else if(data[6]=="2"){
                        ui->State->setText("Позиция определена, режм DGPS, SPS");
                    }else if(data[6]=="3"){
                        ui->State->setText("Позиция определена, режм GPS PPS");
                    }
                    ui->sat->setText(data[7]);
                    QString lattitude = data[2];
                    QString longtitude = data[4];
                    QString eastWest = data[5];
                    QString nordSouth = data[3];
                    QString height = data[9];
                    ui->Lattitude->setText(lattitude);
                    ui->Longitude->setText(longtitude);
                    ui->NW->setText(nordSouth);
                    ui->EW->setText(eastWest);
                    ui->Height->setText(height);
                }
                //qDebug()<<filter_txt;
            }else if((filter_txt.contains("GNGSA"))||(filter_txt.contains("GPGSA"))){
                QStringList data=filter_txt.split(",");
                for(int i=3;i<14;i++){
                    if((data[i].toInt()<=32)&&(data[i]!="")){
                        g = g + 1;
                        ui->gps->setText(QString::number(g));
                    }else if(data[i].toInt()>=65){
                        gl = gl + 1;
                        ui->glnss->setText(QString::number(gl));
                    }
                }
                qDebug()<<filter_txt;
            }
        }
        filter_txt.clear();
    }else filter_txt+=data;

}

void MainWindow::on_logBtn_clicked()
{
    if(log == false){
        log = true;
        ui->logBtn->setText("Лог стоп");
        qDebug()<<log;
    }else{
        log = false;
        ui->logBtn->setText("Лог старт");
        qDebug()<<log;
    }

}
