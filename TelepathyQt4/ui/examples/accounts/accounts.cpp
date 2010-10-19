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
#include <QHBoxLayout>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <qdeclarative.h>
#include <QDeclarativeView>
#include <TelepathyQt4/ui/AccountModel>
#include <TelepathyQt4/ui/ContactsListModel>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    Tp::registerTypes();

    Tp::AccountManagerPtr am(Tp::AccountManager::create());
    Tp::AccountModel* model = new Tp::AccountModel(am);
    Tp::ContactsListModel* contactsModel = new Tp::ContactsListModel(am);

    QWidget main;
    QHBoxLayout* layout = new QHBoxLayout(&main);

    QDeclarativeView* view = new QDeclarativeView(&main);
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);

    layout->addWidget(view);

    view->rootContext()->setContextProperty(QString::fromLatin1("accountsModel"), model);
    view->setSource(QUrl::fromLocalFile(QString::fromLatin1("account-view.qml")));
    view->rootContext()->setContextProperty(QString::fromLatin1("contactsListModel"), contactsModel);

    main.resize(500, 500);
    main.show();

    app.exec();
}
