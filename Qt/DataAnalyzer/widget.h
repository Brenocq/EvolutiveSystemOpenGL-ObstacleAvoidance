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

private:
    void updatePlot();
    void getRobotsFitness(int gen, int env, int rep, QVector<QVector<double> >&fitness, QVector<QVector<double> >&meanFitness);
    Ui::Widget *ui;
    QString _fileName;
};

#endif // WIDGET_H
