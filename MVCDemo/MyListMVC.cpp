#include "MyListMVC.h"

#include <QPainter>
#include <qcombobox.h>
#include <qevent.h>
#include <qdebug.h>

testBtn::testBtn(QWidget* parent)
    : QPushButton(parent)
{
    setStyleSheet("QPushButton{background-color:green;}");
}

testBtn::~testBtn()
{
}

DelegateForListView::DelegateForListView(QObject* parent)
    : QStyledItemDelegate(parent),
    m_btn(new testBtn())
{
    //m_btn->setStyleSheet("QPushButton {border-radius: 8; background-color: red;}");
}

DelegateForListView::~DelegateForListView()
{
}

void DelegateForListView::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    QString strId = index.data((int)UserRoleDefine::AddFriendId).toString();
    QString strImagePath = index.data((int)UserRoleDefine::AddFriendImagePath).toString();
    QString strVerifyInfo = index.data((int)UserRoleDefine::AddFriednVerifyInfo).toString();
    bool isValid = index.data((int)UserRoleDefine::AddFriendValid).toBool();

    QRect imageRect(option.rect.x() + 20, option.rect.y() + 10, 40, 40);
    QRect idRect(option.rect.x() + 80, option.rect.y() + 10, 100, 20);
    QRect verifyRect(option.rect.x() + 80, option.rect.y() + 30, 300, 30);

    auto displayOption = option;
    displayOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    painter->drawPixmap(imageRect, QPixmap(strImagePath));
    painter->drawText(idRect, strId);
    painter->drawText(verifyRect, strVerifyInfo);

    if (!isValid)
    {
        QStyleOptionButton button;
        button.rect = QRect(option.rect.right() - 230, option.rect.y() + 10, 80, 40);
        button.state |= QStyle::State_None;
        button.text = QString::fromLocal8Bit("同意");

        QWidget* tmpBtn = m_btn.get();

        tmpBtn->style()->drawControl(QStyle::CE_PushButton, &button, painter, tmpBtn);
    }
    else
    {
        painter->drawText(QRect{ option.rect.right() - 230, option.rect.y() + 10, 80, 40 }, "已添加");
    }
}


bool DelegateForListView::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    QMouseEvent* pEvent = reinterpret_cast<QMouseEvent*> (event);
    QRect buttonRect = { option.rect.right() - 230, option.rect.y() + 10, 80, 40 };
    if (!buttonRect.contains(pEvent->pos()))
    {
        return false;
    }
    if (pEvent->button() == Qt::LeftButton)
    {
        qDebug() << index.data((int)UserRoleDefine::AddFriendId).toString();
    }
    return true;
}

QSize DelegateForListView::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize({ 800,60 });
}

QWidget* DelegateForListView::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == 0)
    {
        QComboBox* box = new QComboBox(parent);
        box->setFixedHeight(option.rect.height());
        box->addItems(index.model()->data(index, (int)UserRoleDefine::AddFriendList).toStringList());
        return box;
    }
    else
    {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void DelegateForListView::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, (int)UserRoleDefine::AddFriendComboxRealValue).toString();
    QComboBox* box = static_cast<QComboBox*>(editor);
    box->setCurrentText(text);
}

void DelegateForListView::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* box = static_cast<QComboBox*>(editor);
    if (box)
    {
        model->setData(index, box->currentText(), (int)UserRoleDefine::AddFriendComboxRealValue);
    }
    else
    {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void DelegateForListView::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect.right() - 400, 0, 100, option.rect.height());
}

ListModel::ListModel(QObject* parent)
    : QAbstractListModel(parent)
{
    m_vecAddFriendInfo = {};
}

ListModel::~ListModel()
{
}

void ListModel::setData(std::vector<AddFriendInfo>& addFriendInfo)
{
    beginResetModel();
    m_vecAddFriendInfo.clear();
    m_vecAddFriendInfo = addFriendInfo;
    endResetModel();
}

void ListModel::addData(const AddFriendInfo& addFriendInfo)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_vecAddFriendInfo.insert(m_vecAddFriendInfo.begin(), addFriendInfo);
    endInsertRows();
}

QVariant ListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    switch (role)
    {
    case (int)UserRoleDefine::AddFriendId:
        return m_vecAddFriendInfo[index.row()].m_strFriendId;
    case (int)UserRoleDefine::AddFriendImagePath:
        return m_vecAddFriendInfo[index.row()].m_strProfileImagePath;
    case (int)UserRoleDefine::AddFriednVerifyInfo:
        return m_vecAddFriendInfo[index.row()].m_strVerifyInfo;
    case (int)UserRoleDefine::AddFriendValid:
        return m_vecAddFriendInfo[index.row()].isValid;
    case (int)UserRoleDefine::AddFriendList:
        return m_vecAddFriendInfo[index.row()].comboxText;
    case (int)UserRoleDefine::AddFriendComboxRealValue:
        return m_vecAddFriendInfo[index.row()].m_strComBoxRealValue;
    default:
        return QVariant();
    }

    return QVariant();
}

int ListModel::rowCount(QModelIndex const& parent /*= QModelIndex()*/) const
{
    return m_vecAddFriendInfo.size();
}

