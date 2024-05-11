#ifndef NOTEEDITOR_H
#define NOTEEDITOR_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

namespace Ui {
class NoteEditor;
}

class NoteEditor : public QDialog
{
    Q_OBJECT

public:
    explicit NoteEditor(QWidget *parent = nullptr, QString note = "", QByteArray key = "", QByteArray keyencr = "");
    ~NoteEditor();

private slots:
    void on_SaveButton_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::NoteEditor *ui;
    QString note;
    QByteArray key;
    QByteArray keyencr;
};

#endif // NOTEEDITOR_H
