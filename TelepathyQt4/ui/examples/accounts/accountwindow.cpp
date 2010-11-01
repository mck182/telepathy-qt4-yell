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

#include "accountwindow.h"

#include "TelepathyQt4/ui/examples/accounts/_gen/accountwindow.moc.hpp"

#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/PendingReady>

#include <TelepathyQt4/ui/FlatModelProxy>
#include <TelepathyQt4/ui/AvatarImageProvider>

#include <QAbstractItemModel>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QVBoxLayout>

#include "telepathy-initializer.h"

AccountWindow::AccountWindow()
    : QWidget(0)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    mView = new QDeclarativeView(this);
    mView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    layout->addWidget(mView);

    Tp::AccountManagerPtr am(Tp::AccountManager::create());
    TelepathyInitializer *initializer = new TelepathyInitializer(am);
    connect(initializer,
            SIGNAL(finished(TelepathyInitializer *)),
            SLOT(onInitializationFinished(TelepathyInitializer *)));
    initializer->run();

    mView->engine()->addImageProvider(
        QString::fromLatin1("avatars"),
        new Tp::AvatarImageProvider(am));
}

void AccountWindow::onInitializationFinished(TelepathyInitializer *initializer)
{
    QAbstractItemModel *contactModel =
        new Tp::FlatModelProxy(initializer->accountModel());
    mView->rootContext()->setContextProperty(
        QString::fromLatin1("accountsModel"),
        initializer->accountModel());
    mView->rootContext()->setContextProperty(
        QString::fromLatin1("contactsListModel"),
        contactModel);
    mView->setSource(QUrl::fromLocalFile(
        QString::fromLatin1("account-view.qml")));
}

