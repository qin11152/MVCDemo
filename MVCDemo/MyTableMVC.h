#pragma once

#include "MyListMVC.h"

#include <QAbstractTableModel>
#include <QDebug>

class MyTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MyTableModel(QObject* parent = nullptr);
    ~MyTableModel()=default;

    void setData(std::vector<AddFriendInfo>& addFriendInfo);
    void addData(const AddFriendInfo& addFriendInfo);

protected:
    QVariant data(const QModelIndex& index, int role) const override;
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex())const override;
    QMap<int, QVariant> itemData(const QModelIndex& index)const override;
    Qt::ItemFlags flags(const QModelIndex& index)const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole)override;

private:
    std::vector<AddFriendInfo> m_vecAddFriendInfo;
};

#include <QStyledItemDelegate>

class MyTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    MyTableDelegate(QObject* parent = nullptr);
    ~MyTableDelegate() = default;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

    void setEditorData(QWidget* editor, const QModelIndex& index)const override;

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index)const override;

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index)const override;

private:
    QScopedPointer<testBtn> m_btn;
};
