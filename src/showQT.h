#pragma once
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QScrollBar>
#include <QMouseEvent>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/strategies/buffer.hpp>
#include <boost/geometry/io/wkt/read.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "bump.h"
#include "designRule.h"

using point = boost::geometry::model::d2::point_xy<double>;
using polygon = boost::geometry::model::polygon<point>;
using multi_point = boost::geometry::model::multi_point<point>;
using multi_polygon = boost::geometry::model::multi_polygon<polygon>;
using linestring = boost::geometry::model::linestring<point>;
using namespace std;

void QtDraw(QGraphicsScene *scene, const string &bumpLocationFile, const string &designRuleFile);
void QtReadResult(vector<Bump> &bumps, const string &bumpLocationFile);
void QtReadDesignRule(DesignRule &designRule, const string &designRuleFile);


class QtView : public QGraphicsView {
public:
    QtView(QGraphicsScene *scene) : QGraphicsView(scene) {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setStyleSheet("background-color: black;");
    }

protected:
    bool rightMousePressed = false;
    QPoint lastMousePos;

    // 滑鼠按下事件
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::RightButton) {
            rightMousePressed = true;
            lastMousePos = event->pos();
            setCursor(Qt::ClosedHandCursor);
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }

    // 滑鼠移動事件
    void mouseMoveEvent(QMouseEvent *event) override {
        if (rightMousePressed) {
            QPoint delta = event->pos() - lastMousePos;
            lastMousePos = event->pos();
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        } else {
            QGraphicsView::mouseMoveEvent(event);
        }
    }

    // 滑鼠釋放事件
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::RightButton) {
            rightMousePressed = false;
            setCursor(Qt::ArrowCursor);
        } else {
            QGraphicsView::mouseReleaseEvent(event);
        }
    }


    void wheelEvent(QWheelEvent *event) override {
        if (event->modifiers() & Qt::ControlModifier) {
            double factor = (event->angleDelta().y() > 0) ? 1.1 : 0.9;
            scale(factor, factor);
        } else {
            QGraphicsView::wheelEvent(event);
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Z) {
            resetTransform();  // 重置縮放和平移
            centerOn(0, 0);     // 回到初始座標位置 (0, 0)
        } else {
            QGraphicsView::keyPressEvent(event);
        }
    }
};

