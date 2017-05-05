#include "progressionform.h"
#include "ui_progressionform.h"

ProgressionForm::ProgressionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressionForm)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
    ui->pushButton->setEnabled(false);
}

ProgressionForm::~ProgressionForm()
{
    delete ui;
}

void ProgressionForm::on_pushButton_clicked()
{
    this->close();
}

void ProgressionForm::updateProgressionBar(int value) {
    ui->progressBar->setValue(value);
    if (value == 100) {
        ui->pushButton->setEnabled(true);
    }
}
