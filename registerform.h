#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QHash>
#include <QMessageBox>

namespace Ui {
class registerForm;
}

class registerForm : public QWidget
{
  Q_OBJECT

      public:
               explicit registerForm(QWidget *parent = nullptr);
  ~registerForm();

             private slots:
               void on_pushButton_2_clicked();

               void on_pushButton_clicked();

             private:
               void  init(QSqlDatabase db);
  Ui::registerForm *ui;

};

#endif // REGISTERFORM_H
