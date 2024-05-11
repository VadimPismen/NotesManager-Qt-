#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <noteeditor.h>
#include <QDir>
#include <QInputDialog>
#include <newnoteeditor.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ListOfNotes_itemDoubleClicked(QListWidgetItem *item);
    void needtoupdate();

    void on_createnewnote_clicked();

    void on_DeleteNote_clicked();

    void on_ListOfNotes_itemClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
//    NoteEditor *window;
    void updatelist();
};
#endif // MAINWINDOW_H
