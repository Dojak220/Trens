#include "mainwindow.h"
#include "ui_mainwindow.h"

#define BASE_SPEED 9

void* green_train_thread_function(void*);
void* blue_train_thread_function(void*);
void* purple_train_thread_function(void*);
void* red_train_thread_function(void*);
void* speeds_thread_function(void*);
void* rails_thread_function(void*);

int blue_x, blue_y, green_x, green_y, red_x, red_y, purple_x, purple_y;
int blue_train_speed, green_train_speed, red_train_speed, purple_train_speed;

sem_t sem_L3, sem_L4, sem_L5, sem_L6, sem_L10, sem_L3L4L6, sem_L5L6L10, sem_L3L4L5L6L10;

Ui::MainWindow ui_aux;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui_aux = *ui;

    blue_train_speed = BASE_SPEED - ui->BlueSpeedSlider->value();
    green_train_speed = BASE_SPEED - ui->GreenSpeedSlider->value();
    red_train_speed = BASE_SPEED - ui->RedSpeedSlider->value();
    purple_train_speed = BASE_SPEED - ui->PurpleSpeedSlider->value();

    ui->BlueTrain->setVisible(false);
    ui->BlueTrain->setGeometry(230,254,40,40);
    ui->BlueTrain->setPixmap(QPixmap("/home/dorgival/Pictures/trains_assets/BlueTrainLeft.png"));

    ui->GreenTrain->setVisible(false);
    ui->GreenTrain->setGeometry(90,10,40,40);
    ui->GreenTrain->setPixmap(QPixmap("/home/dorgival/Pictures/trains_assets/GreenTrainRight.png"));

    ui->PurpleTrain->setVisible(false);
    ui->PurpleTrain->setGeometry(230,10,40,40);
    ui->PurpleTrain->setPixmap(QPixmap("/home/dorgival/Pictures/trains_assets/PurpleTrainRight.png"));

    ui->RedTrain->setVisible(false);
    ui->RedTrain->setGeometry(370,10,40,40);
    ui->RedTrain->setPixmap(QPixmap("/home/dorgival/Pictures/trains_assets/RedTrainRight.png"));

    blue_x = ui->BlueTrain->x();
    blue_y = ui->BlueTrain->y();
    green_x = ui->GreenTrain->x();
    green_y = ui->GreenTrain->y();
    red_x = ui->RedTrain->x();
    red_y = ui->RedTrain->y();
    purple_x = ui->PurpleTrain->x();
    purple_y = ui->PurpleTrain->y();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Trilhos do trem verde: L1 -> L2 -> L3* -> L4*
void* green_train_thread_function(void*){
    sleep(1);
    while (1)
    {
        // Trem Verde: L4 -> L1
        printf("Trem Verde - Trilho L1 \n");
        green_x = 25;
        green_y = 73;
        sleep(green_train_speed);
        sem_post(&sem_L4);

        // Trem Verde: L1 -> L2
        printf("Trem Verde - Trilho L2 \n");
        green_x = 90;
        green_y = 10;
        sleep(green_train_speed);

        // Trem Verde: L2 -> L3
        sem_wait(&sem_L3);
        sem_wait(&sem_L3L4L6);
        sem_wait(&sem_L3L4L5L6L10);
        printf("Trem Verde - Trilho L3 \n");
        green_x = 161;
        green_y = 73;
        sleep(green_train_speed);

        // Trem Verde: L3 -> L4
        sem_wait(&sem_L4);
        printf("Trem Verde - Trilho L4 \n");
        green_x = 90;
        green_y = 134;
        sem_post(&sem_L3L4L5L6L10);
        sem_post(&sem_L3L4L6);
        sem_post(&sem_L3);
        sleep(green_train_speed);
    }
    pthread_exit(0);
}

// Trilhos do trem roxo: L7 -> L5* -> L6* -> L3*
void* purple_train_thread_function(void*){
    sleep(1);
    while (1)
    {
        // Trem Roxo: L3 -> L7
        printf("Trem Roxo - Trilho L7 \n");
        purple_x = 230;
        purple_y = 10;
        sem_post(&sem_L3);
        sleep(purple_train_speed);

        // Trem Roxo: L7 -> L5
        sem_wait(&sem_L5);
        sem_wait(&sem_L5L6L10);
        sem_wait(&sem_L3L4L5L6L10);
        printf("Trem Roxo - Trilho L5 \n");
        purple_x = 295;
        purple_y = 73;
        sleep(purple_train_speed);

        // Trem Roxo: L5 -> L6
        sem_wait(&sem_L6);
        sem_wait(&sem_L3L4L6);
        printf("Trem Roxo - Trilho L6 \n");
        purple_x = 230;
        purple_y = 134;
        sem_post(&sem_L3L4L5L6L10);
        sem_post(&sem_L5L6L10);
        sem_post(&sem_L5);
        sleep(purple_train_speed);

        // Trem Roxo: L6 -> L3
        sem_wait(&sem_L3);
        printf("Trem Roxo - Trilho L3 \n");
        purple_x = 165;
        purple_y = 73;
        sem_post(&sem_L3L4L6);
        sem_post(&sem_L6);
        sleep(purple_train_speed);
    }
    pthread_exit(0);
}

