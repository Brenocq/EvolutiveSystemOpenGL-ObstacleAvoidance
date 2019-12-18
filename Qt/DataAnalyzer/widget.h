#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_importButton_clicked();

    void on_envComboBox_currentIndexChanged(int index);

    void on_genComboBox_currentIndexChanged(int index);

    void on_repComboBox_currentIndexChanged(int index);

    void callUpdate();

    void on_allVisibleButton_clicked();

    void on_allInvisibleButton_clicked();

    void on_visibleCheckBox_clicked();

    void on_visibleCheckBox_stateChanged(int arg1);

    void on_selRobotComboBox_currentIndexChanged(int index);

private:
    void updatePlot();
    void getRobotsFitness(int gen, int env, int rep, QVector<QVector<double> >&fitness, QVector<QVector<double> >&meanFitness);
    void getRobotsSensors(int gen, int env, int rep, QVector<QVector<double> >&qtdSensors);
    Ui::Widget *ui;
    QString _fileName;
    QTimer *timer;

    QVector<bool> _visibleRobots;
    int _sizeEnv, _sizeGen, _sizeRep;
    int _qtdSensors;
    float _enableSensor;
    float _maxTime;
};

#endif // WIDGET_H
