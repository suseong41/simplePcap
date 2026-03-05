#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pcap/pcap.h>
#include <string>
#include <QstringListModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onStartButton();

private:
    Ui::MainWindow *ui;
    std::string devType;

    bool isRunning;
    QStringListModel *lenModel;
    QStringListModel *typeModel;
    QStringListModel *saModel;
    QStringListModel *daModel;
};
#endif // MAINWINDOW_H