// Trilhos do trem vermelho: L8 -> L9 -> L10* -> L5*
void* red_train_thread_function(void*){
    sleep(1);
    while (1)
    {
        // Trem Vermelho: L5 -> L8
        printf("Trem Vermelho - Trilho L8 \n");
        red_x = 370;
        red_y = 10;
        sem_post(&sem_L5);
        sleep(red_train_speed);

        // Trem Vermelho: L8 -> L9
        printf("Trem Vermelho - Trilho L9 \n");
        red_x = 435;
        red_y = 73;
        sleep(red_train_speed);

        // Trem Vermelho: L9 -> L10
        sem_wait(&sem_L10);
        sem_wait(&sem_L5L6L10);
        sem_wait(&sem_L3L4L5L6L10);
        printf("Trem Vermelho - Trilho L10 \n");
        red_x = 370;
        red_y = 134;
        sleep(red_train_speed);

        // Trem Vermelho: L10 -> L5
        sem_wait(&sem_L5);
        printf("Trem Vermelho - Trilho L5 \n");
        red_x = 299;
        red_y = 73;
        sem_post(&sem_L3L4L5L6L10);
        sem_post(&sem_L5L6L10);
        sem_post(&sem_L10);
        sleep(red_train_speed);
    }
    pthread_exit(0);
}

// Trilhos do trem azul: L13 -> L11 -> L4* -> L6* -> L10* -> L12
void* blue_train_thread_function(void*){
    sleep(1);
    while (1)
    {

        // Trem Azul: L12 -> L13
        printf("Trem Azul - Trilho L13 \n");
        blue_x = 230;
        blue_y = 254;
        sleep(blue_train_speed);

        // Trem Azul: L13 -> L11
        printf("Trem Azul - Trilho L11 \n");
        blue_x = 25;
        blue_y = 199;
        sleep(blue_train_speed);

        // Trem Azul: L11 -> L4
        sem_wait(&sem_L4);
        sem_wait(&sem_L3L4L6);
        sem_wait(&sem_L3L4L5L6L10);
        printf("Trem Azul - Trilho L4 \n");
        blue_x = 100;
        blue_y = 138;
        sleep(blue_train_speed);

        // Trem Azul: L4 -> L6
        sem_wait(&sem_L6);
        sem_wait(&sem_L5L6L10);
        printf("Trem Azul - Trilho L6 \n");
        blue_x = 230;
        blue_y = 138;
        sem_post(&sem_L3L4L6);
        sem_post(&sem_L4);
        sleep(blue_train_speed);

        // Trem Azul: L6 -> L10
        sem_wait(&sem_L10);
        printf("Trem Azul - Trilho L10 \n");
        blue_x = 364;
        blue_y = 138;
        sem_post(&sem_L5L6L10);
        sem_post(&sem_L6);
        sleep(blue_train_speed);

        // Trem Azul: L10 -> L12
        sem_post(&sem_L3L4L5L6L10);
        sem_post(&sem_L10);
        printf("Trem Azul - Trilho L12 \n");
        blue_x = 435;
        blue_y = 199;
        sleep(blue_train_speed);
    }
    pthread_exit(0);
}

// UI
void* speeds_thread_function(void*){
    while(1)
    {
        blue_train_speed = BASE_SPEED - ui_aux.BlueSpeedSlider->value();
        green_train_speed = BASE_SPEED - ui_aux.GreenSpeedSlider->value();
        red_train_speed = BASE_SPEED - ui_aux.RedSpeedSlider->value();
        purple_train_speed = BASE_SPEED - ui_aux.PurpleSpeedSlider->value();

        usleep(100000); // A thread dorme por 0,1 segundo

    }
}
void* rails_thread_function(void*){
    while(1)
    {
        ui_aux.GreenTrain->setGeometry(green_x,green_y,30,30);
        ui_aux.BlueTrain->setGeometry(blue_x,blue_y,30,30);
        ui_aux.PurpleTrain->setGeometry(purple_x,purple_y,30,30);
        ui_aux.RedTrain->setGeometry(red_x,red_y,30,30);
        usleep(100000); // A thread_interface dorme por 0,1 segundo
    }
}


