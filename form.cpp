#include "form.h"
#include "ui_form.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QHash>
#include <QMessageBox>
Form::Form(QWidget *parent) :
                              QWidget(parent),
                              ui(new Ui::Form)
{
  ui->setupUi(this);

}

Form::~Form()
{
  delete ui;
}
int Form::login()
{
  QSqlDatabase db;
  QHash <QString,QString>admin;
  if(QSqlDatabase::contains("qt_sql_default_connection"))
    db = QSqlDatabase::database("qt_sql_default_connection");
  else
  {db = QSqlDatabase::addDatabase("QODBC");
  db.setHostName("localhost");
  db.setDatabaseName("a");  //设置数据源名
  db.setUserName("sa");
  db.setPassword("9876543210");
  db.open();}
  QSqlQuery query(db);
  query.exec("select * from Users");
  while(query.next())
  {
    QString name=query.value(0).toString();
    QString password=query.value(1).toString();
    admin.insert(name,password);
  }
  db.close();
  if(admin.contains(ui->lineEdit->text()))
  {
  QHash<QString, QString>::iterator i;
  i = admin.find(ui->lineEdit->text());
  QString p=ui->lineEdit_2->text();
  if(p==i.value())
    return  1;
  else
    return 0;//0
  }
  else
  return 2;//2
}
void Form::on_pushButton_clicked()
{
  switch(login())
  {case 1:
  {emit mySignal(ui->lineEdit->text());
  break;}
  case 0:
  {QMessageBox::critical(nullptr,"警告","密码出错");
    break;}
  case 2:
  {QMessageBox::critical(nullptr,"警告","用户名出错");
    break;}
        }
}


void Form::on_pushButton_2_clicked()
{
  exit(0);
}

