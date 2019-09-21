#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFile>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    _fileName = "";
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_importButton_clicked()
{
    QString dataDir;

    //----- Find data directory -----//
    QString currDir = qApp->applicationDirPath();
    QStringList pieces = currDir.split( "/" );
    for(int i=0;i<pieces.length()-2;i++){
        dataDir+=pieces.value(i);
        dataDir+="/";
    }
    dataDir+="src/Data/";

    //----- Get file -----//
   _fileName = QFileDialog::getOpenFileName(this, "Select the data file", dataDir);

    if(_fileName.size()==0)
        return;

    QFile file(_fileName);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "error", file.errorString());
    }

    //----- Read file header -----//
    QTextStream in(&file);

    QString line;
    QStringList fields;

    fields = _fileName.split("/");
    ui->fileNameEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->qtdRobotsEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->qtdEnvEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->qtdRepEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->qtdObstEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->pointsColEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->pointsMovEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->maxPopEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->maxTimeEdit->setText(fields.back());
    line = in.readLine();
    fields = line.split(" ");
    ui->envMutEdit->setText(fields.back());

    file.close();

    //----- Update combobox -----//
    QStringList itemList;
    int qtdEnv = ui->qtdEnvEdit->text().toInt();
    for(int i=0;i<qtdEnv;i++){
        itemList += QString::number(i);
    }
    ui->envComboBox->clear();
    ui->envComboBox->addItems(itemList);
    ui->genComboBox->setEnabled(true);
}

void Widget::on_envComboBox_currentIndexChanged(int index)
{
    int qtdGen=0;
    QString startGenEnv = "Environment "+ QString::number(index)+"\t";

    //----- Search qtd generation -----//
    QFile file(_fileName);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "error", file.errorString());
    }
    QTextStream in(&file);

    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if(fields.size()>=2){
            int cmp = QString::compare(fields[0]+" "+fields[1], startGenEnv, Qt::CaseInsensitive);
            if(cmp==0){
                qtdGen++;
            }
        }
    }
    //----- Update combobox -----//
    QStringList itemList;
    for (int i=0;i<qtdGen;i++) {
        itemList += QString::number(i);

    }
    ui->genComboBox->clear();
    ui->genComboBox->addItems(itemList);
    ui->genComboBox->setEnabled(qtdGen>0?true:false);
    ui->repComboBox->setEnabled(qtdGen>0?true:false);

}

void Widget::on_genComboBox_currentIndexChanged(int index)
{
    int qtdRep=0;
    int currEnv=-1;
    int currGen=-1;
    int currRep=-1;

    //----- Search qtd generation -----//
    QFile file(_fileName);

    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "error", file.errorString());
    }
    QTextStream in(&file);

    while(!in.atEnd()){
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        if(fields.size()>=2){
            int cmp = QString::compare(fields[0], "Environment", Qt::CaseInsensitive);
            if(cmp==0){
               currEnv = fields[1].toInt();
               qDebug() << "currEnv "<<currEnv;
               if(currEnv == ui->envComboBox->currentIndex()){
                   currGen++;
               }
            }

            cmp = QString::compare(fields[0], "\tRepetition", Qt::CaseInsensitive);
            if(cmp==0){
               currRep = fields[1].toInt();
               qDebug() << "    currRep "<<currRep;

               if(currEnv == ui->envComboBox->currentIndex() && currGen == ui->genComboBox->currentIndex()){
                   qtdRep++;
               }
            }
        }
    }

    //----- Update combobox -----//
    QStringList itemList;
    for (int i=0;i<qtdRep;i++) {
        itemList += QString::number(i);

    }
    ui->repComboBox->clear();
    ui->repComboBox->addItems(itemList);
    ui->repComboBox->setEnabled(qtdRep>0?true:false);
    updatePlot();
}

void Widget::updatePlot()
{
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }

    //----- Update gross graph -----//
    // create graph and assign data to it
    ui->graphGross->addGraph();
    ui->graphGross->graph(0)->setPen(QPen(Qt::blue));
    ui->graphGross->graph(0)->setData(x, y);
    // give the axes some labels
    ui->graphGross->xAxis->setLabel("Generation");
    ui->graphGross->yAxis->setLabel("Fitness");

    // set axes ranges, so we see all data
    ui->graphGross->xAxis->setRange(-1, 1);
    ui->graphGross->yAxis->setRange(0, 1);
    ui->graphGross->replot();

    //----- Update mean graph -----//
    // create graph and assign data to it
    ui->graphMean->addGraph();
    ui->graphMean->graph(0)->setData(x, y);
    // give the axes some labels
    ui->graphMean->xAxis->setLabel("Generation");
    ui->graphMean->yAxis->setLabel("Fitness");
    // set axes ranges, so we see all data
    ui->graphMean->xAxis->setRange(-1, 1);
    ui->graphMean->yAxis->setRange(0, 1);
    ui->graphMean->replot();

}

QVector<QVector<float> > Widget::getRobotsFitness(int gen, int env, int rep)
{

}

