/********************************************************************************
** Form generated from reading UI file 'MVCDemo.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MVCDEMO_H
#define UI_MVCDEMO_H

#include <QtCore/QVariant>
#include <QtQuickWidgets/QQuickWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MyListMVC.h"

QT_BEGIN_NAMESPACE

class Ui_MVCDemoClass
{
public:
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    MyListView *listView1;
    QHBoxLayout *horizontalLayout;
    QTreeView *treeView;
    QQuickWidget *quickWidget;

    void setupUi(QWidget *MVCDemoClass)
    {
        if (MVCDemoClass->objectName().isEmpty())
            MVCDemoClass->setObjectName(QString::fromUtf8("MVCDemoClass"));
        MVCDemoClass->resize(600, 400);
        horizontalLayout_2 = new QHBoxLayout(MVCDemoClass);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tableView = new QTableView(MVCDemoClass);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);

        listView1 = new MyListView(MVCDemoClass);
        listView1->setObjectName(QString::fromUtf8("listView1"));

        verticalLayout->addWidget(listView1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        treeView = new QTreeView(MVCDemoClass);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        horizontalLayout->addWidget(treeView);

        quickWidget = new QQuickWidget(MVCDemoClass);
        quickWidget->setObjectName(QString::fromUtf8("quickWidget"));
        quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

        horizontalLayout->addWidget(quickWidget);


        verticalLayout->addLayout(horizontalLayout);

        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 1);

        horizontalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2->setStretch(0, 1);

        retranslateUi(MVCDemoClass);

        QMetaObject::connectSlotsByName(MVCDemoClass);
    } // setupUi

    void retranslateUi(QWidget *MVCDemoClass)
    {
        MVCDemoClass->setWindowTitle(QCoreApplication::translate("MVCDemoClass", "MVCDemo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MVCDemoClass: public Ui_MVCDemoClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MVCDEMO_H
