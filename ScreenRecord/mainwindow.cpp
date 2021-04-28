#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QMainWindow>
#include<QDebug>
#include<QEvent>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<QMainWindow::size();
    mainWindowWidth=QMainWindow::size().width();
    mainWindowHeight=QMainWindow::size().height();
    this->convertProcess=new QProcess;
    this->processor=NULL;
    connect (this->convertProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));  // connect process signals with your code
    connect (this->convertProcess, SIGNAL(readyReadStandardError()), this, SLOT(processOutput()));
    this->timeTimer=new QTimer(this);
    connect(this->timeTimer,SIGNAL(timeout()),this,SLOT(updateTimer()));
    this->timerTime=new QTime(0,0);
    this->ui->timer_label->setText(this->timerTime->toString());
    this->ui->start_record_button->setIcon(QIcon(QPixmap(":/Images/recordIcon.png")));
    this->ui->start_record_button->setIconSize(QSize(95,95));
    this->ui->stop_record_button->setIcon(QIcon(QPixmap(":/Images/stopIcon.png")));
    this->ui->stop_record_button->setIconSize(QSize(55,55));
    this->ui->video_open_button->setIcon(QIcon(QPixmap(":/Images/fileIcon.png")));
    this->ui->video_open_button->setIconSize(QSize(55,55));
    this->ui->video_open_extract_ss_button->setIcon(QIcon(QPixmap(":/Images/fileIcon.png")));
    this->ui->video_open_extract_ss_button->setIconSize(QSize(55,55));
    this->ui->output_plain_text_edit->setVisible(false);
    this->ui->start_record_button->setDisabled(false);
    this->ui->output_plain_text_edit->setReadOnly(true);

    convert_frame_button_style="QPushButton#convert_frame_button{border: 2px groove rgb(30, 110, 139);font: 12pt 'Verdana';color: rgb(39, 146, 181);}QPushButton#convert_frame_button:hover{border: 3px groove rgb(30, 110, 139);font: 13pt 'Verdana';color: rgb(39, 146, 181);}";
    recorder_frame_button_style="QPushButton#recorder_frame_button{border: 2px groove rgb(30, 110, 139);font: 12pt 'Verdana';color: rgb(39, 146, 181);}QPushButton#recorder_frame_button:hover{border: 3px groove rgb(30, 110, 139);font: 13pt 'Verdana';color: rgb(39, 146, 181);}";
    extract_ss_frame_button_style="QPushButton#extract_ss_frame_button{border: 2px groove rgb(30, 110, 139);font: 12pt 'Verdana';color: rgb(39, 146, 181);}QPushButton#extract_ss_frame_button:hover{border: 3px groove rgb(30, 110, 139);font: 13pt 'Verdana';color: rgb(39, 146, 181);}";
    this->ui->stop_record_button->setDisabled(true);
    this->isMicOn=true;

    setApplicationLayout();

}

MainWindow::~MainWindow()
{
    delete ui;
    if(this->processor!=NULL)
    {
        this->processor->write("q\n");
        this->processor->waitForFinished();
        this->processor->close();
    }
    delete this->processor;
    delete this->extractProcess;
    delete this->convertProcess;
}



QString str="";
void MainWindow::processOutput()
{
    QString str=this->convertProcess->readAllStandardError();

    qDebug()<<str.length();
   if(str.contains("muxing overhead:",Qt::CaseSensitive) && str.indexOf("%")!=-1)
   {
    this->convertProcess->waitForFinished();
    this->convertProcess->close();
    this->ui->output_plain_text_edit->setPlainText("Conversion Completed");
    this->ui->file_name_label->setText("File Name: ");
   }
   else
   {
       this->ui->output_plain_text_edit->appendPlainText(str);
   }

}




void MainWindow::updateTimer()
{
    QTime temp=this->timerTime->addSecs(1);
    delete this->timerTime;
    this->timerTime=new QTime(temp);
    this->ui->timer_label->setText(this->timerTime->toString());
}


