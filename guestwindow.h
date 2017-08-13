#ifndef GUESTWINDOW_H
#define GUESTWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSignalMapper>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

class guestWindow : public QDialog
{
    Q_OBJECT

public:
    explicit guestWindow(QWidget *parent = 0);
    QString ipNumber;

private slots:
    void deleteIP();
    void resetIP();
    void addNumber(int num);
    void accept();

private:
    QPushButton *buttons[2][5],*resetButton,*delButton,*okButton,*cancelButton,*dotButton;
    QLineEdit *ipBoard;
    QLabel *label;
    QGridLayout *numberLayout,*decisionLayout;
    QVBoxLayout *outerLayout;
    QHBoxLayout *showerLayout;
    QSignalMapper *mapper;
};

#endif // GUESTWINDOW_H
