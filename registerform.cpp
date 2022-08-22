#include "registerform.h"
#include "ui_registerform.h"

QString id_s;
registerForm::registerForm(QWidget *parent) :
                                              QWidget(parent),
                                              ui(new Ui::registerForm)
{
  ui->setupUi(this);
}

registerForm::~registerForm()
{
  delete ui;
}
void registerForm::init(QSqlDatabase db)
{
  if(QSqlDatabase::contains("qt_sql_default_connection"))
    db = QSqlDatabase::database("qt_sql_default_connection");
  else
  {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setDatabaseName("a");  //设置数据源名
    db.setUserName("sa");
    db.setPassword("9876543210");
    db.open();
  }
}
void registerForm::on_pushButton_2_clicked()
{
  QSqlDatabase db;
  QSqlQuery query(db);
  int n=0;
  QHash <QString,QString>customer;
  QString name,id,tel,c_id;
  name=ui->lineEdit->text();
  id=ui->lineEdit_2->text();
  tel=ui->lineEdit_3->text();
  if(!name.isEmpty()&&!id.isEmpty()&&!tel.isEmpty()){
    if(ui->label_2->text()=="姓名：    "){
  customer.insert("name",name);
  customer.insert("id",id);
  customer.insert("tel",tel);
  query.exec("select * from Customers");
  while(query.next())
  {
    c_id=query.value("id").toString();qDebug()<<c_id;
    if(c_id==id)
    {n=1;qDebug()<<n;break;}
  }
  if(n)
    QMessageBox::information(nullptr,"提示","该身份证号已注册，不能重复注册！");
  else
  {
    QHash<QString, QString>::iterator i,j,k;
    i=customer.find("id");
    j=customer.find("name");
    k=customer.find("tel");
    query.prepare("INSERT INTO Customers (id, name, tel) "
                  "VALUES (:id, :name, :tel)");
    query.bindValue(":id", i.value());
    query.bindValue(":name", j.value());
    query.bindValue(":tel", k.value());

    query.exec();
    QMessageBox::information(nullptr,"提示","注册成功！");
  }}
    else
    {  query.exec("select * from Customers");
      while(query.next())
      {
        c_id=query.value(0).toString();
        if(c_id==id_s)
        {n=1;qDebug()<<n<<c_id;break;}
      }
      if(n==1){
        QString str="update Customers set gender=?,age=?,address=? where id='"+id_s+"'";
        query.prepare(str);
        query.addBindValue(ui->lineEdit->text());
        query.addBindValue(ui->lineEdit_2->text());
        query.addBindValue(ui->lineEdit_3->text());
        qDebug()<<query.exec();
        QMessageBox::information(nullptr,"提示",id_s+"信息添加成功！");
        }
      else
      {ui->lineEdit->clear();ui->lineEdit_2->clear();ui->lineEdit_3->clear();QMessageBox::information(nullptr,"提示","还未注册该乘客！");}
    }
  }
  else
  {
      ui->label_2->setText("姓名：");
      ui->label_3->setText("身份证：");
      ui->label_4->setText("电话：");
    QMessageBox::information(nullptr,"提示","信息不能为空！");}


}


void registerForm::on_pushButton_clicked()
{
  id_s=ui->lineEdit_2->text();
  if(!id_s.isEmpty())
  {
    if(!ui->lineEdit->text().isEmpty()&&!ui->lineEdit_3->text().isEmpty())
  ui->lineEdit->clear();
  ui->lineEdit_2->clear();
  ui->lineEdit_3->clear();
  ui->label_2->setText("性别：");
  ui->label_3->setText("年龄：");
  ui->label_4->setText("地址：");
  }
  else
   QMessageBox::information(nullptr,"提示","信息不能为空！");
}

