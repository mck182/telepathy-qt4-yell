/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2008-2010 Collabora Ltd. <http://www.collabora.co.uk/>
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

#ifndef _TelepathyQt4_flat_model_proxy_h_HEADER_GUARD_
#define _TelepathyQt4_flat_model_proxy_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_HEADER
#error IN_TELEPATHY_QT4_HEADER
#endif

#include <QAbstractProxyModel>
#include <TelepathyQt4/Global>

namespace Tp
{

class TELEPATHY_QT4_EXPORT FlatModelProxy : public QAbstractProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
    
    FlatModelProxy(QAbstractItemModel *source);

    virtual QModelIndex mapFromSource(const QModelIndex &index) const;
    virtual QModelIndex mapToSource(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;

    int rowCount() const;

Q_SIGNALS:

    void rowCountChanged();

private Q_SLOTS:

    void onRowsAboutToBeInserted(const QModelIndex &index, int first, int last);
    void onRowsInserted(const QModelIndex &index, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex &index, int first, int last);
    void onRowsRemoved(const QModelIndex &index, int first, int last);

private:
    
    int offsetOf(int index) const;
};

}

#endif // _TelepathyQt4_flat_model_proxy_h_HEADER_GUARD_

