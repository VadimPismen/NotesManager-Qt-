#include "newnoteeditor.h"
#include "ui_newnoteeditor.h"

newnoteeditor::newnoteeditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newnoteeditor)
{
    ui->setupUi(this);
    ui->titleofnote->setText(QDateTime::currentDateTime().toString("dd-MM-yyyy HH-mm-ss")); // Новое имя заметки по умолчанию - текущее время.
}

newnoteeditor::~newnoteeditor()
{
    delete ui;
}

void newnoteeditor::on_SaveButton_clicked()
{
    QString localnote = ui->titleofnote->text().trimmed(); // Получение заголовка заметки, при этом лишние пробелы удаляются.
    if (localnote.size() == 0){
        QMessageBox::warning(this, "Ошибка", "Не задан заголовок.");
        return;
    }
    QRegExp rx("[\\.\\\\\\/\\^\\&\\|\\[\\]\"\\*\\>\\<:\\?\\+@!%]"); // Проверка на присутствие недопустимых символов: .\/^&|[]"*><:?+@!%
    if (localnote.contains(rx)){
        QMessageBox::warning(this, "Ошибка", "Неверное имя записи.");
        return;
    }
    QByteArray localkey, localkeyencr = "";
    localkey = ui->Password->text().toUtf8();
    if (localkey.length() == 0){
        QMessageBox::warning(this, "Ошибка", "Задан пустой пароль.");
        return;
    }
    if (QFile::exists("./notes/" + localnote + ".txt")){
        if (QMessageBox::question(this, " ", "Перезаписать заметку?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::No){
            return;
        }
    }
    QFile file("./notes/" + localnote + ".txt");
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Ошибка", "Ошибка записи.");
        return;
    }
    ushort checksum = 0;
    for (int i = 0; i < localkey.length(); i++){
        checksum += localkey[i];
    }
    localkeyencr += checksum;
    int checksumsize = localkeyencr.size();
    localkeyencr += localkey;
    for (int i = checksumsize; i < localkeyencr.length(); i++)
        localkeyencr[i]= localkeyencr[i] ^ localkey[i % localkey.length()]; // На этот раз не нужен костыль, так как пробел не кодируется как нулевой символ.
    QByteArray text = ui->TextEditor->toPlainText().toUtf8();
    file.write(localkeyencr);
    for (int i = 0; i < text.length(); i++){
        text[i] = text[i] ^ localkey[i % localkey.length()];
    }
    file.write(text);
    file.close();
    this->close();
}
