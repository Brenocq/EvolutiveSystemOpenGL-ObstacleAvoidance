#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QtMath>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    _fileName = "";
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(callUpdate()));
    timer->start(1000);

    _visibleRobots.resize(0);

    _sizeEnv = -1;
    _sizeGen = -1;
    _sizeRep = -1;
    _qtdSensors = -1;
    _enableSensor = -1;
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

    line = in.readLine();
    fields = line.split(" ");
    _qtdSensors = fields.back().toInt();
    line = in.readLine();
    fields = line.split(" ");
    _enableSensor = fields.back().toFloat();

    file.close();

    //----- Update combobox -----//
    QStringList itemList;
    int qtdEnv = ui->qtdEnvEdit->text().toInt();
    for(int i=0;i<qtdEnv;i++){
        itemList += QString::number(i);
    }

    if(_sizeEnv != itemList.size()){
        _sizeEnv = itemList.size();
        ui->envComboBox->clear();
        ui->envComboBox->addItems(itemList);
        ui->genComboBox->setEnabled(true);
    }
    //----- Update visible -----//
    QStringList visibleList;
    int qtdRbt = ui->qtdRobotsEdit->text().toInt();
     _visibleRobots.resize(qtdRbt);
     for(int i=0;i<qtdRbt;i++){
         visibleList += QString::number(i);
         _visibleRobots[i]=true;
     }

     ui->selRobotComboBox->clear();
     ui->selRobotComboBox->addItems(visibleList);
     ui->selRobotComboBox->setEnabled(true);
     ui->allVisibleButton->setEnabled(true);
     ui->allInvisibleButton->setEnabled(true);
     ui->visibleCheckBox->setEnabled(true);
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
    if(_sizeGen != itemList.size()){
        _sizeGen = itemList.size();
        ui->genComboBox->clear();
        ui->genComboBox->addItems(itemList);
        ui->genComboBox->setEnabled(qtdGen>0?true:false);
        ui->repComboBox->setEnabled(qtdGen>0?true:false);
    }
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
               //qDebug() << "currEnv "<<currEnv;
               if(currEnv == ui->envComboBox->currentIndex()){
                   currGen++;
               }
            }

            cmp = QString::compare(fields[0], "\tRepetition", Qt::CaseInsensitive);
            if(cmp==0){
               currRep = fields[1].toInt();
               //qDebug() << "    currRep "<<currRep;

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
    //qDebug() << "_sizeRep" <<_sizeRep <<" -> " << itemList.size();
    if(_sizeRep != itemList.size()){
        _sizeRep = itemList.size();
        ui->repComboBox->clear();
        ui->repComboBox->addItems(itemList);
        ui->repComboBox->setEnabled(qtdRep>0?true:false);
    }
    updatePlot();
}

