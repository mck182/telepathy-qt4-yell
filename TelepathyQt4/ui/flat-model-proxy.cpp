/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2010 Collabora Ltd. <http://www.collabora.co.uk/>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <TelepathyQt4/ui/FlatModelProxy>

#include "TelepathyQt4/ui/_gen/flat-model-proxy.moc.hpp"

namespace Tp
{

FlatModelProxy::FlatModelProxy(QAbstractItemModel *source)
    : QAbstractProxyModel(source)
{
    setSourceModel(source);

    connect(source,
            SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)),
            SLOT(onRowsAboutToBeInserted(QModelIndex, int, int)));
    connect(source,
            SIGNAL(rowsInserted(QModelIndex, int, int)),
            SLOT(onRowsInserted(QModelIndex, int, int)));
    connect(source,
            SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
            SLOT(onRowsAboutToBeRemoved(QModelIndex, int, int)));
    connect(source,
            SIGNAL(rowsRemoved(QModelIndex, int, int)),
            SLOT(onRowsRemoved(QModelIndex, int, int)));
}

QModelIndex FlatModelProxy::mapFromSource(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    QModelIndex parent = index.parent();

    if (!parent.isValid()) {
        return QModelIndex();
    }

    return createIndex(offsetOf(parent.row()) + index.row(), index.column(), parent.row());
}

QModelIndex FlatModelProxy::mapToSource(const QModelIndex &index) const
{
    int parentRow = index.internalId();
    QModelIndex parent = sourceModel()->index(parentRow, 0, QModelIndex());
    int row = index.row() - offsetOf(parent.row());
    return sourceModel()->index(row, index.column(), parent);
}

QModelIndex FlatModelProxy::index(int row, int column, const QModelIndex &parent) const
{
    int count = 0;
    for (int i = 0; i < sourceModel()->rowCount(QModelIndex()); i++) {
        QModelIndex sourceIndex = sourceModel()->index(i, 0, QModelIndex());
        count += sourceModel()->rowCount(sourceIndex);
        if (row < count) {
            return createIndex(row, column, i);
        }
    }

    return QModelIndex();
}

QModelIndex FlatModelProxy::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int FlatModelProxy::columnCount(const QModelIndex &parent) const
{
    return 1;
}

int FlatModelProxy::rowCount() const
{
    return rowCount(QModelIndex());
}

int FlatModelProxy::rowCount(const QModelIndex &parent) const
{
    return offsetOf(sourceModel()->rowCount(QModelIndex()));
}

int FlatModelProxy::offsetOf(int index) const
{
    int offset = 0;
    for (int i = 0; i < index; i++) {
        offset += sourceModel()->rowCount(sourceModel()->index(i, 0, QModelIndex()));
    }
    return offset;
}

void FlatModelProxy::onRowsAboutToBeInserted(const QModelIndex &index, int first, int last)
{
    if (index.isValid()) {
        int offset = offsetOf(index.row());
        int firstIndex = offset + first;
        int lastIndex = offset + last;

        beginInsertRows(QModelIndex(), firstIndex, lastIndex);
    }
}

void FlatModelProxy::onRowsAboutToBeRemoved(const QModelIndex &index, int first, int last)
{
    if (index.isValid()) {
        int offset = offsetOf(index.row());
        int firstIndex = offset + first;
        int lastIndex = offset + last;

        beginRemoveRows(QModelIndex(), firstIndex, lastIndex);
    }
}

void FlatModelProxy::onRowsInserted(const QModelIndex &index, int first, int last)
{
    if (index.isValid()) {
        endInsertRows();
    }
}

void FlatModelProxy::onRowsRemoved(const QModelIndex &index, int first, int last)
{
    if (index.isValid()) {
        endRemoveRows();
    }
}

}