void MainWindow::setApplicationLayout()
{
    this->ui->main_frame->setGeometry(0,0,mainWindowWidth,mainWindowHeight);
    this->ui->widget_frame->setGeometry(10+mainWindowWidth/6+110,mainWindowHeight/6+50,mainWindowWidth/2,mainWindowHeight/3);
    int widgetFrameWidth=this->ui->widget_frame->width();
    int widgetFrameHeight=this->ui->widget_frame->height();
    this->ui->timer_label->setGeometry(10+widgetFrameWidth/3+80,10,widgetFrameWidth/2,100);
    this->ui->start_record_button->setGeometry(widgetFrameWidth/5+110+130-100-50,100+50,110,110);
    this->ui->start_record_button->setStyleSheet(QString("QPushButton#start_record_button{border-radius:55px;background:#FFF} QPushButton#start_record_button:hover{border: 3px groove #ec0b29;}"));
    this->ui->stop_record_button->setGeometry(widgetFrameWidth/5+110+130,100+55,100,100);
    this->ui->stop_record_button->setStyleSheet(QString("QPushButton#stop_record_button{border-radius:50px;background:#FFF;} QPushButton#stop_record_button:hover{border: 3px groove #ec0b29;}"));
    this->ui->mic_button->setGeometry(widgetFrameWidth/5+110+130+100+50,100+50,110,110);
    this->ui->mic_button->setStyleSheet(QString("QPushButton#mic_button{border-radius:55px;background:#FFF} QPushButton#mic_button:hover{border: 3px groove #ec0b29;}"));
    if(isMicOn==true)
    {
        this->ui->mic_button->setIcon(QIcon(QPixmap(":/Images/micOn.png")));
        this->ui->mic_button->setIconSize(QSize(55,55));
    }
    else
    {
        this->ui->mic_button->setIcon(QIcon(QPixmap(":/Images/micOff.png")));
        this->ui->mic_button->setIconSize(QSize(55,55));
    }
    //hyperlink_button_frame
    this->ui->hyperlink_button_frame->setGeometry(mainWindowWidth/5-10,50,1081,80);
    this->ui->recorder_frame_button->setStyleSheet("QPushButton#recorder_frame_button{border: 2px groove #0ed145;font: 12pt 'Verdana';color: #0ed145;}");
    this->ui->convert_frame_button->setStyleSheet(convert_frame_button_style);
    this->ui->extract_ss_frame_button->setStyleSheet(extract_ss_frame_button_style);


    //output_plain_text_edit
    this->ui->output_plain_text_edit->setGeometry(10+mainWindowWidth/6+110,mainWindowHeight/2+80,mainWindowWidth/2,mainWindowHeight/3);

    //widget_frame_2 layout
    this->ui->widget_frame_2->setVisible(false);
    this->ui->widget_frame_3->setVisible(false);
    this->ui->widget_frame_2->setGeometry(10+mainWindowWidth/6+110,mainWindowHeight/6+50,mainWindowWidth/2,mainWindowHeight/3);
    int widgetFrame2Width=this->ui->widget_frame_2->width();
    int widgetFrame2Height=this->ui->widget_frame_2->height();
    this->ui->convertLabel->setGeometry(widgetFrame2Width/3+70,10,widgetFrame2Width/2,100);
    this->ui->file_name_label->setGeometry(90,widgetFrame2Height/2-30,600,60);
    this->ui->video_open_button->setGeometry(150,widgetFrame2Height/2+50,60,60);
    this->ui->video_open_button->setStyleSheet("QPushButton#video_open_button{font: 12pt 'Verdana';color: rgb(39, 146, 181);border:none;border-radius:20px;}QPushButton#video_open_button:hover{border: 3px groove #ffca18;font: 13pt 'Verdana';color: rgb(39, 146, 181);};");
    this->ui->convertOptioncomboBox->setCurrentIndex(0);
    this->ui->convertOptioncomboBox->setGeometry(140+150,widgetFrame2Height/2+60,widgetFrame2Width/3,40);
    this->ui->convert_button->setGeometry(widgetFrame2Height/2+60+450,widgetFrame2Height/2+55,150,50);
    this->ui->convert_button->setStyleSheet(QString("QPushButton#convert_button{font: 11pt 'Verdana';color: rgb(39, 146, 181);border: 2px groove #ffca18;border-radius:55px;background:#FFF}QPushButton#convert_button:hover{border: 3px groove #ffca18;font: 13pt 'Verdana';color: rgb(39, 146, 181);};"));
    this->ui->hr_line_edit->setText("00");
    this->ui->min_line_edit->setText("00");
    this->ui->sec_line_edit->setText("00");


    //widget_frame_3 layout
    this->ui->widget_frame_3->setGeometry(10+mainWindowWidth/6+110,mainWindowHeight/6+50,mainWindowWidth/2,mainWindowHeight/3);
    int widgetFrame3Width=this->ui->widget_frame_3->width();
    int widgetFrame3Height=this->ui->widget_frame_3->height();
    this->ui->extract_ss_label->setGeometry(widgetFrame3Width/3,10,widgetFrame3Width/2,100);
    this->ui->extract_ss_button->setGeometry(widgetFrame3Height/2+60+450,widgetFrame3Height/2+55,150,50);
    this->ui->extract_ss_button->setStyleSheet(QString("QPushButton#extract_ss_button{font: 11pt 'Verdana';color: rgb(39, 146, 181);border: 2px groove #ffca18;border-radius:55px;background:#FFF}QPushButton#extract_ss_button:hover{border: 3px groove #ffca18;font: 13pt 'Verdana';color: rgb(39, 146, 181);};"));
    this->ui->time_frame->setGeometry(140+150,widgetFrame3Height/2+50,widgetFrame3Width/3,80);
    this->ui->video_open_extract_ss_button->setGeometry(150,widgetFrame3Height/2+50,60,60);
    this->ui->video_open_extract_ss_button->setStyleSheet("QPushButton#video_open_extract_ss_button{font: 12pt 'Verdana';color: rgb(39, 146, 181);border:none;border-radius:20px;}QPushButton#video_open_extract_ss_button:hover{border: 3px groove #ffca18;font: 13pt 'Verdana';color: rgb(39, 146, 181);};");
    this->ui->extract_ss_file_name_label->setGeometry(90,widgetFrame2Height/2-30,600,60);

    //output label

}