// Criação das Threads
void MainWindow::on_StartTrains_triggered()
{
    ui->BlueTrain->setVisible(true);
    ui->GreenTrain->setVisible(true);
    ui->RedTrain->setVisible(true);
    ui->PurpleTrain->setVisible(true);

    // Inicializando semáforos

    res = sem_init(&sem_L3L4L5L6L10, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L3L4L5L6L10. \n");

    res = sem_init(&sem_L5L6L10, 0, 2);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L5L6L10. \n");

    res = sem_init(&sem_L3L4L6, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L3L4L6. \n");

    res = sem_init(&sem_L10, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L10. \n");

    res = sem_init(&sem_L6, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L6. \n");

    res = sem_init(&sem_L5, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L5. \n");

    res = sem_init(&sem_L4, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L4. \n");

    res = sem_init(&sem_L3, 0, 3);
    if (res < 0)
    {
        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Semaforo sem_L3. \n");

//    init_semaphore(sem_L3L4L5L6L10, 3);
//    init_semaphore(sem_L5L6L10, 2);
//    init_semaphore(sem_L3L4L6, 2);
//    init_semaphore(sem_L10, 1);
//    init_semaphore(sem_L6, 1);
//    init_semaphore(sem_L5, 1);
//    init_semaphore(sem_L4, 1);
//    init_semaphore(sem_L3, 1);


    res = pthread_create(&blue_train_thread, NULL, blue_train_thread_function, NULL);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Criação da Thread Azul falhou");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Thread Azul \n");

    res = pthread_create(&green_train_thread, NULL, green_train_thread_function, NULL);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Criação da Thread Verde falhou");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Thread Verde \n");

    res = pthread_create(&purple_train_thread, NULL, purple_train_thread_function, NULL);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Criação da Thread Roxa falhou");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Thread Roxa \n");

    res = pthread_create(&red_train_thread, NULL, red_train_thread_function, NULL);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Criação da Thread Vermelha falhou");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Thread Vermelha \n");

    res = pthread_create(&speeds_thread, NULL, speeds_thread_function, NULL);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Criação da Thread das Velocidades falhou");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Thread das Velocidades \n");

    res = pthread_create(&rails_thread, NULL, rails_thread_function, NULL);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Criação da Thread dos Trilhos falhou");
        exit(EXIT_FAILURE);
    }
    printf("Criação de Thread dos Trilhos \n");


//    create_thread(blue_train_thread, blue_train_thread_function);
//    create_thread(green_train_thread, green_train_thread_function);
//    create_thread(red_train_thread, red_train_thread_function);
//    create_thread(purple_train_thread, purple_train_thread_function);
//    create_thread(speeds_thread, MainWindow::speeds_thread_function);
//    create_thread(rails_thread, MainWindow::rails_thread_function);
}

//void MainWindow::init_semaphore(sem_t sem, unsigned int value){
//    res = sem_init(&sem, 0, value);
//    if (res < 0)
//    {
//        QMessageBox::warning(this,"ERRO","Semaphore initialization failed");
//        exit(EXIT_FAILURE);
//    }
//    printf("Criação de Semaforo. \n");
//}

//void MainWindow::create_thread(pthread_t /* *__restrict */ thread, void thread_function){
//    res = pthread_create(&thread, NULL, thread_function, NULL);
//    if (res != 0)
//    {
//        QMessageBox::warning(this,"ERRO","Criação da Thread falhou");
//        exit(EXIT_FAILURE);
//    }
//    printf("Criação de Thread \n");
//}


// Finalização das Threads
void MainWindow::on_StopTrains_triggered()
{
    join_thread(blue_train_thread);
    join_thread(green_train_thread);
    join_thread(red_train_thread);
    join_thread(purple_train_thread);
    join_thread(speeds_thread);
    join_thread(rails_thread);

    sem_destroy(&sem_L3);
    sem_destroy(&sem_L4);
    sem_destroy(&sem_L5);
    sem_destroy(&sem_L6);
    sem_destroy(&sem_L10);
    sem_destroy(&sem_L3L4L6);
    sem_destroy(&sem_L5L6L10);
    sem_destroy(&sem_L3L4L5L6L10);
    exit(EXIT_SUCCESS);

    QApplication::quit();
}

void MainWindow::join_thread(pthread_t thread){
    res = pthread_join(thread, &thread_result);
    if (res != 0)
    {
        QMessageBox::warning(this,"ERRO","Junção da Thread falhou");
        exit(EXIT_FAILURE);
    }
    printf("MAIN() --> Thread foi juntada com sucesso\n");
}
