#pragma once

#include <QtWidgets/QWidget>
#include "ui_MVCDemo.h"
#include "MyListMVC.h"
#include "MyTreeMVC.h"
#include "MyTableMVC.h"

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
    MySortModel* m_ptrListSortModelForQML{ nullptr };
    MyListModel* m_ptrListModelForQML{ nullptr };
    MyTreeModel* m_ptrTreeModel{ nullptr };
    MyStyledItemDelegate* m_ptrTreeDelegate{ nullptr };
    MyTableModel* m_ptrTableModel{ nullptr };
    MyTableDelegate* m_ptrTableDelegate{ nullptr };
};