void MainWindow::changeEvent(QEvent *e)
{
    if(e->type()==QEvent::WindowStateChange)
    {
        mainWindowWidth=QMainWindow::size().width();
        mainWindowHeight=QMainWindow::size().height();
        setApplicationLayout();
    }

}

void MainWindow::on_startRecordButton_clicked()
{
    this->processor=new QProcess;
    this->timerTime->setHMS(0,0,0);
    this->ui->timer_label->setText(this->timerTime->toString());
    processor->setProgram("cmd.exe");
    processor->setArguments({"/k","cd D:"});
    QString jsonFilter="All files";
    QString fileName=QFileDialog::getSaveFileName(this,"Open the file","/","All files",&jsonFilter,QFileDialog::DontUseNativeDialog);
    qDebug()<<fileName;
    QStringList myStringList=fileName.split(".");
    processor->start("ffmpeg -f gdigrab -framerate 30 -i desktop -f dshow -i audio=\"Microphone Array (Realtek(R) Audio)\" "+fileName);
    this->timeTimer->start(996);
}

void MainWindow::on_stopRecordButton_clicked()
{
    delete this->timerTime;
    this->timerTime=new QTime(0,0);
    this->ui->timer_label->setText(this->timerTime->toString());
    this->timeTimer->stop();
    processor->write("q\n");
    processor->waitForFinished();
    processor->close();
}

void MainWindow::on_recorder_frame_button_clicked()
{
    this->ui->widget_frame->setVisible(true);
    this->ui->widget_frame_2->setVisible(false);
    this->ui->widget_frame_3->setVisible(false);
    this->ui->output_plain_text_edit->setVisible(false);
    this->ui->output_plain_text_edit->setPlainText("");
    this->ui->recorder_frame_button->setDisabled(true);
    this->ui->convert_frame_button->setDisabled(false);
    this->ui->extract_ss_frame_button->setDisabled(false);
    this->ui->recorder_frame_button->setStyleSheet("QPushButton#recorder_frame_button{border: 2px groove #0ed145;font: 12pt 'Verdana';color: #0ed145;}");
    this->ui->convert_frame_button->setStyleSheet(convert_frame_button_style);
    this->ui->extract_ss_frame_button->setStyleSheet(extract_ss_frame_button_style);


}

