#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainView;
class QImage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QImage * mainImage;

private:

    MainView* m_mainview;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void focusOutEvent ( QFocusEvent * event );

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;
};

#endif // MAINWINDOW_H