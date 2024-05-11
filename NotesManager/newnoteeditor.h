#ifndef NEWNOTEEDITOR_H
#define NEWNOTEEDITOR_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>

namespace Ui {
class newnoteeditor;
}

class newnoteeditor : public QDialog
{
    Q_OBJECT

public:
    explicit newnoteeditor(QWidget *parent = nullptr);
    ~newnoteeditor();

private slots:
    void on_SaveButton_clicked();

private:
    Ui::newnoteeditor *ui;
};

#endif // NEWNOTEEDITOR_H