void MainWindow::on_extract_ss_frame_button_clicked()
{
    this->ui->widget_frame->setVisible(false);
    this->ui->widget_frame_2->setVisible(false);
    this->ui->widget_frame_3->setVisible(true);
    this->ui->output_plain_text_edit->setVisible(false);
    this->ui->output_plain_text_edit->setPlainText("");
    this->ui->recorder_frame_button->setDisabled(false);
    this->ui->convert_frame_button->setDisabled(false);
    this->ui->extract_ss_frame_button->setDisabled(true);
    this->ui->extract_ss_frame_button->setStyleSheet("QPushButton#extract_ss_frame_button{border: 2px groove #0ed145;font: 12pt 'Verdana';color: #0ed145;}");
    this->ui->convert_frame_button->setStyleSheet(convert_frame_button_style);
    this->ui->recorder_frame_button->setStyleSheet(recorder_frame_button_style);
}


void MainWindow::on_mic_button_clicked()
{
    if(isMicOn==true) isMicOn=false;
    else isMicOn=true;
    if(isMicOn==true)
    {
        this->ui->mic_button->setIcon(QIcon(QPixmap(":/Images/micOn.png")));
        this->ui->mic_button->setIconSize(QSize(55,55));
    }
    else
    {
        this->ui->mic_button->setIcon(QIcon(QPixmap(":/Images/micOff.png")));
        this->ui->mic_button->setIconSize(QSize(55,55));
    }
}

void MainWindow::on_start_record_button_clicked()
{
    this->ui->stop_record_button->setDisabled(false);
    this->ui->start_record_button->setDisabled(true);
    this->ui->mic_button->setDisabled(true);
    this->processor=new QProcess;
    this->timerTime->setHMS(0,0,0);
    this->ui->timer_label->setText(this->timerTime->toString());
    processor->setProgram("cmd.exe");
    processor->setArguments({"/k","cd D:"});

    QStringList myStringList=this->fileName.split(".");
    if(isMicOn==true)
    {
      qDebug()<<"dir: "<<QDir::currentPath()+"/"+QDateTime::currentDateTime().toString(QString("dd-MM-yyyy_hh-mm-ss"))+".mp4";
     processor->start("ffmpeg -f gdigrab -framerate 30 -i desktop -f dshow -i audio=\"Microphone Array (Realtek(R) Audio)\" "+QDir::currentPath()+"/"+QDateTime::currentDateTime().toString(QString("dd-MM-yyyy_hh-mm-ss"))+".mp4");
    }
    else
    {
        processor->start("ffmpeg -f gdigrab -framerate 30 -i desktop "+QDir::currentPath()+"/"+QDateTime::currentDateTime().toString(QString("dd-MM-yyyy_hh-mm-ss"))+".mp4");
    }
    this->timeTimer->start(996);
}

void MainWindow::on_stop_record_button_clicked()
{
    this->ui->stop_record_button->setDisabled(true);
    this->ui->start_record_button->setDisabled(false);
    this->ui->mic_button->setDisabled(false);
    delete this->timerTime;
    this->timerTime=new QTime(0,0);
    this->ui->timer_label->setText(this->timerTime->toString());
    this->timeTimer->stop();
    this->processor->write("q\n");
    this->processor->waitForFinished();
    this->processor->close();
    delete this->processor;
    this->processor=NULL;
}

void MainWindow::on_video_open_button_clicked()
{
    QString jsonFilter="*.mp4,*.avi,*.mkv";
    this->convertFileName=QFileDialog::getOpenFileName(this,"Open the file","/","*.mp4\n*.avi",&jsonFilter,QFileDialog::DontUseNativeDialog);
    int i=0;
    for (i=convertFileName.length()-1;convertFileName.at(i)!='\\' && convertFileName.at(i)!='/';i--);
    this->convertFilePath=convertFileName.left(i+1);
    this->ui->file_name_label->setText("File Name: "+this->convertFileName);
}


void MainWindow::on_convert_frame_button_clicked()
{
    this->ui->widget_frame->setVisible(false);
    this->ui->widget_frame_2->setVisible(true);
    this->ui->widget_frame_3->setVisible(false);
    this->ui->output_plain_text_edit->setVisible(true);
    this->ui->output_plain_text_edit->setPlainText("");
    this->ui->recorder_frame_button->setDisabled(false);
    this->ui->convert_frame_button->setDisabled(true);
    this->ui->extract_ss_frame_button->setDisabled(false);
    this->ui->convert_frame_button->setStyleSheet("QPushButton#convert_frame_button{border: 2px groove #0ed145;font: 12pt 'Verdana';color: #0ed145;}");
    this->ui->extract_ss_frame_button->setStyleSheet(extract_ss_frame_button_style);
    this->ui->recorder_frame_button->setStyleSheet(recorder_frame_button_style);
}

