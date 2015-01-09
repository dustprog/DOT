#ifndef GridE_H
#define GridE_H

#include "GraphicsViewEditor.h"
#include "qpiemenu.h"

#include <QComboBox>
#include <QVBoxLayout>
class InheritanceWidget;
class GridEditor : public QWidget
{
    Q_OBJECT
public:
    QPieMenu *pieMenu;
    GridEditor(World *wo, QWidget *parent = 0);

    GridEditor(QWidget *parent = 0) : QWidget(parent) { }
    ~GridEditor()
    {
        //_sleep(10);
    }
    GraphcisViewEditor *view;
    GridScene *grid;
    World *worldptr;
    QWidget *p;
    int prev;
    QVBoxLayout *Layout;
    void Delete();

protected:
    virtual void paintEvent(QPaintEvent * event) override;


    QComboBox *combobox;
    InheritanceWidget *widget;
    bool Dragging;
    bool WhileSync;
private:
    void DeleteEntity(int index);
    void DeleteUnpaired();
    void DeleteAdvertisement(int index);
    private slots:

    void CreateEntity();
    void CreateAdvertisement();
    void TextReturn();
};
#endif
