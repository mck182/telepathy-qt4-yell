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

#ifndef _TelepathyQt4Yell_Models_tree_node_h_HEADER_GUARD_
#define _TelepathyQt4Yell_Models_tree_node_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_YELL_MODELS_HEADER
#error IN_TELEPATHY_QT4_YELL_MODELS_HEADER
#endif

#include <TelepathyQt4Yell/Models/Global>

#include <QObject>
#include <QVariant>

namespace Tpy
{

class TELEPATHY_QT4_YELL_MODELS_EXPORT TreeNode : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TreeNode)

public:
    TreeNode();

    virtual ~TreeNode();

    TreeNode *childAt(int index) const;

    void addChild(TreeNode *node);

    int indexOf(TreeNode *node) const;

    int size() const;

    TreeNode *parent() const;

    virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);
    virtual void remove();

Q_SIGNALS:
    void changed(TreeNode *);
    void childrenAdded(TreeNode *parent, const QList<TreeNode *> &nodes);
    void childrenRemoved(TreeNode *parent, int first, int last);

private:
    struct Private;
    friend struct Private;
    Private *mPriv;
};

}

#endif // _TelepathyQt4Yell_Models_tree_node_h_HEADER_GUARD_
