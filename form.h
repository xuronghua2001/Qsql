#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
  Q_OBJECT

      public:
               explicit Form(QWidget *parent = nullptr);
  ~Form();

             private slots:
               void on_pushButton_clicked();

               void on_pushButton_2_clicked();

             private:
               int login();
  Ui::Form *ui;
             signals:
  void mySignal(QString);
};

#endif // FORM_H
