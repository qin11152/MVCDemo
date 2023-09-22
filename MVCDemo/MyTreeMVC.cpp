#include "MyTreeMVC.h"
#include <qpainter.h>
#include <qdebug.h>

MyTreeView::MyTreeView(QWidget *parent)
    : QTreeView(parent)
{}

MyTreeView::~MyTreeView()
{}

TreeItem::TreeItem(void* data, TreeItem* parent)
    : m_data(data)
    , m_pParent(parent)
{
    if (nullptr == m_pParent)
    {
        m_level = 0;
    }
    else
    {
        m_level = m_pParent->level() + 1;
    }
}

TreeItem::~TreeItem()
{
    if (m_data)
    {
        delete m_data;
        m_data = nullptr;
    }
}

void TreeItem::appendChild(TreeItem* pChild)
{
    m_children.append(pChild);
}

void TreeItem::insertChild(int idx, TreeItem* pChild)
{
    m_children.insert(idx, pChild);
}

void TreeItem::removeChild(TreeItem* p)
{
    int index = m_children.indexOf(p);
    if (index < 0) 
    { 
        return;
    }
    m_children.removeAt(index);
    delete p;
}

QVariant TreeItem::data(int column)
{
    if(1==m_level)
    {
        auto ptr = reinterpret_cast<Class*>(m_data);
        if (0 == column)
        {
            return ptr->m_strClass.c_str();
        }
        else
        {
            return QVariant();
        }
    }
    else
    {
        auto ptr = reinterpret_cast<Student*>(m_data);
        switch (column)
        {
        case 1:
        {
            return ptr->name.c_str();
        }
        break;
        case 2:
        {
            return ptr->age;
        }
        break;
        case 3:
        {
            return ptr->score;
        }
        break;
        default:
            return QVariant();
            break;
        }
    }
    return QVariant();
}

void TreeItem::setData(void* data)
{
    m_data = data;
}

int TreeItem::childCount()
{
    return m_children.size();
}

int TreeItem::level() const
{
    return m_level;
}

void TreeItem::setLevel(int level)
{
    m_level = level;
}

QList<TreeItem*>& TreeItem::children()
{
    return m_children;
}

TreeItem* TreeItem::parent()
{
    return m_pParent;
}

TreeItem* TreeItem::child(int row)
{
    if (row < 0 || row >= childCount()) 
    { 
        return nullptr;
    }
    return m_children[row];
}

int TreeItem::row()
{
    if (m_pParent)
    {
        return m_pParent->children().indexOf(this);
    }
    return 0;
}

MyTreeModel::MyTreeModel(QStringList header, QObject* parent)
    :QAbstractItemModel(parent),
    m_header(header)
{
    m_pRootItem = new TreeItem(nullptr, nullptr);
}

MyTreeModel::~MyTreeModel()
{
    delete m_pRootItem;
}

QVariant MyTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) 
    { 
        return QVariant();
    }
    TreeItem* p = static_cast<TreeItem*>(index.internalPointer());
    return p->data(index.column());
}

int MyTreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* p = m_pRootItem;
    if (parent.isValid()) 
    {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    return p->childCount();
}

int MyTreeModel::columnCount(const QModelIndex& parent) const
{
    return m_header.size();
}

//Qt::ItemFlags MyTreeModel::flags(const QModelIndex& index) const
//{
//    if (!index.isValid()) 
//    {
//        return Qt::ItemIsEnabled;
//    }
//    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
//        | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
//}

bool MyTreeModel::setData(const QModelIndex& index, void* data, int role)
{
    TreeItem* p = m_pRootItem;
    if (index.isValid()) 
    {
        p = static_cast<TreeItem*>(index.internalPointer());
    }

    p->setData(data);
    emit dataChanged(index, index);
    return true;
}

bool MyTreeModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count - 1);
    TreeItem* p = m_pRootItem;
    if (parent.isValid()) 
    {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    p->insertChild(row, new TreeItem(nullptr, p));
    endInsertRows();
    return true;
}

bool MyTreeModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    TreeItem* p = m_pRootItem;
    if (parent.isValid()) 
    {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    p->removeChild(p->child(row));
    endRemoveRows();
    return true;
}

