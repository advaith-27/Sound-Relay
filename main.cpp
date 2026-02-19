#include "mainwindow.h"

#include <QApplication>
#include <QFrame>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QPushButton>
#include <QLocale>
#include <QTranslator>
#include <iostream>
// #include <windows.h>
#include <QTimer>
#include <QAudioInput>
#include <QAudioSource>
#include <QAudioSink>
#include <QComboBox>
#include <QAudioOutput>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QAudioFormat>
#include <QBuffer>
#include <QLabel>

#include <QPoint>

#include <cmath>
#include<QDebug>
#include <QtCore>
#include <QCryptographicHash>
#include <QInputDialog>
#include <QVBoxLayout>


QFrame* connect_frame;
QUdpSocket* lis;
QFrame* f1;
QVBoxLayout* l2;
QFrame* f2;
QVBoxLayout* l3;
QFrame* f3;
QVBoxLayout* l4;
QFrame* f4;
QVBoxLayout* l5;
QString nam="";
MainWindow* www;
QHash<QString, QList<QString>> devices;
QString num="";
QLabel *Ch_label; //channel label
QAudioSource *audioInput;
QList<QHostAddress> to_send;
int towrite=0,toread=0;
QByteArray ruff[20];
QAudioFormat format;
int con_chan=0,send_chan=0;
bool connected=false;
bool mic=false;
bool broadcast =true;
void func(QString name, QString key,QHostAddress ip){
    // qCritical("(currentThread()== QCoreApplication::instance()->thread())");
    if(!devices.contains(name)){

        QList<QString> a;
        a.append(key);
        a.append(ip.toString());
        QPushButton *b=new QPushButton(name,f1);
        QString temp =b->text();
        devices[name]=a;
        QObject::connect(b,&QPushButton::clicked,b,[temp](){
            bool ok;
            QString text = QInputDialog::getText(www, ("Input Dialog"),
                                                 ("Enter your password:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok);
            if (ok && !text.isEmpty()) {
                QUdpSocket udpSocket;
                QByteArray datagram = "Glove Thingi auth Pass:";
                datagram +=QString(QCryptographicHash::hash((text+devices[temp][0]).toUtf8(), QCryptographicHash::Md5).toHex()).toUtf8();
                QHostAddress destinationAddress(devices[temp][1]);
                quint16 destinationPort = 5555;
                nam=temp;
                qCritical( temp.toUtf8());
                udpSocket.writeDatagram(datagram.data(), datagram.size(), destinationAddress, destinationPort);
            }
        });
        f1->layout()->addWidget(b);
        b->show();
    }
}

class Worker : public QObject
{

public:
    Worker(){
        // qCritical( "UDP");

    }
public slots:
    void runinThread(){
        QThread* t = QThread::create([this]{
            something();
            QEventLoop loop;
            loop.exec();
        });
        t->start();
    }
    void something(){
        if(f3->isVisible()){
            // qCritical((QThread::currentThread()== QCoreApplication::instance()->thread())?"1":"0");
            // QBuffer rwBuff;
            // rwBuff.open(QBuffer::ReadWrite);
            QAudioDevice loopbackDevice;
            for (const QAudioDevice &device : QMediaDevices::audioInputs()) {

                qDebug() << "Device:" << device.description()
                << "| ID:" << device.id();
                if (device.description().contains("AudioRelay", Qt::CaseInsensitive) ||
                    device.description().contains("monitor", Qt::CaseInsensitive)|| device.description().contains("Stereo Mix")) {
                    qCritical("...");
                    loopbackDevice = device;
                    break;
                }
            }
            qCritical(loopbackDevice.description().toUtf8());
            qCritical(loopbackDevice.id().toBase64());
            // QAudioFormat format;
            // format.setSampleRate(96000);
            // format.setChannelCount(2);
            // format.setSampleFormat(QAudioFormat::Int16);
            if(mic){
                audioInput= new QAudioSource(QMediaDevices::defaultAudioInput(),format);
            }
            else{
                audioInput = new QAudioSource(loopbackDevice, format);
            }
            // audioInput.setDevice(QMediaDevices::defaultAudioInput());
            audioInput->setBufferSize(4096);
            QIODevice *io=audioInput->start();
            QHostAddress destinationAddress(devices[nam][1]);
            // QHostAddress multicastAddress("10.45.1.230");//("239.134.233.10");
            QUdpSocket udpSocket;
            quint16 destinationPort = 5555;
            //int jit_num=0;
            QByteArray datagram="Glove Thingi send";
                udpSocket.writeDatagram(datagram.data(),datagram.size(),destinationAddress,destinationPort);
            // qCritical(datagram);
                // QIODevice *audioIO ;QAudioSink *ao ;
                // ao= new QAudioSink(QMediaDevices::defaultAudioOutput(),format);
                // ao->setBufferSize(48000);
                // audioIO =ao->start();
            while(f3->isVisible()){
                // destinationAddress = QHostAddress(devices[nam][1]);
                // qCritical(io->readAll().toBase64());
                if(send_chan!=0){
                if(io->bytesAvailable()>0){
                QByteArray rwBuff =io->readAll();// QString::number(jit_num).toUtf8()+io->read(8192);
                    // qCritical(QString::number(rwBuff.size()).toUtf8());
                for(QHostAddress abc: to_send){
                    udpSocket.writeDatagram(rwBuff, rwBuff.size(), abc, 4444+send_chan);}
                // jit_num=(jit_num+1)%7;
                        // QThread::msleep(20);
                // audioIO->write(rwBuff);
                }}
                else{
                    QThread::msleep(100);
                }
                // qCritical((currentThread()== QCoreApplication::instance()->thread())?"1":"0");
            }
            audioInput->stop();
        }
        else if(f4->isVisible()){

            // qCritical((QThread::currentThread()== QCoreApplication::instance()->thread())?"1":"0");
            QHostAddress destinationAddress(devices[nam][1]);
            QUdpSocket *udpSocket=new QUdpSocket();
            quint16 destinationPort = 5555;
            QByteArray datagram="Glove Thingi list";
            udpSocket->writeDatagram(datagram.data(),datagram.size(),destinationAddress,destinationPort);
            // udpSocket=nullptr;//("239.134.233.10");
            QAudioSink *ao ;
            QUdpSocket *socke ;QIODevice *audioIO ;
            while(f4->isVisible()){
                if(con_chan!=0){
                    quint16 port = 4444+con_chan;
                    ao= new QAudioSink(QMediaDevices::defaultAudioOutput(),format);
                    ao->setBufferSize(8192);
                    audioIO =ao->start();
                    socke = new QUdpSocket();
                    datagram="Glove Thingi conn "+QString::number(con_chan).toUtf8();
                    udpSocket->writeDatagram(datagram.data(),datagram.size(),destinationAddress,destinationPort);
                    udpSocket->close();
                    delete udpSocket;
                    // socke->connectToHost(multicastAddress, port);
                    socke->bind(QHostAddress::AnyIPv4, port);//,QUdpSocket::ShareAddress| QUdpSocket::ReuseAddressHint);
                    // QNetworkInterface iface = QNetworkInterface::interfaceFromName("wlp2s0");
                    // qCritical(socke->joinMulticastGroup(multicastAddress,iface)==true?QString::number(port).toUtf8():"0");
                    // qCritical((QThread::currentThread()== QCoreApplication::instance()->thread())?"1":"0");
                    // audioIO =ao->start();
                    break;
                }
            }
            QThread* t = QThread::create([=]{
                while(true){

            if(toread!=towrite ){audioIO->write(ruff[toread]);toread=(toread+1)%20;}
                    }
                QEventLoop loop;
                loop.exec();
            });
            t->start();           //qCritical("Enter or not");
            // QByteArray ruff;
            // ruff.resize(5096);
            for(int i=0;i<20;i++){ruff[i].resize(4096);
                }
            while(f4->isVisible()){
                while (socke->hasPendingDatagrams()) {
                    socke->readDatagram(ruff[towrite].data(),4096);
                    // audioIO->write(ruff[towrite]);
                    towrite=(towrite+1)%20;
// qCritical("read");
                }
                // QThread::msleep(100);
            }
            // t->exit();
            ao->stop();
        }
        // exec();
    }
};

void readReady(){
    while(lis->hasPendingDatagrams())
    {

        QByteArray UDPBuffer;

        UDPBuffer.resize(lis->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        lis->readDatagram(UDPBuffer.data(),UDPBuffer.size(), &sender, &senderPort);
        QString s = UDPBuffer.data();
        // qCritical(s.toUtf8());
        if(s.startsWith("Glove Thingi response Name:")){
            QString name="";
            int i=27;
            while(s[i]!=' '){
                name=name.append(s[i++]);
            }
            i+=5;
            QString key="";
            while(i<s.length() && s[i]!=' '){
                key=key.append(s[i++]);
            }
            func(name,key,sender);
        }
        if(s.startsWith("Glove Thingi send ")){
            QString channel_num="";
            int i=18;
            while(i<s.length() && s[i]!=' '){
                channel_num=channel_num.append(s[i++]);
            }
            // qCritical(channel_num.toUtf8());
            send_chan=channel_num.toInt();
            QMetaObject::invokeMethod(qApp, [=]() {
                Ch_label->setText("Channel "+channel_num);
            });

        }
        if(s.startsWith("Glove Thingi conn ")){
            QString connected_ip="";
            int i=18;
            while(i<s.length() && s[i]!=' '){
                connected_ip=connected_ip.append(s[i++]);
            }
            // qCritical(channel_num.toUtf8());
            // send_chan=channel_num.toInt();
            QHostAddress ip(connected_ip);
            to_send.append(ip);
            QMetaObject::invokeMethod(qApp, [=]() {
                if(to_send.length()==1){
                    Ch_label->setText(Ch_label->text()+"\n\nConnected devices:\n"+connected_ip);}
                else{Ch_label->setText(Ch_label->text()+"\n"+connected_ip);}
            });
        }
        if(s.startsWith("Glove Thingi list ")){
            QString channels_num="";
            int cn=0;
            int i=18;
            while(i<s.length() && s[i]!=' '){
                channels_num=channels_num.append(s[i++]);
            }
            cn=channels_num.toInt();
            // qCritical(channel_num.toUtf8());
            QPushButton *b;
            for(int i=1;i<=cn;i++){
                b=new QPushButton(QString::number(i));
                QString temp=b->text();
                www->connect(b,&QPushButton::clicked,[temp](){
                    con_chan=temp.toInt();
                });
                l5->addWidget(b);
            }

        }
        if(s.startsWith("Glove Thingi response sucessfully connected Name:")){
            QString name="";
            int i=49;
            while(i<s.length() && s[i]!=' '){
                name=name.append(s[i++]);
            }

            if(nam==name) {connected=true;
                qCritical(devices[nam][1].toUtf8());
            connect_frame->setVisible(false);
            f2->setVisible(true);
            }
            // if(s.startsWith("Glove Thingi values ") && devices.contains(nam) && sender.toString()==devices[nam][1] && connected==true){

        }
    }
}

void ping(){
    QUdpSocket udpSocket;
    QByteArray datagram = "Glove Thingi ping";
    QLayoutItem* child;
    while(l2->count()!=0)
    {
        child = l2->takeAt(0);
        delete child;
    }
    devices.clear();
    QList<QNetworkInterface> is = QNetworkInterface::allInterfaces();
    // QNetworkInterface ai;
    quint16 destinationPort = 5555;
    QHostAddress broadcastAddress;
    if(!broadcast){
        bool ok;
        QString text = QInputDialog::getText(www, ("Input Dialog"),("Enter your ip:"), QLineEdit::Normal,QDir::home().dirName(), &ok);
        if (ok && !text.isEmpty()) {broadcastAddress=*(new QHostAddress(text));}}
    for(const QNetworkInterface& i:is){
        if(i.flags().testFlag(QNetworkInterface::IsUp) && !i.flags().testFlag(QNetworkInterface::IsLoopBack)){
            for (const QNetworkAddressEntry &entry : i.addressEntries()) {
                // Check if the address is IPv4 before getting the broadcast address
                if (entry.ip().protocol() == QHostAddress::IPv4Protocol && !entry.broadcast().isNull()) {
                    // qCritical(i.name().toUtf8());
                    if(broadcast){ broadcastAddress = entry.broadcast();}
                    udpSocket.writeDatagram(datagram.data(), datagram.size(), broadcastAddress, destinationPort);
                }
            }
            // ai = i;
            // break;
        }
    }
    broadcast=true;
    // QHostAddress destinationAddress = ai.addressEntries().first().broadcast();//QHostAddress::Broadcast;
    // qCritical(destinationAddress.toString().toUtf8());
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QVBoxLayout l1;
    f1=new QFrame();
    connect_frame=new QFrame();
    QVBoxLayout connect_layout;
    l2= new QVBoxLayout();
    f2=new QFrame();
    l3= new QVBoxLayout();
    f3=new QFrame();
    l4= new QVBoxLayout();
    f4=new QFrame();
    l5= new QVBoxLayout();
    Ch_label = new QLabel("Channel ???");
    w.centralWidget()->setLayout(&l1);
    QPushButton send_button("Send");
    QPushButton stopsend_button("Stop");
    QPushButton rec_button("Receive");
    QPushButton disconnect_button("Disconnect");
    QPushButton b("Reload");
    QPushButton ipb("Connect with ip");

    QComboBox *comboBox = new QComboBox();

    comboBox->addItem("Microphone");
    comboBox->addItem("Audio Loopback");
    comboBox->setCurrentIndex(1);

    format.setSampleRate(48000);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Int16);
    w.connect(comboBox, &QComboBox::currentIndexChanged, [](int index) {
        if(index==0){audioInput= new QAudioSource(QMediaDevices::defaultAudioInput(),format);}
        else{
            QAudioDevice loopbackDevice;
            for (const QAudioDevice &device : QMediaDevices::audioInputs()) {
                if (device.description().contains("combined", Qt::CaseInsensitive) &&
                    device.description().contains("monitor", Qt::CaseInsensitive)) {
                    loopbackDevice = device;
                    break;
                }
            }
            audioInput = new QAudioSource(loopbackDevice, format);
        }
    });
    QThread *t= new QThread();
    Worker *work= new Worker();
    // work->moveToThread(t)
    lis= new QUdpSocket();
    lis->bind(QHostAddress::Any,5555);
    // lis->moveToThread(t);
    w.connect(lis,&QUdpSocket::readyRead,&w,readReady,Qt::QueuedConnection);
    // qCritical((currentThread()== t)?"1":"0");
    w.connect(t, &QThread::finished, t, &QObject::deleteLater);
    w.connect(&b,&QPushButton::clicked,&w,ping);
    w.connect(&ipb,&QPushButton::clicked,[](){broadcast=false;ping();});
    w.connect(&send_button,&QPushButton::clicked,&w,[work](){
        f2->setVisible(false);
        f3->setVisible(true);
        work->runinThread();});
    w.connect(&rec_button,&QPushButton::clicked,&w,[work](){
        f2->setVisible(false);
        f4->setVisible(true);
        work->runinThread();});
    w.connect(&stopsend_button,&QPushButton::clicked,&w,[](){
        f3->setVisible(false);
        f2->setVisible(true);});
    w.connect(&disconnect_button,&QPushButton::clicked,&w,[](){
        f4->setVisible(false);
        f2->setVisible(true);});
    // w.connect(&rec_button,&QPushButton::clicked,&w,recFunc);
    // t->start();
    l1.addWidget(connect_frame);
    l1.addWidget(f2);
    l1.addWidget(f3);
    l1.addWidget(f4);
    f2->setVisible(false);
    f3->setVisible(false);
    f4->setVisible(false);
    // l1.setSizeConstraint(QLayout::SetFixedSize);
    f1->setFrameStyle(QFrame::Box | QFrame::Raised);
    f1->setMinimumSize(400,400);
    connect_layout.addWidget(&b);
    connect_layout.addWidget(&ipb);
    connect_layout.addWidget(f1);
    // l2->addWidget(&b);
    f1->setLayout(l2);
    f4->setLayout(l5);
    l4->addWidget(Ch_label);
    l4->addWidget(comboBox);
    l4->addWidget(&stopsend_button);
    l3->addWidget(&send_button);
    l3->addWidget(&rec_button);
    l5->addWidget(&disconnect_button);
    connect_frame->setLayout(&connect_layout);
    f2->setLayout(l3);
    f3->setLayout(l4);
    // w.setLayout(&l1);
    w.setWindowTitle("SoundRelay");
    w.show();
    return a.exec();
}
