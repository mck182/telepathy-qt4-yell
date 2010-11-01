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

#include "tree-view.h"

#include "TelepathyQt4/ui/examples/treeview/_gen/tree-view.moc.hpp"

#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingContacts>
#include <TelepathyQt4/ContactManager>

#include <TelepathyQt4/ui/AccountsModel>

#include <QTreeView>
#include <QVBoxLayout>

#include "telepathy-initializer.h"

TreeView::TreeView(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    mView = new QTreeView(this);
    layout->addWidget(mView);

    Tp::AccountManagerPtr am(Tp::AccountManager::create());
    TelepathyInitializer *initializer = new TelepathyInitializer(am);
    connect(initializer,
            SIGNAL(finished(TelepathyInitializer *)),
            SLOT(onInitializationFinished(TelepathyInitializer *)));
    initializer->run();
}

void TreeView::onInitializationFinished(TelepathyInitializer *initializer)
{
    Tp::AccountsModel *model = initializer->accountModel();
    mView->setModel(model);
}

