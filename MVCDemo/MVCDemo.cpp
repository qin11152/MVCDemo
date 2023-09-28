#include "MVCDemo.h"
#include "MyTreeMVC.h"
#include <QQmlContext>
#include <QHeaderView>

MVCDemo::MVCDemo(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setFixedSize(QSize(1920, 900));

    m_ptrListSortModelForQML = new MySortModel(this);
    m_ptrListModelForQML = new MyListModel(this);
    std::vector<MyListModelStu> datas;
    
    {
        std::vector<AddFriendInfo> vecAddFriendInfo;
        for (int i = 0; i < 2; ++i)
        {
            auto tmpinfo = AddFriendInfo();
            tmpinfo.isValid = false;
            tmpinfo.m_strFriendId = QString::number(i);
            tmpinfo.m_strProfileImagePath = "D:/1.png";
            tmpinfo.m_strVerifyInfo = "dakjshdkjas";
            vecAddFriendInfo.push_back(tmpinfo);
        }
        ui.listView1->setDataToModel(vecAddFriendInfo);
    }

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

        m_ptrListSortModelForQML->setSourceModel(m_ptrListModelForQML);
        m_ptrListModelForQML->setData(datas);
        ui.quickWidget->rootContext()->setContextProperty("listModels", m_ptrListSortModelForQML);
        ui.quickWidget->setSource(QUrl("qrc:/qml/Listview.qml"/*"qrc:/testMVC/Swipview.qml"*/));
        ui.quickWidget->setAttribute(Qt::WA_AcceptTouchEvents);
        setAttribute(Qt::WA_AcceptTouchEvents);
    }
    
    {
        m_ptrTableModel = new MyTableModel();
        m_ptrTableDelegate = new MyTableDelegate();

        std::vector<AddFriendInfo> vecAddFriendInfo;
        for (int i = 0; i < 3; ++i)
        {
            auto tmpinfo = AddFriendInfo();
            tmpinfo.isValid = false;
            tmpinfo.m_strFriendId = QString::number(i);
            tmpinfo.m_strProfileImagePath = "D:/1.png";
            tmpinfo.m_strVerifyInfo = "dakjshdkjas";
            tmpinfo.comboxText << "com1" << "com2" << "com3";
            vecAddFriendInfo.push_back(tmpinfo);
        }
        m_ptrTableModel->setData(vecAddFriendInfo);

        ui.tableView->setModel(m_ptrTableModel);
        ui.tableView->setItemDelegate(m_ptrTableDelegate);
        ui.tableView->verticalHeader()->setDefaultSectionSize(80);
        ui.tableView->horizontalHeader()->setVisible(false);
        ui.tableView->verticalHeader()->setVisible(false);
        ui.tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui.tableView->setColumnWidth(0, 80);
        ui.tableView->setColumnWidth(1, 200);
        ui.tableView->setColumnWidth(2, 260);
        ui.tableView->setColumnWidth(3, 260);
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
                stu->score = rand() % 100;
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
        heads->resizeSection(2, 200);
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
    m_ptrListSortModelForQML->setFilterMap(filterMap);
}

MVCDemo::~MVCDemo()
{
    delete m_ptrTableDelegate;
    m_ptrTableDelegate = nullptr;
    delete m_ptrTableModel;
    m_ptrTableModel = nullptr;
    delete m_ptrTreeDelegate;
    m_ptrTreeDelegate = nullptr;
    delete m_ptrTreeModel;
    m_ptrTreeModel = nullptr;
    delete m_ptrListSortModelForQML;
    m_ptrListSortModelForQML = nullptr;
    delete m_ptrListModelForQML;
    m_ptrListModelForQML = nullptr;
}