void Widget::updatePlot()
{
    // generate some data:
    QVector<QVector<double> > fitness, meanFitness, qtdSensors; // initialize with entries 0..100
    QVector<double> generation;
    getRobotsFitness(ui->genComboBox->currentIndex(), ui->envComboBox->currentIndex(), ui->repComboBox->currentIndex(), fitness, meanFitness);
    getRobotsSensors(ui->genComboBox->currentIndex(), ui->envComboBox->currentIndex(), ui->repComboBox->currentIndex(), qtdSensors);

    for (int i=0;i<fitness[0].size();i++) {
        generation.push_back(i);
    }

    int qtdRobots = ui->qtdRobotsEdit->text().toInt();
    //---------- Calculate mean lines ----------//
    QVector<double> grossLine;
    for(int gen=0;gen<generation.size(); gen++){
        double mean=0;
        for(int robot=0;robot<qtdRobots;robot++)
           mean+=fitness[robot][gen];
        mean/=qtdRobots;
        grossLine.push_back(mean);
    }

    QVector<double> meanLine;
    for(int gen=0;gen<generation.size(); gen++){
        double mean=0;
        for(int robot=0;robot<qtdRobots;robot++)
           mean+=meanFitness[robot][gen];
        mean/=qtdRobots;
        meanLine.push_back(mean);
    }

    QVector<double> meanSensors;
    for(int gen=0;gen<generation.size(); gen++){
        double mean=0;
        for(int robot=0;robot<qtdRobots;robot++)
           mean+=qtdSensors[robot][gen];
        mean/=qtdRobots;
        meanSensors.push_back(mean);
    }

    //---------- Set visible robots ----------//
    for (int i=0;i<qtdRobots && i<_visibleRobots.size();i++) {
        if(!_visibleRobots[i]){
            for (int j=0;j<fitness[0].size();j++) {
                fitness[i][j]=-10000;
                meanFitness[i][j]=-10000;
                qtdSensors[i][j]=-1;
            }
        }
    }
    //---------- Update gross graph ----------//
    // create graph and assign data to it

    for (int i=1;i<=qtdRobots;i++) {
        ui->graphGross->addGraph();
        ui->graphGross->graph(i-1)->setPen(QPen(QColor(int(i%2)*255,int((i/2)%2)*255,int((i/4)%2)*255,255 )));
        ui->graphGross->graph(i-1)->setData(generation, fitness[i-1]);
    }

    // give the axes some labels
    ui->graphGross->xAxis->setLabel("Generation");
    ui->graphGross->yAxis->setLabel("Fitness");

    // set axes ranges, so we see all data
    ui->graphGross->xAxis->setRange(0, fitness[0].size());
    //ui->graphGross->xAxis->setRange(0, 50);
    ui->graphGross->yAxis->setRange(-2000, 2000);
    ui->graphGross->replot();

    //---------- Update mean graph ----------//
    // create graph and assign data to it
    for (int i=1;i<=qtdRobots;i++) {
        ui->graphMean->addGraph();
        ui->graphMean->graph(i-1)->setPen(QPen(QColor(int(i%2)*255,int((i/2)%2)*255,int((i/4)%2)*255,255 )));
        ui->graphMean->graph(i-1)->setData(generation, meanFitness[i-1]);
    }

    //---------- Plot mean line (gross graph) ----------//
    ui->graphGross->addGraph();
    ui->graphGross->graph(qtdRobots)->setPen(QPen(QColor(255,0,0,255),2));

    ui->graphGross->graph(qtdRobots)->setData(generation, grossLine);

    //---------- Plot mean line (mean graph) ----------//
    ui->graphMean->addGraph();
    ui->graphMean->graph(qtdRobots)->setPen(QPen(QColor(255,0,0,255),2));



    ui->graphMean->graph(qtdRobots)->setData(generation, meanLine);

    // give the axes some labels
    ui->graphMean->xAxis->setLabel("Generation");
    ui->graphMean->yAxis->setLabel("Fitness");
    // set axes ranges, so we see all data
    ui->graphMean->xAxis->setRange(0, meanFitness[0].size());
    //ui->graphMean->xAxis->setRange(0, 50);
    ui->graphMean->yAxis->setRange(-2000, 2000);
    ui->graphMean->replot();

    //---------- Plot number of sensors ----------//
    // create graph and assign data to it
    for (int i=1;i<=qtdRobots;i++) {
        ui->graphSensors->addGraph();
        ui->graphSensors->graph(i-1)->setPen(QPen(QColor(int(i%2)*255,int((i/2)%2)*255,int((i/4)%2)*255,255 )));
        ui->graphSensors->graph(i-1)->setData(generation, qtdSensors[i-1]);
    }

    // give the axes some labels
    ui->graphSensors->xAxis->setLabel("Generation");
    ui->graphSensors->yAxis->setLabel("Qtd sensors");

    // set axes ranges, so we see all data
    ui->graphSensors->xAxis->setRange(0, fitness[0].size());
    //ui->graphGross->xAxis->setRange(0, 50);
    ui->graphSensors->yAxis->setRange(0, 4);
    ui->graphSensors->replot();

    //---------- Plot mean line (sensors graph) ----------//
    ui->graphSensors->addGraph();
    ui->graphSensors->graph(qtdRobots)->setPen(QPen(QColor(255,0,0,255),2));

    ui->graphSensors->graph(qtdRobots)->setData(generation, meanSensors);
}

