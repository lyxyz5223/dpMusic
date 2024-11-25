#pragma once
#include <qabstractitemmodel.h>
#include <qmap.h>
class MyMusicsListViewItemModel :
    public QAbstractListModel
{
public:
	int rowCount(const QModelIndex& parent) const {
		return list.size();
	}
	int columnCount(const QModelIndex& parent) const {
		return columnName.size();
	}
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const {
		if (!index.isValid())
			return QVariant();
		return list[index.row()][role];
	}
	void setData(const QMap<int/*row*/, QMap<int/*role*/, QVariant/*content*/>> dataList) {
		list = dataList;
	}
	void addRow(const QStringList& strList) {
		columnName.insert(0, "title");
		columnName.insert(1, "artists");
		columnName.insert(2, "albumTitle");
		columnName.insert(3, "length");
		QMap<int, QVariant> tmp;
		for (int i = 0; i < strList.size(); i++)
			tmp.insert(i, strList[i]);
		list[list.count()] = tmp;
	}
private:
	QMap<int/*row*/, QMap<int/*role*/, QVariant/*content*/>> list;
	QMap<int/*role*/, QString/*name*/> columnName;
};

