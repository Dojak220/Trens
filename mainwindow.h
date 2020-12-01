#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QMessageBox>
#include <QPixmap>


#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    int blue_x, blue_y, green_x, green_y, red_x, red_y, purple_x, purple_y;
    pthread_t blue_train_thread, green_train_thread, red_train_thread, purple_train_thread, speeds_thread, rails_thread; // declaração de 05 threads

private slots:
//    void* green_train_thread_function(void*);
//    void* blue_train_thread_function(void*);
//    void* purple_train_thread_function(void*);
//    void* red_train_thread_function(void*);
//    void* speeds_thread_function(void*);
//    void* rails_thread_function(void*);

    void on_StartTrains_triggered();
//    void init_semaphore(sem_t sem, unsigned int value);
//    void create_thread(pthread_t /* *__restrict */ thread, void thread_function);

    void on_StopTrains_triggered();
    void join_thread(pthread_t thread);

private:
    Ui::MainWindow *ui;

//    sem_t sem_L3, sem_L4, sem_L5, sem_L6, sem_L10, sem_L3L4L6, sem_L5L6L10, sem_L3L4L5L6L10;
//    int blue_train_speed, green_train_speed, red_train_speed, purple_train_speed;


    int res;
    void *thread_result;
};
#endif // MAINWINDOW_H
