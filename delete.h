#ifndef DELETE_H
#define DELETE_H

#include <QWidget>
#include "mylistwidgetitem.h"

namespace Ui {
class Delete;
}

class Delete : public QWidget
{
    Q_OBJECT

public:
    explicit Delete(QWidget *parent = nullptr);
    ~Delete();

    QString fileName;
    QString filePath;
    myListWidgetItem * deleteItem;
    void setName();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void checkDelete(myListWidgetItem * deleteItem);
    void checkCancel(myListWidgetItem * deleteItem);

private:
    Ui::Delete *ui;
};

#endif // DELETE_H
