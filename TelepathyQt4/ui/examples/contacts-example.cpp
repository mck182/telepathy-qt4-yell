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

#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <TelepathyQt4/AccountSet>
#include <TelepathyQt4/ui/ContactsListModel>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Connection>

#include "contacts-connection.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Tp::registerTypes();

    Tp::AccountManagerPtr am(Tp::AccountManager::create());

    Tp::ContactsListModel* contactsModel = new Tp::ContactsListModel(am);

    QDeclarativeView ContactsView;
    ContactsView.rootContext()->setContextProperty(QString::fromLatin1("contactsListModel"), contactsModel);
    ContactsView.setSource(QUrl::fromLocalFile(QString::fromLatin1("ContactsView.qml")));
    ContactsView.show();


    app.exec();
}
