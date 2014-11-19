#include "GridEditor.h"
#include "GridScene.h"
#include "Inheritance.h"
#include <qcombobox.h>

GridEditor::GridEditor(World *wo, QWidget *parent)
    :QWidget(parent)
{
    Layout = new QVBoxLayout(this);
    this->p = parent;
    this->worldptr = wo;
    grid = new GridScene(-10, -10, 1000, 1000);
    view = new GraphcisViewEditor(grid);
    view->rotate(30);

    view->p = this;
    Layout->addWidget(view);
    pieMenu = new QPieMenu(this);
    pieMenu->addAction("Agent", this, SLOT(CreateEntity()));
    pieMenu->addAction("Ad",  this, SLOT(CreateAdvertisement()));
    pieMenu->addAction("Other", pieMenu, SLOT(closeMenu()));
    combobox = new QComboBox();
    Layout->addWidget(pieMenu);
    Dragging = false;
    view->ptr = grid;
    view->world = worldptr;
}

void GridEditor::Delete()
{
    switch (grid->selected.t)
    {
    case 0:
        DeleteEntity(grid->selected.i);
        view->Delete = false;
        break;
    case 1:
        DeleteAdvertisement(grid->selected.i);
        view->Delete = false;
        break;
    default:
        break;
    }
}

void GridEditor::paintEvent(QPaintEvent * event)
{
    if (view->c == true)
    {
        TypeIndex typeindex = grid->CheckIntercept(view->Temp); //Check to see if we interecepted any of the ads or entites
        if (typeindex.t == -1)
        {
            pieMenu->showMenu();
        }
        view->c = false;
    }
    if (view->Delete == true)
    {
        Delete();
        view->Delete = false;
    }
    if (worldptr->ExternUpdate)
    {
        DeleteUnpaired();
        worldptr->ExternUpdate = false;
    }
    setFixedHeight(p->height() - 30);
}

void GridEditor::DeleteEntity(int index)
{
    std::string Link = worldptr->SimulationEntities->at(index)->Link;
    int num = worldptr->SimulationEntities->at(index)->LinkNumber;
    worldptr->SimulationEntities->erase(worldptr->SimulationEntities->begin() + index);
    grid->Entities.erase(grid->Entities.begin() + index);
    grid->Entities.squeeze();
    worldptr->SimulationEntities->shrink_to_fit();
    int m = 0;
    for (int i = 0; i < worldptr->SimulationEntities->size(); i++)
    {
        EntityEdit temp = grid->Entities.at(i);
        temp.index = i;
        if (temp.Link == Link) //They inherit from the same object
        {
            if (worldptr->SimulationEntities->at(i)->LinkNumber > num)
            {
                worldptr->SimulationEntities->at(i)->Link = Link;
                worldptr->SimulationEntities->at(i)->Name = Link + ", Poly " + std::to_string(num + m);

                temp.Link = Link;
                temp.Name = Link + ", Poly: " + std::to_string(num + m);
                m++;
            }
        }
        grid->Entities.replace(i, temp);
    };

}

void GridEditor::DeleteUnpaired()
{
    bool check = false;
    for (int i = 0; i < grid->Entities.size(); i++)
    {
        if (worldptr->ReturnEntity(grid->Entities.at(i).Link) == nullptr)
        {
            worldptr->SimulationEntities->erase(worldptr->SimulationEntities->begin() + worldptr->SimulationalEntityIndex(grid->Entities.at(i).Name));
            grid->Entities.erase(grid->Entities.begin() + i);
            check = true;
        }
    }
    for (int i = 0; i < grid->Ads.size(); i++)
    {
        if (worldptr->ReturnAdvertisement(grid->Ads.at(i).Link) == nullptr)
        {
            worldptr->SimulationAds->erase(worldptr->SimulationAds->begin() + worldptr->SimulationAdIndex(grid->Ads.at(i).Name));
            grid->Ads.erase(grid->Ads.begin() + i);
            check = true;
        }
    }
    if (check)
        DeleteUnpaired();
}

void GridEditor::DeleteAdvertisement(int index)
{
    std::string Link = worldptr->SimulationAds->at(index)->Link;
    int num = worldptr->SimulationAds->at(index)->LinkNumber;
    worldptr->SimulationAds->erase(worldptr->SimulationAds->begin() + index);
    grid->Ads.erase(grid->Ads.begin() + index);
    grid->Ads.squeeze();
    worldptr->SimulationEntities->shrink_to_fit();
    int m = 0;
    for (int i = 0; i < worldptr->SimulationAds->size(); i++)
    {
        AdEdit temp = grid->Ads.at(i);
        temp.index = i;
        if (temp.Link == Link) //They inherit from the same object
        {
            if (worldptr->SimulationAds->at(i)->LinkNumber > num)
            {
                worldptr->SimulationAds->at(i)->Name = Link + ", Poly: " + std::to_string(num + m);
                worldptr->SimulationAds->at(i)->Link = Link; //Set my pointer
                temp.Link = Link;
                temp.Name = Link + ", Poly: " + std::to_string(num + m);
                m++;
            }
        }
        grid->Ads.replace(i, temp);
    };

}
void GridEditor::CreateEntity()
{
    widget = new InheritanceWidget(worldptr,0);
    connect(widget->button, SIGNAL(pressed()), SLOT(TextReturn()));
    widget->show();
    WhileSync = true;
}
void GridEditor::CreateAdvertisement()
{
    widget = new InheritanceWidget(worldptr, 1);
    connect(widget->button, SIGNAL(pressed()), SLOT(TextReturn()));
    widget->show();
    WhileSync = true;
}

void GridEditor::TextReturn()
{
    while (WhileSync)
    {
        QPointF point;
        Entity *et;
        Advertisement *at;
        EntityEdit ed;
        AdEdit ad;
        switch (widget->t)
        {
        case 0:
            widget->hide();
            worldptr->AddSimulationEntity();
            et = worldptr->CopyEntityForSim(widget->combobox->currentText().toStdString(),VECTOR2DF(view->Temp.x(),
                                                                                                    view->Temp.y()), grid->Entities.size()); //Link the edit object with the actual object
            ed.Name = et->Name;
            ed.Link = et->Link;
            ed.index = grid->Entities.size();
            ed.Position = VECTOR2DF(view->Temp.x(), view->Temp.y());
            ed.rect = new QRect(view->Temp.x(), view->Temp.y(), 50, 50);
            grid->Entities.push_back(ed);
            grid->selected = TypeIndex(0, grid->Entities.size() - 1);
            WhileSync = false;
            break;
        case 1:
            widget->hide();
            worldptr->AddSimulationAdvertisement();
            at = worldptr->CopyAdForSim(widget->combobox->currentText().toStdString(), VECTOR2DF(view->Temp.x(),
                                                                                                 view->Temp.y()), grid->Ads.size()); //Link the edit object with the actual object
            at->LinkAddress = worldptr->AdIndex(at->Link);
            ad.Name = at->Name;
            ad.Link = at->Link;
            ad.index = grid->Ads.size();
            ad.Position = VECTOR2DF(view->Temp.x(), view->Temp.y());
            ad.rect = new QRect(view->Temp.x(), view->Temp.y(), 50, 50);
            grid->Ads.push_back(ad);
            grid->selected = TypeIndex(1, grid->Ads.size() - 1);
            WhileSync = false;
            break;
        default:
            break;
        }
    }
}
