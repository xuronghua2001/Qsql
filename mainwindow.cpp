#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
  f.show();
  ui->setupUi(this);
  connect(&f,&Form::mySignal,this,&MainWindow::changeSubWindow);
}
void MainWindow::init(QSqlDatabase db)
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
void MainWindow::changeSubWindow(QString s)
{
  f.hide();
  QSqlDatabase db;
  init(db);
  QSqlQuery query(db);
  query.exec("select * from Users");
  QString level;
  while(query.next())
  {
    QString name=query.value(0).toString();
    level=query.value(2).toString();
    if(s==name)
    {qDebug()<<level;break;}
  }
  QString status;
  qDebug()<<this->statusBar();
  if(level=='0')
  {ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    status="管理员模式";l=0;}
  else
  {ui->pushButton_6->hide();status="客户模式";}
  QStatusBar *pStatusBar=statusBar();
  QLabel *pLabel=new QLabel(this);
  pLabel->setText(status);
  pStatusBar->addWidget(pLabel);
  ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
  this->show();
  ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(Click(QTableWidgetItem*)));
}
void MainWindow::Click(QTableWidgetItem* item)
{
  int r=item->row();
  QString airline=ui->tableWidget->item(r,3)->data(0).toString();
  QString flight=ui->tableWidget->item(r,4)->data(0).toString();
  ui->lineEdit_4->setText(airline);
  ui->lineEdit_5->setText(flight);
}
void MainWindow::Click_2(QTableWidgetItem* item)
{
  int r=item->row();
  QString id=ui->tableWidget_2->item(r,0)->data(0).toString();
  QString airline=ui->tableWidget_2->item(r,4)->data(0).toString();
  QString flight=ui->tableWidget_2->item(r,3)->data(0).toString();
  ui->lineEdit_3->setText(id);
  ui->lineEdit_4->setText(airline);
  ui->lineEdit_5->setText(flight);
}
MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_pushButton_clicked()
{
  ui->tableWidget->clear();
  QSqlDatabase db;
  init(db);
  QSqlQuery query(db);
  QString str;
  if(ui->lineEdit->text().isEmpty()&&ui->lineEdit_2->text().isEmpty())
  str="select * from Airline,flightInformation where Airline.airlineNo=flightInformation.airlineNo";
  else
  {
  QString start="'"+ui->lineEdit->text()+"'";
  QString end="'"+ui->lineEdit_2->text()+"'";
  str="select * from Airline,flightInformation where Airline.airlineNo=flightInformation.airlineNo and departure="+start+" and destination="+end;
  }
  query.exec(str);
  QString departure,destination,date,o,remain,f,p,q,d,a,company;
  int i=0;
  ui->tableWidget->setColumnCount(11);
  QStringList list;
  list<<"起点"<<"终点"<<"日期"<<"航线号"<<"航班号"<<"余票量"<<"航空公司"<<"票价"<<"飞机型号"<<"起飞时间"<<"到达时间";
  ui->tableWidget->setHorizontalHeaderLabels(list);
  while(query.next())
  {
    company=query.value("airCompany").toString();
    departure=query.value("departure").toString();
    destination=query.value("destination").toString();
    date=query.value("date").toString();
    remain=query.value("remain").toString();
    o=query.value("model").toString();
    f=query.value("flightNo").toString();
    p=query.value("price").toString();
    q=query.value("airlineNo").toString();
    d=query.value("departureTime").toTime().toString();
    a=query.value("arrivalTime").toTime().toString();
    if(query.value("date").toDate()>ui->dateTimeEdit->dateTime().date()){
    i++;ui->tableWidget->setRowCount(i);
    ui->tableWidget->setItem(i-1,0,new QTableWidgetItem(departure));
    ui->tableWidget->setItem(i-1,1,new QTableWidgetItem(destination));
    ui->tableWidget->setItem(i-1,2,new QTableWidgetItem(date));
    ui->tableWidget->setItem(i-1,3,new QTableWidgetItem(q));
    ui->tableWidget->setItem(i-1,4,new QTableWidgetItem(f));
    ui->tableWidget->setItem(i-1,5,new QTableWidgetItem(remain));
    ui->tableWidget->setItem(i-1,6,new QTableWidgetItem(company));
    ui->tableWidget->setItem(i-1,7,new QTableWidgetItem(p));
    ui->tableWidget->setItem(i-1,8,new QTableWidgetItem(o));
    ui->tableWidget->setItem(i-1,9,new QTableWidgetItem(d));
    ui->tableWidget->setItem(i-1,10,new QTableWidgetItem(a));}
  }

}


