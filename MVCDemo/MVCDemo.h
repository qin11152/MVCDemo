#pragma once

#include <QtWidgets/QWidget>
#include "ui_MVCDemo.h"
#include "MyListMVC.h"
#include "MyTreeMVC.h"

class MVCDemo : public QWidget
{
    Q_OBJECT

public:
    MVCDemo(QWidget *parent = nullptr);
    ~MVCDemo();

public slots:
    void onTreeViewClicked(const QModelIndex& index);

private:
    Ui::MVCDemoClass ui;
    MySortModel* ptrSortModel{ nullptr };
    MyListModel* ptrListModel{ nullptr };
    MyTreeModel* m_ptrTreeModel{ nullptr };
    MyStyledItemDelegate* m_ptrTreeDelegate{ nullptr };
};
