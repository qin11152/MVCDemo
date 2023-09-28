#pragma once
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QAbstractListModel>
#include <QScopedPointer>
#include <QPushButton>
#include <QListView>

#include <vector>
#include <memory>

enum class UserRoleDefine
{
    AddFriendId = Qt::UserRole + 1,
    AddFriendImagePath = Qt::UserRole + 2,
    AddFriednVerifyInfo = Qt::UserRole + 3,
    AddFriendValid = Qt::UserRole + 4,
    AddFriendList = Qt::UserRole + 5,
    AddFriendComboxRealValue = Qt::UserRole + 6,
};

struct AddFriendInfo
{
    QStringList comboxText;
    QString m_strFriendId{ "" };        //���ѵ�id
    QString m_strProfileImagePath{ "" };//����ͷ���·��
    QString m_strVerifyInfo{ "" };      //���ѵ���֤��Ϣ
    QString m_strComBoxRealValue{ "com1" };
    bool isValid{ false };              //�Ƿ��Ѿ����˺���
};


class testBtn : public QPushButton
{
    Q_OBJECT

public:
    testBtn(QWidget* parent = nullptr);
    ~testBtn();
};


class ListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ListModel(QObject* parent = nullptr);
    ~ListModel();

    void setData(std::vector<AddFriendInfo>& addFriendInfo);
    void addData(const AddFriendInfo& addFriendInfo);

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    QMap<int, QVariant> itemData(const QModelIndex& index)const override;

private:
    std::vector<AddFriendInfo> m_vecAddFriendInfo;
};

Q_DECLARE_METATYPE(AddFriendInfo)

class DelegateForListView : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DelegateForListView(QObject* parent = nullptr);
    ~DelegateForListView();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    void setEditorData(QWidget* editor, const QModelIndex& index)const override;

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)const override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

private:
    std::shared_ptr<testBtn> m_btn;
};



struct MyListModelRole
{
    enum Role
    {
        ListModelStr = Qt::UserRole + 1,
        ListModelInt,
        ListModeImage,
    };
};

struct MyListModelStu
{
    QString m_strInfo{ "" };
    QString m_strImagePaht{ "file:///D:/1.png" };
    int m_iIndex{ -1 };
};

class MyListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    MyListModel(QObject* parent = nullptr);
    ~MyListModel();

    //����model�е�����
    void setData(std::vector<MyListModelStu>& vecListModelData);
    void setDataList(const QList<std::map<int, QVariant>>& dataList);

    //************************************
    // Method:    data
    // FullName:  MyListModel::data()
    // Access:    public 
    // Returns:   QT_NAMESPACE::QVariant
    // Qualifier: const 
    // Parameter: const QModelIndex & index��ģ���е�ĳ��index
    // Parameter: int role
    // Brief ����ĳһ���ض�role��Ӧ��value
    //************************************
    QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */)const override;

    //************************************
    // Method:    rowCount
    // FullName:  MyListModel::rowCount()
    // Access:    public 
    // Returns:   int
    // Qualifier: const 
    // Parameter: const QModelIndex & parent
    // Brief: ��������
    //************************************
    int rowCount(const QModelIndex& parent /* = QModelIndex() */)const override;

    //************************************
    // Method:    itemData
    // FullName:  MyListModel::itemData()
    // Access:    public 
    // Returns:   QMap<int, QVariant>
    // Qualifier: const 
    // Parameter: const QModelIndex & index
    // Brief: ����һ��map��map�б�����ǲ�ͬrole��Ӧ��value
    //************************************
    QMap<int, QVariant> itemData(const QModelIndex& index)const override;
    //************************************
    // Method:    roleNames
    // FullName:  MyListModel::roleNames()
    // Access:    public 
    // Returns:   QHash<int, QT_NAMESPACE::QByteArray>
    // Qualifier: const
    // Brief: ����role��Ӧ��string����string������д��delegate����qml��ʹ��
    //************************************
    QHash<int, QByteArray> roleNames()const override;

private:
    std::vector<MyListModelStu> m_vecModelData;
    QList<std::map<int, QVariant>> m_dataList;
};

Q_DECLARE_METATYPE(MyListModelStu)


class MyListViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    MyListViewDelegate(QObject* parent = nullptr);
    ~MyListViewDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;
};

class MySortModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MySortModel(QObject* parent);
    ~MySortModel();

    void setFilterMap(const std::map<int, QVariant>& mapFilter);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent)const override;

private:
    std::map<int, QVariant> m_mapFilter;
};

class MyListView  : public QListView
{
    Q_OBJECT

public:
    MyListView(QWidget* parent = nullptr);
    ~MyListView();

    void setViewFilterRule(const std::map<int, QVariant>& filterMap);
    void addToModel(const AddFriendInfo& friendInfo);
    void setDataToModel(std::vector<AddFriendInfo>& addFriendInfo);

signals:
    void signalClicked(int num);

protected:
    void mousePressEvent(QMouseEvent* event)override;

private:
    ListModel* m_ptrListModel{ nullptr };
    DelegateForListView* m_ptrListDelegate{ nullptr };
    MySortModel* m_ptrSortAndFilterModel{ nullptr };
};