QModelIndex MyTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent)) 
    { 
        return QModelIndex();
    }

    TreeItem* p = m_pRootItem;
    if (parent.isValid()) 
    {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* pChild = p->child(row);
    if (pChild) 
    {
        return createIndex(row, column, pChild);
    }
    return QModelIndex();
}

QModelIndex MyTreeModel::parent(const QModelIndex& child) const
{
    if (!child.isValid()) 
    { 
        return QModelIndex();
    }
    TreeItem* c = static_cast<TreeItem*>(child.internalPointer());
    TreeItem* p = c->parent();
    if (p == m_pRootItem) 
    { 
        return QModelIndex();
    }
    return createIndex(p->row(), 0, p);
}

QVariant MyTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return m_header.value(section);
    }
    return QVariant();
}

QModelIndex MyTreeModel::addTree(void* data, const QModelIndex& parent)
{
    TreeItem* p = m_pRootItem;
    if (parent.isValid()) 
    {
        p = static_cast<TreeItem*>(parent.internalPointer());
    }
    int row = p->childCount();

    //先插入了一个qmodelindex，并且new了一个item放入了vector中
    insertRows(row, 1, parent);	// row 追加

    //这里createindex时把item指针给到了modelindex
    QModelIndex index = this->index(row, 0, parent);
    //把data指针给到item，也可以在前边构造的时候给
    setData(index, data, Qt::DisplayRole);
    return index;
}

void MyTreeModel::removeTree(QModelIndex& parent)
{
    if (!parent.isValid())
    { 
        return; 
    }
    removeRows(parent.row(), 1, parent.parent());
}

TreeItem* MyTreeModel::getRootItem() const
{
    return m_pRootItem;
}

MyStyledItemDelegate::MyStyledItemDelegate(QObject* parent)
{
    m_ptrTree = reinterpret_cast<QTreeView*>(parent);
}

void MyStyledItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.isValid())
    {
        //保存按钮状态
        painter->save();
        QStyleOptionViewItem viewOption(option);
        //initStyleOption(&viewOption, index);
        if (option.state.testFlag(QStyle::State_HasFocus))
            viewOption.state = viewOption.state ^ QStyle::State_HasFocus;

        //QStyledItemDelegate::paint(painter, viewOption, index);

        auto displayOption = option;
        displayOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

        //抗锯齿绘图
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter->fillRect(option.rect, Qt::NoBrush);

        TreeItem* ptr = reinterpret_cast<TreeItem*>(index.internalPointer());
        if (ptr)
        {
            QTreeView* tree = reinterpret_cast<QTreeView*>(parent());
            if (1 == ptr->level())
            {
                if (0 == index.column())
                {
                    QString strClass = index.data(0).toString();
                    QRect nameRect{ option.rect.x() + 20, option.rect.y() + 5, 40,40 };
                    painter->drawText(nameRect, Qt::AlignVCenter, strClass);
                }
            }
            else if (2 == ptr->level())
            {
                if (1 == index.column())
                {
                    QString name = index.data(1).toString();
                    QRect nameRect{ option.rect.x() + 20, option.rect.y() + 5, 160,40 };
                    painter->drawText(nameRect, Qt::AlignVCenter, name);
                }
                else if (2 == index.column())
                {
                    int age = index.data(2).toInt();
                    QRect ageRect{ option.rect.x() + 20, option.rect.y() + 5, 60,40 };
                    painter->drawText(ageRect, Qt::AlignVCenter, QString::number(age));
                    QRect imageRect{ option.rect.x() + 20 + 80,option.rect.y() + 5,40,40 };
                    painter->drawPixmap(imageRect, QPixmap("D:/1.png"));
                }
                else if (3 == index.column())
                {
                    const QWidget* widget = option.widget;
                    int score = index.data(3).toInt();
                    QRect scoreRect{ option.rect.x() + 20, option.rect.y() + 5, 160,40 };
                    painter->drawText(scoreRect, Qt::AlignVCenter, QString::number(score));
                }
            }
        }

        painter->restore();//恢复painter
    }
}

QSize MyStyledItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return{ 800,50 };
}

bool MyStyledItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return false;
}

