#ifndef MYLISTWIDGETITEM_H
#define MYLISTWIDGETITEM_H

#include <QWidget>
#include <QString>

namespace Ui {
class myListWidgetItem;
}

class myListWidgetItem : public QWidget
{
    Q_OBJECT

public:
    explicit myListWidgetItem(QWidget *parent = nullptr);
    ~myListWidgetItem();

    void m_addItem(const QString& fileName ,QString filePath);
    QString m_fileName;
    QString m_filePath;

    QString isUDisk;

private:
    Ui::myListWidgetItem *ui;

signals:
    void deleteItem(myListWidgetItem * );
    void chooseFile(myListWidgetItem * );

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MYLISTWIDGETITEM_H
