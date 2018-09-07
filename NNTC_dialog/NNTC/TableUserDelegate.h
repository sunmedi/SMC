
#ifndef TableUserDelegate_H
#define TableUserDelegate_H

#include <QModelIndex>
#include <QPixmap>
#include <QSize>
#include <QSqlRelationalDelegate>

QT_FORWARD_DECLARE_CLASS(QPainter)

#include "qsqlrelationaldelegate.h"
class TableUserDelegate :
	public QSqlRelationalDelegate
{
public:
	TableUserDelegate(QObject *parent);
	~TableUserDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

	QSize sizeHint(const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

	bool editorEvent(QEvent *event, QAbstractItemModel *model,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) override;

	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const override;

private:
	QPixmap star;
};

#endif