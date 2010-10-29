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

#ifndef _TelepathyQt4_tree_node_h_HEADER_GUARD_
#define _TelepathyQt4_tree_node_h_HEADER_GUARD_

#include <QObject>
#include <QVariant>

class TreeNode : public QObject
{
    Q_OBJECT
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
    void childrenAdded(TreeNode *parent, const QList<TreeNode *>& nodes);
    void childrenRemoved(TreeNode *parent, int first, int last);

protected:
    
    QList<TreeNode *> mChildren;
    TreeNode *mParent;
};

#endif // _TelepathyQt4_tree_node_h_HEADER_GUARD_