QMap<int, QVariant> ListModel::itemData(const QModelIndex& index) const
{
    QMap<int, QVariant> roles;

    int iDataIndex = index.row();

    auto tmpInfo = m_vecAddFriendInfo[iDataIndex];

    roles.insert(int(UserRoleDefine::AddFriendId), QVariant::fromValue(tmpInfo.m_strFriendId));
    roles.insert(int(UserRoleDefine::AddFriendImagePath), tmpInfo.m_strProfileImagePath);
    roles.insert(int(UserRoleDefine::AddFriednVerifyInfo), tmpInfo.m_strVerifyInfo);
    roles.insert(int(UserRoleDefine::AddFriendValid), tmpInfo.isValid);
    roles.insert(int(UserRoleDefine::AddFriendList), tmpInfo.comboxText);
    roles.insert(int(UserRoleDefine::AddFriendComboxRealValue), tmpInfo.m_strComBoxRealValue);

    return roles;
}

MyListModel::MyListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

MyListModel::~MyListModel()
{
}

void MyListModel::setData(std::vector<MyListModelStu>& vecListModelData)
{
    beginResetModel();

    m_vecModelData = vecListModelData;

    endResetModel();
}

void MyListModel::setDataList(const QList<std::map<int, QVariant>>& dataList)
{
    beginResetModel();

    m_dataList = dataList;

    endResetModel();
}

QVariant MyListModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    int iDataIndex = index.row();
    MyListModelStu tmpInfo = m_vecModelData[iDataIndex];
    if (Qt::UserRole < role)
    {
        if (MyListModelRole::Role::ListModelStr == role)
        {
            return QVariant::fromValue(tmpInfo.m_strInfo);
        }
        else if (MyListModelRole::Role::ListModelInt == role)
        {
            return QVariant::fromValue(tmpInfo.m_iIndex);
        }
        else if (MyListModelRole::Role::ListModeImage == role)
        {
            return QVariant::fromValue(tmpInfo.m_strImagePaht);
        }
    }
    return QVariant();
}

int MyListModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
    //return m_dataList.size();
    return m_vecModelData.size();
}

QMap<int, QVariant> MyListModel::itemData(const QModelIndex& index) const
{
    QMap<int, QVariant> roles;

    int iDataIndex = index.row();

    auto tmpInfo = m_vecModelData[iDataIndex];

    roles.insert(int(MyListModelRole::Role::ListModelInt), QVariant::fromValue(tmpInfo.m_iIndex));
    roles.insert(int(MyListModelRole::Role::ListModelStr), tmpInfo.m_strInfo);
    roles.insert(int(MyListModelRole::Role::ListModeImage), tmpInfo.m_strImagePaht);

    return roles;
}

QHash<int, QByteArray> MyListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[MyListModelRole::Role::ListModelStr] = "name";
    roles[MyListModelRole::Role::ListModelInt] = "suibian";
    roles[MyListModelRole::Role::ListModeImage] = "imagePath";
    return roles;
}


MyListViewDelegate::MyListViewDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

MyListViewDelegate::~MyListViewDelegate()
{
}

void MyListViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto info = index.data();
    //MyListModelStu tmp = info.value<MyListModelStu>();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::red));
    painter->drawRect(option.rect.x() + 30, option.rect.y() + 15, 240, 150);
    painter->setBrush(QBrush(Qt::green));
    painter->drawRoundedRect(option.rect.x() + 100, option.rect.y() + 75, 100, 50, 10, 10);
    painter->setPen(Qt::black);
    painter->drawText(option.rect.x() + 110, option.rect.y() + 85, index.data(MyListModelRole::Role::ListModelStr).toString());
}

QSize MyListViewDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(298, 180);
}

MySortModel::MySortModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
}

MySortModel::~MySortModel()
{
}

void MySortModel::setFilterMap(const std::map<int, QVariant>& mapFilter)
{
    m_mapFilter = mapFilter;
    invalidateFilter();
}

bool MySortModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    if (0 < m_mapFilter.size())
    {
        bool bMatch = false;
        //根据行、列和父对象获取模型中的数据
        //因为在筛选行，所以列为0
        QModelIndex sourceIndex = sourceModel()->index(source_row, 0, source_parent);
        for (auto& filterItem : m_mapFilter)
        {
            QVariant rule = filterItem.second;
            QVariant mode = sourceIndex.data(filterItem.first);
            //根据索引中对应的规则返回数据
            if (filterItem.second == sourceIndex.data(filterItem.first))
            {
                bMatch = true;
                break;
            }
        }
        return bMatch;
    }
    return true;
}

MyListView::MyListView(QWidget* parent)
    : QListView(parent)
{
    //setFlow(QListView::LeftToRight);
    setResizeMode(QListView::Adjust);
    //setViewMode(QListView::IconMode);
    setAttribute(Qt::WA_AcceptTouchEvents);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_ptrSortAndFilterModel = new MySortModel(this);
    m_ptrListModel = new ListModel(this);
    m_ptrDelegate = new DelegateForListView();
    m_ptrSortAndFilterModel->setSourceModel(m_ptrListModel);
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
    m_ptrListModel->setData(vecAddFriendInfo);
    setModel(m_ptrSortAndFilterModel);
    setItemDelegate(m_ptrDelegate);

    setMovement(QListView::Free);
    setDragDropMode(QAbstractItemView::DragOnly);
}

MyListView::~MyListView()
{
}

void MyListView::setViewFilterRule(const std::map<int, QVariant>& filterMap)
{
    if (m_ptrSortAndFilterModel)
    {
        m_ptrSortAndFilterModel->setFilterMap(filterMap);
    }
}

void MyListView::addToModel(const AddFriendInfo& friendInfo)
{
    m_ptrListModel->addData(friendInfo);
}

void MyListView::mousePressEvent(QMouseEvent* event)
{
    auto pressPoint = event->pos();
    auto item = indexAt(pressPoint);
    if (!item.isValid())
    {
        return;
    }
    int num = item.data(MyListModelRole::Role::ListModelInt).toInt();
    emit signalClicked(num);
}