void MainWindow::on_pushButton_2_clicked()
{
  QString id=ui->lineEdit_3->text();
  QSqlDatabase db;
  init(db);
  QSqlQuery query(db);
  QString str;
  str="select id from Customers";
  query.exec(str);
  int i=0;
  while(query.next())
  {
    QString n=query.value(0).toString();
    if(n==id)
    {qDebug()<<n;i=1;break;}
  }
  if(i==1){
    query.prepare("exec real_price ?,?,?,? output");
    query.bindValue(0,ui->lineEdit_4->text());
    query.bindValue(1,ui->lineEdit_5->text());
    query.bindValue(2,id);
    query.bindValue(3,0,QSql::Out);
    query.exec();
    QString realPice=query.boundValue(3).toString();
    query.prepare("exec insert_booking ?,?,?,?,? output");
    query.bindValue(0,id);
    query.bindValue(1,realPice);
    query.bindValue(2,ui->lineEdit_5->text());
    query.bindValue(3,ui->lineEdit_4->text());
    query.bindValue(4,0,QSql::Out);
    qDebug()<<query.exec()<<query.boundValue(4);
    if(query.boundValue(4)==0)
    {
      on_pushButton_4_clicked();on_pushButton_clicked();
      QMessageBox::information(nullptr,"提示",id+"购票成功！");
    }
    else if(query.boundValue(4)==2)
      QMessageBox::information(nullptr,"提示",id+"购票失败，余票不足！");
    else
      QMessageBox::information(nullptr,"提示",id+"购票失败，该乘客已经预定本次航班！");
  }
  else
    QMessageBox::information(nullptr,"提示",id+"身份证号不存在，请先注册该乘客！");
}


void MainWindow::on_pushButton_5_clicked()
{
    r.show();
}


void MainWindow::on_pushButton_4_clicked()
{
  ui->tableWidget_2->clear();
  QSqlDatabase db;
  init(db);
  QSqlQuery query(db);
  QString str;
  str="select * from v_c";
  qDebug()<<query.exec(str);
  int i=0;
  ui->tableWidget_2->setColumnCount(6);
  QStringList list;
  list<<"身份证号"<<"座位号"<<"实际票价"<<"航班号"<<"航线号"<<"日期";
  ui->tableWidget_2->setHorizontalHeaderLabels(list);
  while(query.next())
  {
    QString id=query.value("id").toString();
    QString seatNo=query.value("seatNo").toString();
    QString ap=query.value("actualPrice").toString();
    QString flightNo=query.value("flightNo").toString();
    QString airlineNo=query.value("airlineNo").toString();
    QString date=query.value("date").toString();
    qDebug()<<id<<seatNo<<flightNo<<airlineNo;
    if(l==0){//权限为管理员时可以查询所有订票信息
      i++;ui->tableWidget_2->setRowCount(i);
    ui->tableWidget_2->setItem(i-1,0,new QTableWidgetItem(id));
    ui->tableWidget_2->setItem(i-1,1,new QTableWidgetItem(seatNo));
    ui->tableWidget_2->setItem(i-1,2,new QTableWidgetItem(ap));
    ui->tableWidget_2->setItem(i-1,3,new QTableWidgetItem(flightNo));
    ui->tableWidget_2->setItem(i-1,4,new QTableWidgetItem(airlineNo));
    ui->tableWidget_2->setItem(i-1,5,new QTableWidgetItem(date));}
    else
      if(id==ui->lineEdit_3->text())//权限为客户时只允许查询输入身份的订票信息
    {i++;ui->tableWidget_2->setRowCount(i);
      ui->tableWidget_2->setItem(i-1,0,new QTableWidgetItem(id));
      ui->tableWidget_2->setItem(i-1,1,new QTableWidgetItem(seatNo));
      ui->tableWidget_2->setItem(i-1,2,new QTableWidgetItem(ap));
      ui->tableWidget_2->setItem(i-1,3,new QTableWidgetItem(flightNo));
      ui->tableWidget_2->setItem(i-1,4,new QTableWidgetItem(airlineNo));
      ui->tableWidget_2->setItem(i-1,5,new QTableWidgetItem(date));
    }
  }
  connect(ui->tableWidget_2,SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(Click_2(QTableWidgetItem*)));
}


