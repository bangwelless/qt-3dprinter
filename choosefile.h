#ifndef CHOOSEFILE_H
#define CHOOSEFILE_H

#include <QWidget>
#include <QString>

namespace Ui {
class chooseFile;
}

class chooseFile : public QWidget
{
    Q_OBJECT

public:
    explicit chooseFile(QWidget *parent = nullptr);
    ~chooseFile();

    void setFileName(QString fileName);
    QString m_fileName;
    QString m_filePath;

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

signals:
    void choose();
    void cancel();
private:
    Ui::chooseFile *ui;
};

#endif // CHOOSEFILE_H