void Widget::getRobotsFitness(int gen, int env, int rep, QVector<QVector<double> >&fitness, QVector<QVector<double> >&meanFitness)
{
    int qtdRobots = ui->qtdRobotsEdit->text().toInt();
    fitness.clear();
    meanFitness.clear();
    fitness.resize(qtdRobots);
    meanFitness.resize(qtdRobots);

    //----- Get data from file -----//
    int currEnv=-1;
    int currGen=-1;
    int currRep=-1;
    int currPopNum=-1;

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
               //qDebug() << "currEnv "<<currEnv;
               if(currEnv == ui->envComboBox->currentIndex()){
                   currGen++;
               }
            }

            cmp = QString::compare(fields[0], "\tRepetition", Qt::CaseInsensitive);
            if(cmp==0){
               currRep = fields[1].toInt();
            }

            cmp = QString::compare(fields[0], "\t\tPopulation", Qt::CaseInsensitive);
            if(cmp==0){
               currPopNum = fields[1].toInt();

               if(currEnv == env && currGen==gen && currRep == rep && cmp==0){
                   for(int i=0; i<qtdRobots; i++){
                       QString robotLine = in.readLine();
                       QStringList robotFields = robotLine.split(" ");
                       //qDebug()<<"fitness "<< robotFields[3].toDouble()<<" mean:"<<robotFields[5].toDouble();
                       fitness[i].push_back(robotFields[3].toDouble());
                       meanFitness[i].push_back(robotFields[5].toDouble());
                   }
               }
            }
        }
    }
}

void Widget::getRobotsSensors(int gen, int env, int rep, QVector<QVector<double> >&qtdSensors){
    int qtdRobots = ui->qtdRobotsEdit->text().toInt();
    const int maxSensors = _qtdSensors==-1 ? 3 : _qtdSensors;
    const float enableValue =_enableSensor==-1 ? 0.4f : _enableSensor;

    //qDebug()<<"1:"<<maxSensors<<" 2:"<<enableValue<<endl;

    qtdSensors.clear();
    qtdSensors.resize(qtdRobots);

    //----- Get data from file -----//
    int currEnv=-1;
    int currGen=-1;
    int currRep=-1;
    int currPopNum=-1;

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
               //qDebug() << "currEnv "<<currEnv;
               if(currEnv == ui->envComboBox->currentIndex()){
                   currGen++;
               }
            }

            cmp = QString::compare(fields[0], "\tRepetition", Qt::CaseInsensitive);
            if(cmp==0){
               currRep = fields[1].toInt();
            }

            cmp = QString::compare(fields[0], "\t\tPopulation", Qt::CaseInsensitive);
            if(cmp==0){
               currPopNum = fields[1].toInt();

               if(currEnv == env && currGen==gen && currRep == rep && cmp==0){
                   for(int i=0; i<qtdRobots; i++){
                       int total = 0;
                       QString robotLine = in.readLine();
                       QStringList robotFields = robotLine.split(" ");

                       for(int i=0;i<maxSensors;i++){
                            total += (robotFields[11+i*2].toDouble())>=double(enableValue);
                       }

                       qtdSensors[i].push_back(total);
                   }
               }
            }
        }
    }
}


void Widget::on_repComboBox_currentIndexChanged(int index)
{
    updatePlot();
}

void Widget::callUpdate()
{
    if(ui->repComboBox->isEnabled()){
        //----- Update combobox -----//
        on_genComboBox_currentIndexChanged(ui->genComboBox->currentIndex());
        on_envComboBox_currentIndexChanged(ui->envComboBox->currentIndex());
        on_repComboBox_currentIndexChanged(ui->repComboBox->currentIndex());
        //----- Update plots -----//
        updatePlot();
    }
}

void Widget::on_allVisibleButton_clicked()
{
    int qtdRobots = ui->qtdRobotsEdit->text().toInt();
    for (int i=0;i<qtdRobots;i++) {
        _visibleRobots[i]=true;
    }
}

void Widget::on_allInvisibleButton_clicked()
{
    int qtdRobots = ui->qtdRobotsEdit->text().toInt();
    for (int i=0;i<qtdRobots;i++) {
        _visibleRobots[i]=false;
    }
}
void Widget::on_visibleCheckBox_stateChanged(int arg1)
{
    int selectedRobot = ui->selRobotComboBox->currentIndex();
    _visibleRobots[selectedRobot]= arg1==2? true : false;

    qDebug()<<_visibleRobots;

}

void Widget::on_selRobotComboBox_currentIndexChanged(int index)
{
    if(_visibleRobots[index])
        ui->visibleCheckBox->setChecked(1);
    else
        ui->visibleCheckBox->setChecked(0);
}

void Widget::on_visibleCheckBox_clicked()
{

}
