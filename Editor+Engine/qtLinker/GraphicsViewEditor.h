#ifndef GRAPHICSVIEWEDITOR
#define GRAPHICSVIEWEDITOR
#include <QtWidgets/QWidget>
#include <QtWidgets/QGraphicsView>
class GridScene;
struct World;
class GraphcisViewEditor : public QGraphicsView
{
public:
    GraphcisViewEditor(QGraphicsScene *scene, QWidget *parent = 0);
    bool b;
    bool c;
    bool Delete;
    QPoint Temp;
    QPoint Last;
    QWidget *p;
    GridScene *ptr;
    World *world;
    QPoint GetLocal(QPoint Window);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *keyevent);
    void keyReleaseEvent(QKeyEvent *keyevent);
    int zoomval;
    bool Control;
};

#endif // GRAPHICSVIEWEDITOR

