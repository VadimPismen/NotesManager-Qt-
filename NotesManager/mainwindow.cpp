#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updatelist();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Обновление списка заметок.
void MainWindow::updatelist(){
    ui->DeleteNote->setEnabled(false);
    ui->ListOfNotes->clear();
    QDir dir("./notes");
    dir.setNameFilters(QStringList("*.txt"));
    dir.setSorting(QDir::Time); // Используется сортировка по времени (дата изменения). Более поздние заметки появятся сверху.
    for (const QString &filename : dir.entryList(QDir::Files))
         ui->ListOfNotes->addItem(filename.chopped(4));
}

// Двойной клик по заметке
void MainWindow::on_ListOfNotes_itemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return; // Если есть выделенная заметка...
    while(true){
        QString note = item->text();
        QFile file("./notes/" + note + ".txt");
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this, "Ошибка", "Файл недоступен.");
            updatelist();
            return;
        }
        bool ok;
        QByteArray key = QInputDialog::getText(this, " ",
                                             tr("Введите пароль:"), QLineEdit::Password,
                                             "", &ok, Qt::Dialog).toUtf8();
        if (ok && !key.isEmpty()) // Подтверждаем пароль.
        {
            QFile file("./notes/" + note + ".txt");
            if (!file.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this, "Ошибка", "Файл недоступен.");
                return;
            }
            ushort checksum = 0; // Старая добрая сумма кодов символов пароля.
            for (int i = 0; i < key.length(); i++){
                checksum += key[i];
            }
            QByteArray checkedkey = ""; // На этот раз я оперирую массивами байтов, а не строками. Так не исчезнут нетекстовые символы.
            checkedkey += checksum;
            int checksumsize = checkedkey.size();
            checkedkey += key;
            for (int i = checksumsize; i < checkedkey.length(); i++)
                checkedkey[i]= checkedkey[i] ^ key[i % key.length()];

            QByteArray checkstring = file.read(checkedkey.length());
            file.close();
            if (checkstring == checkedkey){
                NoteEditor *window = new NoteEditor(this, note, key, checkedkey);
                window->show();
                connect(window, &NoteEditor::finished, this, &MainWindow::needtoupdate);
                return;
            }
        }
        else{
            updatelist();
            return;
        }
    }
}

// Событие, вызываемое при закрытии дочерних окон.
void MainWindow::needtoupdate()
{
    updatelist();
}

// Клик по кнопке создания новой заметки.
void MainWindow::on_createnewnote_clicked()
{
    newnoteeditor *window = new newnoteeditor(this);
    window->show();
    connect(window, &newnoteeditor::finished, this, &MainWindow::needtoupdate); // Возможность получения события закрытия дочернего окна.
    return;
}

// Удаление заметки.
void MainWindow::on_DeleteNote_clicked()
{
    if (ui->ListOfNotes->currentItem()){ // Проверяем выделение заметки.
        QString filename = ui->ListOfNotes->currentItem()->text(); // Имя выделенной заметки.
        if (QFile::exists("./notes/" + filename + ".txt")){
            if (QMessageBox::question(this, " ", "Удалить заметку?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
                QFile file("./notes/" + filename + ".txt");
                if (!file.remove()){
                    QMessageBox::warning(this, "Ошибка", "Ошибка удаления.");
                }
                file.close();
            }
        }
        else{
            QMessageBox::warning(this, "Ошибка", "Заметка уже удалена.");
        }
    }
    updatelist();
}

// Включаем кнопку удаления заметки, если какая-нибудь выделена.
void MainWindow::on_ListOfNotes_itemClicked(QListWidgetItem *item)
{
    ui->DeleteNote->setEnabled(true);
}


void MainWindow::on_pushButton_clicked()
{
    updatelist();
}

