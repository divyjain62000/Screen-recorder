#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    int mainWindowWidth;
    int mainWindowHeight;
    QProcess *processor;
    QProcess *convertProcess;
    QProcess *extractProcess;
    QThread *qThread;
    QString recorder_frame_button_style;
    QString convert_frame_button_style;
    QString extract_ss_frame_button_style;
    bool isMicOn;
    QString fileName;
    QString convertFileName;
    QString extractFileName;
    QString convertFilePath;
    QString extractFilePath;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeEvent(QEvent *e);
    void setApplicationLayout();
    void closeEvent(QCloseEvent *event);
public slots:
    void updateTimer();
    void processOutput();
private slots:
    void on_startRecordButton_clicked();

    void on_stopRecordButton_clicked();

    void on_recorder_frame_button_clicked();

    void on_extract_ss_frame_button_clicked();

    void on_convert_frame_button_clicked();

    void on_mic_button_clicked();

    void on_start_record_button_clicked();

    void on_stop_record_button_clicked();

    void on_video_open_button_clicked();

    void on_convert_button_clicked();

    void on_video_open_extract_ss_button_clicked();

    void on_extract_ss_button_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timeTimer;
    QTime *timerTime;
};
#endif // MAINWINDOW_H
