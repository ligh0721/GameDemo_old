#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "UipEditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow* getUi() const;

protected:
     void closeEvent(QCloseEvent *event);
    
private slots:
     void on_btnUnitEditor_clicked();

private:
    Ui::MainWindow *ui;
    CUipEditor* m_pUe;
};

#endif // MAINWINDOW_H
