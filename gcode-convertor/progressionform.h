#ifndef PROGRESSIONFORM_H
#define PROGRESSIONFORM_H

#include <QWidget>

namespace Ui {
class ProgressionForm;
}

class ProgressionForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressionForm(QWidget *parent = 0);
    ~ProgressionForm();
    void updateProgressionBar(int value);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProgressionForm *ui;
};

#endif // PROGRESSIONFORM_H