void MainWindow::on_convert_button_clicked()
{
    QString format=this->ui->convertOptioncomboBox->currentText();
    QString convertedFileName=convertFileName.left(convertFileName.indexOf("."));
    qDebug()<<convertedFileName+"."+format<<endl;
    this->ui->output_plain_text_edit->setVisible(true);
    if(convertFileName.right(3).compare(format)==0)
    {
        this->ui->output_plain_text_edit->setPlainText("File is already in "+format);
    }
    else
    {
        this->convertProcess->start("ffmpeg -i  "+convertFileName+" "+convertFilePath+QDateTime::currentDateTime().toString(QString("dd-MM-yyyy_hh-mm-ss"))+"."+format);
    }
}

void MainWindow::on_video_open_extract_ss_button_clicked()
{
    QString jsonFilter="*.mp4,*.avi,*.mkv";
    this->extractFileName=QFileDialog::getOpenFileName(this,"Open the file","/","*.mp4\n*.avi\n*mkv",&jsonFilter,QFileDialog::DontUseNativeDialog);
    int i=0;
    for (i=extractFileName.length()-1;extractFileName.at(i)!='\\' && extractFileName.at(i)!='/';i--);
    this->extractFilePath=extractFileName.left(i+1);
    this->ui->extract_ss_file_name_label->setText("File Name: "+this->extractFileName);

}

void MainWindow::on_extract_ss_button_clicked()
{
     QString hour=this->ui->hr_line_edit->text();
     QString min=this->ui->min_line_edit->text();
     QString sec=this->ui->sec_line_edit->text();
     QString validatorString="0123456789";
     QString extractSSTime=hour+":"+min+":"+sec;
     this->ui->output_plain_text_edit->setPlainText("");
     qDebug()<<hour.left(1);
     qDebug()<<hour.right(1);
     qDebug()<<min.left(1);
     qDebug()<<min.right(1);
     qDebug()<<sec.left(1);
     qDebug()<<sec.right(1);

     if(validatorString.contains(hour.left(1)) && validatorString.contains(hour.right(1)) && validatorString.contains(min.left(1)) && validatorString.contains(min.right(1)) && validatorString.contains(sec.left(1)) && validatorString.contains(sec.right(1)))
     {
        this->ui->output_plain_text_edit->setVisible(false);
        this->extractProcess=new QProcess;
        this->extractProcess->setProgram("cmd.exe");
         this->extractProcess->setArguments({"/k","cd D:"});
          this->extractProcess->setArguments({"/k","path=C:/Program Files/UnrealStreaming/UScreenCapture"});
          this->extractProcess->setArguments({"/k","ffmpeg -i "+extractFileName+" -ss "+hour+":"+min+":"+sec+" -vframes 1 "+extractFilePath+QDateTime::currentDateTime().toString(QString("dd-MM-yyyy_hh-mm-ss"))+".png"});
           this->extractProcess->setCreateProcessArgumentsModifier([] (QProcess::CreateProcessArguments*args) {
           });
           this->extractProcess->startDetached();
         delete this->extractProcess;
         this->ui->hr_line_edit->setText("00");
         this->ui->min_line_edit->setText("00");
         this->ui->sec_line_edit->setText("00");
         this->ui->output_plain_text_edit->setPlainText("Image extracted.");
         this->ui->output_plain_text_edit->setVisible(true);
         this->ui->extract_ss_file_name_label->setText("File Name: ");
     }
     else
     {
        this->ui->output_plain_text_edit->appendPlainText("Error: Invalid time is given");
        this->ui->output_plain_text_edit->setVisible(true);
     }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->processor!=NULL)
    {
        this->processor->write("q\n");
        this->processor->waitForFinished();
        this->processor->close();
    }
    delete this->processor;
    delete this->extractProcess;
    delete this->convertProcess;
}
