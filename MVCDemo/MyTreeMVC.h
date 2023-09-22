#pragma once

#include <vector>

#include <QTreeView>
#include <QStyledItemDelegate>
#include <QItemDelegate>

class Student
{
public:
	Student(std::string name = "", int score = 0, int age = 0):
        name(name),
        score(score),
		age(age)
    {}
	std::string name{ "" };
	int score{ 0 };
	int age{ 0 };
};

class Class
{
public:
	Class(std::string strClass):
		m_strClass(strClass)
	{}
	~Class() 
	{
		for (auto iter = m_vecStudent.begin(); iter != m_vecStudent.end();)
		{
			delete (*iter);
		}
	}
	std::string m_strClass{ "" };
	std::vector<Student*> m_vecStudent;
};

class TreeItem
{
public:
	//TreeItem();
	TreeItem(void* data, TreeItem* parent);
	~TreeItem();

	void appendChild(TreeItem* pChild);
	void insertChild(int idx, TreeItem* pChild);
	void removeChild(TreeItem* p);

	QVariant data(int column);
	void setData(void* data);
	int childCount();
	//设置、获取节点是几级节点（就是树的层级）
	int level() const;
	void setLevel(int level);

	QList<TreeItem*>& children();
	TreeItem* parent();
	TreeItem* child(int row);
	int row();

private:
	QString m_str;
	TreeItem* m_pParent;
	QList<TreeItem*> m_children;
	void* m_data;
	int m_level;     //该节点是第几级节点
};

class MyTreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	MyTreeModel(QStringList header, QObject* parent = nullptr);
	~MyTreeModel();

	QVariant data(const QModelIndex& index, int role) const override;
	int rowCount(const QModelIndex& parent) const override;
	int columnCount(const QModelIndex& parent) const;
	//Qt::ItemFlags flags(const QModelIndex& index) const override;
	bool setData(const QModelIndex& index, void* data, int role);
	bool insertRows(int row, int count, const QModelIndex& parent);
	bool removeRows(int row, int count, const QModelIndex& parent);
	QModelIndex index(int row, int column, const QModelIndex& parent) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	QModelIndex addTree(void* data, const QModelIndex& parent);
	void removeTree(QModelIndex& parent);
	TreeItem* getRootItem()const;

private:
	QStringList m_header;   //表头内容
	TreeItem* m_pRootItem;
};

class MyStyledItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	MyStyledItemDelegate(QObject* parent = nullptr);
	~MyStyledItemDelegate() = default;

protected:
	// 绘制item
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	virtual bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;

private:
	QTreeView* m_ptrTree{ nullptr };
};

class MyTreeView  : public QTreeView
{
    Q_OBJECT

public:
	MyTreeView(QWidget* parent = nullptr);
    ~MyTreeView();
};
