#include "noteeditor.h"
#include "ui_noteeditor.h"

NoteEditor::NoteEditor(QWidget *parent, QString _note, QByteArray _key, QByteArray _keyencr) : // Конструктор класса получает имя записи, пароль и его шифрованную версию.
    QDialog(parent),
    ui(new Ui::NoteEditor)
{
    note = _note;
    key = _key;
    keyencr = _keyencr;
    ui->setupUi(this);
    ui->titleofnote->setText(note);
    QFile file("./notes/" + note + ".txt");
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this, "Ошибка", "Файл недоступен.");
        this->close();
    }
    file.seek(keyencr.length());
    QByteArray data = file.readAll();
    for (int i = 0; i < data.length(); i++){
        data[i] = data[i] ^ key[i % key.length()];
    }
    file.close();
    ui->TextEditor->setPlainText(data);
}

NoteEditor::~NoteEditor()
{
    delete ui;
}

void NoteEditor::on_SaveButton_clicked()
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
    if (localnote != note){
        if (QFile::exists("./notes/" + localnote + ".txt")){
            if (QMessageBox::question(this, "Вопрос", "Перезаписать заметку?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::No){
                return;
            }
        }
    }
    QFile file("./notes/" + localnote + ".txt");
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Ошибка", "Ошибка записи.");
        return;
    }
    QByteArray localkey, localkeyencr = "";
    if (ui->checkBox->isChecked()){
        localkey = ui->NewPassword->text().toUtf8();
        if (localkey.length() == 0){
            QMessageBox::warning(this, "Ошибка", "Задан пустой пароль.");
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
            localkeyencr[i]= localkeyencr[i] ^ localkey[i % localkey.length()];
    }
    else{
        localkey = key;
        localkeyencr = keyencr;
    }
    QByteArray text = ui->TextEditor->toPlainText().toUtf8();
    file.write(localkeyencr);
    for (int i = 0; i < text.length(); i++){
        text[i] = text[i] ^ localkey[i % localkey.length()];
    }
    file.write(text);
    file.close();
    if (localnote != note){
        QFile file("./notes/" + note + ".txt");
        file.remove();
        file.close();
    }
    this->close();
}


void NoteEditor::on_checkBox_stateChanged(int arg1) // Проверка флажка нового пароля.
{
    if (ui->checkBox->isChecked()){
        ui->NewPassword->setEnabled(true);
    }
    else{
        ui->NewPassword->setEnabled(false);
    }
}
