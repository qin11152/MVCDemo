#include "MVCDemo.h"
#include "MyTreeMVC.h"
#include <QQmlContext>
#include <QHeaderView>

MVCDemo::MVCDemo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(QSize(1920, 900));

    ptrSortModel = new MySortModel(this);
    ptrListModel = new MyListModel(this);
    std::vector<MyListModelStu> datas;
    
    {
        for (int i = 0; i < 2; ++i)
        {
            MyListModelStu tmpInfo;
            char tmp = 'a';
            tmpInfo.m_strInfo = tmp;
            tmpInfo.m_iIndex = i;
            datas.emplace_back(tmpInfo);
        }
        for (int i = 0; i < 1; ++i)
        {
            MyListModelStu tmpInfo;
            char tmp = 'b';
            tmpInfo.m_strInfo = tmp;
            tmpInfo.m_iIndex = i;
            datas.emplace_back(tmpInfo);
        }
        for (int i = 0; i < 3; ++i)
        {
            MyListModelStu tmpInfo;
            char tmp = 'c';
            tmpInfo.m_strInfo = tmp;
            tmpInfo.m_iIndex = i;
            datas.emplace_back(tmpInfo);
        }
    }

    //qmlRegisterType<MyListModel>("ListModels", 1, 0, "ListModels");
    {
        ptrSortModel->setSourceModel(ptrListModel);
        ptrListModel->setData(datas);
        ui.quickWidget->rootContext()->setContextProperty("listModels", ptrSortModel);
        ui.quickWidget->setSource(QUrl("qrc:/qml/Listview.qml"/*"qrc:/testMVC/Swipview.qml"*/));
        ui.quickWidget->setAttribute(Qt::WA_AcceptTouchEvents);
        setAttribute(Qt::WA_AcceptTouchEvents);
    }
    
    {
        std::vector<Class*> classsList;
        int classCount = 5;
        int studentCount = 5;
        for (int i = 0; i < classCount; i++)
        {
            char tmp = 'a' + i;
            std::string cla = "";
            cla += tmp;
            Class* clas = new Class(cla);
            for (int j = 0; j < studentCount; j++)
            {
                Student* stu = new Student();
                stu->age = rand();
                stu->name=QString("name%1").arg(j).toStdString();
                stu->score = rand();
                clas->m_vecStudent.push_back(stu);
            }
            classsList.push_back(clas);
        }

        QStringList headers;
        headers << QString::fromLocal8Bit("班级")
            << QString::fromLocal8Bit("姓名")
            << QString::fromLocal8Bit("年龄")
            << QString::fromLocal8Bit("分数");

        m_ptrTreeModel = new MyTreeModel(headers, ui.treeView);
        TreeItem* root = m_ptrTreeModel->getRootItem();
        for(auto &clas: classsList)
        {
            TreeItem* tmpItem1 = new TreeItem(clas,root);
            //tmpItem1->setData(clas);
            root->appendChild(tmpItem1);

            for(auto& stu:clas->m_vecStudent)
            {
                TreeItem* tmpItem2 = new TreeItem(stu,tmpItem1);
                //tmpItem2->setData(stu);
                tmpItem1->appendChild(tmpItem2);
            }
        }
        ui.treeView->setModel(m_ptrTreeModel);
        m_ptrTreeDelegate = new MyStyledItemDelegate(ui.treeView);
        ui.treeView->setItemDelegate(m_ptrTreeDelegate);
        auto heads = ui.treeView->header();
        for (int i = 0; i < headers.size(); ++i)
        {
            heads->setSectionResizeMode(i, QHeaderView::Fixed);
            heads->resizeSection(i, 200);
        }
        heads->resizeSection(0, 80);
    }

    connect(ui.treeView, &QTreeView::clicked, this, &MVCDemo::onTreeViewClicked);
}

void MVCDemo::onTreeViewClicked(const QModelIndex& index)
{
    auto treeItem = reinterpret_cast<TreeItem*>(index.internalPointer());
    if (1 != treeItem->level())
    {
        return;
    }
    auto ptrClass = treeItem->data(0).toString();
    std::map<int, QVariant> filterMap;
    filterMap.insert({ MyListModelRole::Role::ListModelStr,ptrClass });
    ptrSortModel->setFilterMap(filterMap);
}

MVCDemo::~MVCDemo()
{}