void MainWindow::on_pushButton_3_clicked()
{
    QString id=ui->lineEdit_3->text();
    QString f_no=ui->lineEdit_5->text();
    QString a_no=ui->lineEdit_4->text();
    if(!id.isEmpty()&&!f_no.isEmpty()&&!a_no.isEmpty())
    {
    QSqlDatabase db;
    init(db);
    QSqlQuery query(db);
    QString str,s_no;
    s_no=f_no+"' and airlineNo='"+a_no+"'";
    str="select id from booking where flightNo='"+s_no;
    query.exec(str);
    int i=0;
    while(query.next())
    {
      QString n=query.value("id").toString();
      if(n==id)
      {qDebug()<<n;i=1;break;}
    }
    if(i)
    {
      int r_ticket;
      QString airCompany,distance;
      query.exec("select * from Airline where airlineNo='"+a_no+"'");
      query.first();
      airCompany=query.value("airCompany").toString();
      distance=query.value("distance").toString();
      qDebug()<<distance;
      distance=distance+"' where id='"+id+"' and airCompany='"+airCompany+"'";
      query.exec("update customer_distance set distance=distance-'"+distance);
      id="'"+id+"' and flightNo='"+f_no+"' and airlineNo='"+a_no+"'";
      query.exec("delete from booking where id="+id);
      query.exec("select * from flightInformation where flightNo='"+s_no);
      query.first();
      r_ticket=query.value("remain").toInt();
      r_ticket++;
      qDebug()<<r_ticket;
      qDebug()<<query.exec("update flightInformation set remain='"+QString::number(r_ticket)+"' where flightNo='"+s_no);
      on_pushButton_4_clicked();on_pushButton_clicked();
      QMessageBox::information(nullptr,"提示","退票已成功！");
    }
    else
      QMessageBox::information(nullptr,"提示","未查找到该乘客定票！");
    }
    else
      QMessageBox::information(nullptr,"提示","未输入完整信息！");
}


void MainWindow::on_pushButton_6_clicked()
{
  QSqlDatabase db;
  init(db);
  QSqlQuery query(db);
  QString str;
  str="select * from Customers";
  query.exec(str);
  QStringList list;
  list<<"身份证号"<<"姓名"<<"性别"<<"手机电话"<<"年龄"<<"地址";
  ui->tableWidget_2->setColumnCount(6);
  ui->tableWidget_2->setHorizontalHeaderLabels(list);
  int i=0;
  while(query.next())
  {
    QString id=query.value("id").toString();
    QString name=query.value("name").toString();
    QString gender=query.value("gender").toString();
    QString tel=query.value("tel").toString();
    QString age=query.value("age").toString();
    QString address=query.value("address").toString();
    i++;ui->tableWidget_2->setRowCount(i);
    ui->tableWidget_2->setItem(i-1,0,new QTableWidgetItem(id));
    ui->tableWidget_2->setItem(i-1,1,new QTableWidgetItem(name));
    ui->tableWidget_2->setItem(i-1,2,new QTableWidgetItem(gender));
    ui->tableWidget_2->setItem(i-1,3,new QTableWidgetItem(tel));
    ui->tableWidget_2->setItem(i-1,4,new QTableWidgetItem(age));
    ui->tableWidget_2->setItem(i-1,5,new QTableWidgetItem(address));
  }
  disconnect(ui->tableWidget_2,SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(Click_2(QTableWidgetItem*)));
}

