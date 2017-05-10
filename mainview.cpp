#include "mainview.h"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QCoreApplication>
#include <QCoreApplication>
#include <Qt>
#include <QScrollBar>
#include <QDebug>
#include <math.h>

using namespace std;

MainView::MainView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent)
{
    m_scrolling = false;
    m_selected = false;
}

void MainView::mousePressEvent(QMouseEvent *e)
{
    time.start();
    m_clickStartPoint = e->pos();
    qDebug() << "started click";
    if (m_selected) {
        m_scrolling = true;
        m_mousePressViewPoint = e->pos();
        viewport()->setCursor(Qt::ClosedHandCursor);
    }
    else {
        QMouseEvent event(e->type(), e->localPos(), e->button(), e->buttons(), e->modifiers());
        QCoreApplication::sendEvent(parent(),&event);
    }
}

void MainView::mouseReleaseEvent(QMouseEvent *e)
{
    int difference = time.elapsed();
    QPoint delta = (e->pos() - m_clickStartPoint);
    int deltasize = std::abs(delta.x()) + std::abs(delta.y());
    qDebug()<<"Delta size"<<deltasize;
    if (difference < 200 && deltasize < 10) {
        if (m_selected == true) {
            m_selected = false;
            setStyleSheet("border: none");
        }
        else {
            setStyleSheet("border: 3px solid orange");
            m_selected = true;
        }
    }

    if (m_scrolling) {
        m_scrolling = false;
        viewport()->setCursor(Qt::OpenHandCursor);
    }
    else {
        QMouseEvent event(e->type(), e->localPos(), e->button(), e->buttons(), e->modifiers());
        QCoreApplication::sendEvent(parent(),&event);
    }
}

void MainView::mouseMoveEvent(QMouseEvent *e)
{
    if(m_selected && m_scrolling) {
        QScrollBar *hBar = horizontalScrollBar();
        QScrollBar *vBar = verticalScrollBar();
        QPoint delta = (e->pos() - m_mousePressViewPoint);
        hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
        vBar->setValue(vBar->value() - delta.y());
        m_mousePressViewPoint = e->pos();
    }
    else {
        QMouseEvent event(e->type(), e->localPos(), e->button(), e->buttons(), e->modifiers());
        QCoreApplication::sendEvent(parent(),&event);
    }
}

void MainView::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    m_selected = false;
    setStyleSheet("border: none");
}

void MainView::wheelEvent(QWheelEvent* event)
{
    if (!m_selected) return;
    // Typical Calculations (Ref Qt Doc)
    const int degrees = event->delta() / 8;
    int steps = degrees / 15;

    static qreal scale = 0;
    static qreal scaleFactor = 0;
    const qreal minFactor = .1;
    const qreal maxFactor = 10.0;

    if (steps > 0 && scaleFactor < maxFactor) {
        scale ++;
    }
    else if (steps < 0 && scaleFactor > minFactor) {
        scale --;
    }

    scaleFactor = exp(scale/4);
    scaleFactor = (scaleFactor <= minFactor) ?
                minFactor :
                (scaleFactor >= maxFactor) ? maxFactor : scaleFactor;

    qDebug() << "scale" << scale;
    qDebug() << "scaleFactor" << scaleFactor;


    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setTransform(QTransform(scaleFactor, 0, 0, scaleFactor, 0, 0));
}