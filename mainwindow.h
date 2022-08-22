#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "form.h"
#include "registerform.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStatusBar>
#include <QLabel>
#include <QTableWidgetItem>
#include <QMessageBox>
QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
  Q_OBJECT

    public:
      Form f;
      registerForm r;
       MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

    private slots:
      void on_pushButton_clicked();
      void Click(QTableWidgetItem*);
      void Click_2(QTableWidgetItem*);
      void on_pushButton_2_clicked();
      void on_pushButton_5_clicked();
      void on_pushButton_4_clicked();

      void on_pushButton_3_clicked();

      void on_pushButton_6_clicked();

    private:
      QMenu *table_widget_menu;
       void changeSubWindow(QString s);
       void  init(QSqlDatabase db);
       int l=1;
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
