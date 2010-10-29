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

#include "tree-node.h"
#include "TelepathyQt4/ui/_gen/tree-node.moc.hpp"

TreeNode::TreeNode()
    : mParent(0)
{ }

TreeNode::~TreeNode()
{
    foreach (TreeNode *child, mChildren) {
        delete child;
    }
}

TreeNode *TreeNode::childAt(int index) const
{
    return mChildren[index];
}

void TreeNode::addChild(TreeNode *node)
{
    // takes ownership of node
    mChildren.append(node);
    node->mParent = this;

    // chain changed and removed signals
    connect(node,
            SIGNAL(changed(TreeNode *)),
            SIGNAL(changed(TreeNode *)));
    connect(node,
            SIGNAL(childrenAdded(TreeNode *, QList<TreeNode *>)),
            SIGNAL(childrenAdded(TreeNode *, QList<TreeNode *>)));
    connect(node,
            SIGNAL(childrenRemoved(TreeNode *, int, int)),
            SIGNAL(childrenRemoved(TreeNode *, int, int)));
}

int TreeNode::indexOf(TreeNode *node) const {
    return mChildren.indexOf(node);
}

int TreeNode::size() const {
    return mChildren.size();
}

TreeNode *TreeNode::parent() const
{
    return mParent;
}

QVariant TreeNode::data(int role) const
{
    return QVariant();
}

bool TreeNode::setData(int role, const QVariant &value)
{
    return false;
}

void TreeNode::remove()
{
    if (mParent) {
        mParent->mChildren.removeOne(this);
        disconnect(this,
                   SIGNAL(changed(TreeNode *)),
                   mParent,
                   SIGNAL(changed(TreeNode *)));
        disconnect(this,
                   SIGNAL(childrenAdded(TreeNode *, QList<TreeNode *>)),
                   mParent,
                   SIGNAL(childrenAdded(TreeNode *, QList<TreeNode *>)));
        disconnect(this,
                   SIGNAL(childrenRemoved(TreeNode *, int, int)),
                   mParent,
                   SIGNAL(childrenRemoved(TreeNode *, int, int)));
    }
    deleteLater();
}

