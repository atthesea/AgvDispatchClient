﻿#include "dialogrootpath.h"

#include <QtWidgets>


DialogRootPath::DialogRootPath(OneMap *_onemap, QWidget *parent) : QDialog(parent),
    onemap(_onemap)
{
    init();
}

void DialogRootPath::init()
{
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->setSpacing(10);
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setHorizontalSpacing(20);
    gridlayout->setVerticalSpacing(10);
    QLabel *fromlabel = new QLabel("from");
    fromComboboxA = new QComboBox;
    fromComboboxA->setObjectName("fromComboboxA");
    QList<MapFloor *> floors = QList<MapFloor *>::fromStdList(onemap->getFloors());
    foreach (auto floor, floors) {
        fromComboboxA->addItem(QString::fromStdString(floor->getName()));
    }
    fromComboboxB = new QComboBox;
    fromComboboxB->setObjectName("fromComboboxB");

    gridlayout->addWidget(fromlabel,0,0);
    gridlayout->addWidget(fromComboboxA,0,1);
    gridlayout->addWidget(fromComboboxB,0,2);

    QLabel *tolabel = new QLabel("to");
    toComboboxA = new QComboBox;
    toComboboxA->setObjectName("toComboboxA");

    foreach (auto floor, floors) {
        toComboboxA->addItem(QString::fromStdString(floor->getName()));
    }
    toComboboxB = new QComboBox;
    toComboboxB->setObjectName("toComboboxB");

    gridlayout->addWidget(tolabel,1,0);
    gridlayout->addWidget(toComboboxA,1,1);
    gridlayout->addWidget(toComboboxB,1,2);


    QHBoxLayout *btnlayout = new QHBoxLayout;
    okbtn = new QPushButton("OK");
    okbtn->setObjectName("okbtn");
    cancelbtn = new QPushButton("cancel");
    cancelbtn->setObjectName("cancelbtn");
    btnlayout->addWidget(okbtn);
    btnlayout->addWidget(cancelbtn);

    mainlayout->addItem(gridlayout);
    mainlayout->addItem(btnlayout);

    setLayout(mainlayout);


    QMetaObject::connectSlotsByName(this);


    if(fromComboboxA->count()>0){
        fromComboboxA->setCurrentIndex(0);
        on_fromComboboxA_currentIndexChanged(0);
    }


    if(toComboboxA->count()>1){
        toComboboxA->setCurrentIndex(1);
        on_toComboboxA_currentIndexChanged(1);
    }else if(toComboboxA->count()>0){
        toComboboxA->setCurrentIndex(0);
        on_toComboboxA_currentIndexChanged(0);
    }
}

void DialogRootPath::on_okbtn_clicked(bool b)
{
    if(fromComboboxA->currentIndex()!=toComboboxA->currentIndex()){
        if(fromComboboxB->currentIndex()>=0 && fromComboboxB->count()>0
                &&toComboboxB->currentIndex()>=0 && toComboboxB->count()>0 ){
            int fromIndexA = fromComboboxA->currentIndex();
            int fromIndexB = fromComboboxB->currentIndex();
            int toIndexA = toComboboxA->currentIndex();
            int toIndexB = toComboboxB->currentIndex();

            auto floors = QList<MapFloor *>::fromStdList(onemap->getFloors());
            auto floorFrom = floors[fromIndexA];
            auto floorTo = floors[toIndexA];

            MapPoint *from  = nullptr;
            MapPoint *to = nullptr;

            auto points =  QList<int>::fromStdList(floorFrom->getPoints());
            foreach (auto p, points) {
                MapPoint *ppp = static_cast<MapPoint *>(onemap->getSpiritById(p));
                if(ppp==nullptr)continue;
                if(ppp->getMapChange()){
                    if(fromIndexB == 0){
                        from = ppp;
                        break;
                    }else{
                        --fromIndexB;
                    }
                }
            }

            points =  QList<int>::fromStdList(floorTo->getPoints());
            foreach (auto p, points) {
                MapPoint *ppp = static_cast<MapPoint *>(onemap->getSpiritById(p));
                if(ppp==nullptr)continue;
                if(ppp->getMapChange()){
                    if(toIndexB == 0){
                        to = ppp;
                        break;
                    }else{
                        --toIndexB;
                    }
                }
            }

            if(from == nullptr || to == nullptr){
                QMessageBox::warning(this,QStringLiteral("选择错误"),QStringLiteral("未选择或选择相同楼层不可以"));
            }else{
                emit sig_add_root_path(from,to);
                accept();
            }
        }
    }else{
        QMessageBox::warning(this,QStringLiteral("选择错误"),QStringLiteral("未选择或选择相同楼层不可以"));
    }
}
void DialogRootPath::on_cancelbtn_clicked(bool b)
{
    reject();
}

void DialogRootPath::on_fromComboboxA_currentIndexChanged(int index)
{
    fromComboboxB->clear();

    auto fs = QList<MapFloor *>::fromStdList( onemap->getFloors());
    if(index<fs.length()){
        MapFloor *f = fs[index];
        if(f!=nullptr){
            auto ps = f->getPoints();
            foreach (auto p, ps) {
                MapPoint *ppp = static_cast<MapPoint *>( onemap->getSpiritById(p));
                if(ppp==nullptr)continue;
                if(ppp->getMapChange()){
                    fromComboboxB->addItem(QString::fromStdString(ppp->getName()));
                }
            }
        }
    }
}

void DialogRootPath::on_toComboboxA_currentIndexChanged(int index)
{
    toComboboxB->clear();
    auto fs = QList<MapFloor *>::fromStdList( onemap->getFloors());
    if(index<fs.length()){
        MapFloor *f = fs[index];
        if(f!=nullptr){
            auto ps = f->getPoints();
            foreach (auto p, ps) {
                MapPoint *ppp = static_cast<MapPoint *>( onemap->getSpiritById(p));
                if(ppp==nullptr)continue;
                if(ppp->getMapChange()){
                    toComboboxB->addItem(QString::fromStdString(ppp->getName()));
                }
            }
        }
    }
}
