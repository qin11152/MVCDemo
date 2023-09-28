#include "MyTableMVC.h"

#include <qevent.h>
#include <qpainter.h>
#include <qcombobox.h>

MyTableModel::MyTableModel(QObject* parent)
{
    m_vecAddFriendInfo = {};
}

void MyTableModel::setData(std::vector<AddFriendInfo>& addFriendInfo)
{
    beginResetModel();
    m_vecAddFriendInfo.clear();
    m_vecAddFriendInfo = addFriendInfo;
    endResetModel();
}

void MyTableModel::addData(const AddFriendInfo& addFriendInfo)
{
    beginInsertRows(QModelIndex(), 0, 0);
    m_vecAddFriendInfo.insert(m_vecAddFriendInfo.begin(), addFriendInfo);
    endInsertRows();
}

QVariant MyTableModel::data(const QModelIndex& index, int role) const
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

int MyTableModel::rowCount(QModelIndex const& parent) const
{
    return m_vecAddFriendInfo.size();
}

int MyTableModel::columnCount(const QModelIndex& parent) const
{
    return 4;
}

QMap<int, QVariant> MyTableModel::itemData(const QModelIndex& index) const
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

Qt::ItemFlags MyTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return  QAbstractItemModel::flags(index);
    }
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.column() == 3)
    {
        flags |= Qt::ItemIsUserCheckable;
    }
    if (index.column() == 2)
    {
        flags |= Qt::ItemIsDropEnabled;
        flags |= Qt::ItemIsEditable;  //可编辑，不然combobox无法下拉
    }
    return  flags;
}

bool MyTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid())
    {
        return false;
    }
    {
        switch (role)
        {
        case (int)UserRoleDefine::AddFriendId:
        {
            m_vecAddFriendInfo[index.row()].m_strFriendId = value.toString();
        }
        break;
        case (int)UserRoleDefine::AddFriendImagePath:
        {
            m_vecAddFriendInfo[index.row()].m_strProfileImagePath = value.toString();
        }
        break;
        case (int)UserRoleDefine::AddFriednVerifyInfo:
        {
            m_vecAddFriendInfo[index.row()].m_strVerifyInfo = value.toString();
        }
        break;
        case (int)UserRoleDefine::AddFriendValid:
        {
            m_vecAddFriendInfo[index.row()].isValid = value.toBool();
        }
        break;
        case (int)UserRoleDefine::AddFriendList:
        {
            m_vecAddFriendInfo[index.row()].comboxText = value.toStringList();
        }
        break;
        case (int)UserRoleDefine::AddFriendComboxRealValue:
        {
            m_vecAddFriendInfo[index.row()].m_strComBoxRealValue = value.toString();
        }
        break;
        default:
            return false;
        }
        return true;
    }
}

MyTableDelegate::MyTableDelegate(QObject* parent)
    : QStyledItemDelegate(parent),
    m_btn(new testBtn())
{
}

void MyTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);
    if (option.state.testFlag(QStyle::State_HasFocus))
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

    QStyledItemDelegate::paint(painter, viewOption, index);

    if (index.column() == 0)
    {
        QRect imageRect(option.rect.x() + 20, option.rect.y() + 10, 40, 40);
        QString strImagePath = index.data((int)UserRoleDefine::AddFriendImagePath).toString();
        painter->drawPixmap(imageRect, QPixmap(strImagePath));
    }
    else if (index.column() == 1)
    {
        QRect idRect(option.rect.x() + 10, option.rect.y() + 10, 100, 20);
        QString strId = index.data((int)UserRoleDefine::AddFriendId).toString();
        painter->drawText(idRect, strId);

        QString strVerifyInfo = index.data((int)UserRoleDefine::AddFriednVerifyInfo).toString();
        QRect verifyRect(option.rect.x() + 10, option.rect.y() + 30, 180, 30);
        painter->drawText(verifyRect, strVerifyInfo);
    }
    else if (index.column() == 2)
    {
        QRect comboxRect(option.rect.x() + 10, option.rect.y() + 10, 80, 40);
        QString strComboxText = index.data((int)UserRoleDefine::AddFriendComboxRealValue).toString();
        painter->drawText(comboxRect, strComboxText);
    }
    else if (index.column() == 3)
    {
        bool isValid = index.data((int)UserRoleDefine::AddFriendValid).toBool();
        if (!isValid)
        {
            QStyleOptionButton button;
            button.rect = QRect(option.rect.x() + 10, option.rect.y() + 10, 80, 40);
            button.state |= QStyle::State_None;
            button.text = QString::fromLocal8Bit("同意");

            QWidget* tmpBtn = m_btn.data();

            tmpBtn->style()->drawControl(QStyle::CE_PushButton, &button, painter, tmpBtn);
        }
        else
        {
            painter->drawText(QRect{ option.rect.x() + 10, option.rect.y() + 10, 80, 40 }, QString::fromLocal8Bit("已添加"));
        }
    }
}

bool MyTableDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    if (index.column() == 3)
    {
        QMouseEvent* pEvent = reinterpret_cast<QMouseEvent*> (event);
        QRect buttonRect = { option.rect.x() + 10, option.rect.y() + 10, 80, 40 };
        if (!buttonRect.contains(pEvent->pos()))
        {
            return false;
        }
        if (pEvent->button() == Qt::LeftButton)
        {
            qDebug() << index.data((int)UserRoleDefine::AddFriendId).toString();
            qDebug() << index.row();
        }
        return true;
    }
    else
    {
        return false;
    }
}

QSize MyTableDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize({ 800,100 });
}

QWidget* MyTableDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == 2)
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

void MyTableDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, (int)UserRoleDefine::AddFriendComboxRealValue).toString();
    QComboBox* box = static_cast<QComboBox*>(editor);
    box->setCurrentText(text);
}

void MyTableDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
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

void MyTableDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    editor->setGeometry(option.rect.x(), option.rect.y(), option.rect.width(), option.rect.height());
}
