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

#include "chatwindow.h"
#include "TelepathyQt4/ui/examples/conversation/_gen/chatwindow.moc.hpp"

#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QLineEdit>
#include <QVBoxLayout>

#include <TelepathyQt4/Connection>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/TextChannel>
#include <TelepathyQt4/PendingContacts>
#include <TelepathyQt4/PendingReady>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent)
    , AbstractClientHandler(channelClassList())
{
    resize(800, 500);
}

void ChatWindow::initialize(const Tp::TextChannelPtr &channel)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    mConversation = new QDeclarativeView(this);
    mInput = new QLineEdit(this);

    layout->addWidget(mConversation);
    layout->addWidget(mInput);

    mModel.reset(new Tp::ConversationModel(channel));
    mConversation->rootContext()->setContextProperty(QString::fromLatin1("conversationModel"),
                                                     mModel.data());
    mConversation->setSource(QUrl::fromLocalFile(QString::fromLatin1("conversation.qml")));
    mConversation->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    connect(mInput, SIGNAL(returnPressed()), SLOT(onReturnPressed()));

    show();
    mContext->setFinished();
}

void ChatWindow::initializeConnection(const Tp::ConnectionPtr &connection)
{
    Tp::Features features;
    features << Tp::Connection::FeatureCore
             << Tp::Connection::FeatureSelfContact
             << Tp::Connection::FeatureSimplePresence
             << Tp::Connection::FeatureRoster
             << Tp::Connection::FeatureRosterGroups
             << Tp::Connection::FeatureAccountBalance;
    connect(connection->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onConnectionReady(Tp::PendingOperation *)));
}

void ChatWindow::initializeContacts(Tp::ContactManager *contactManager)
{
    QList<Tp::ContactPtr> contacts = contactManager->allKnownContacts().toList();
    qDebug() << contacts.size() << "known contacts";

    QSet<Tp::Contact::Feature> features;
    features << Tp::Contact::FeatureAlias
             << Tp::Contact::FeatureAvatarToken
             << Tp::Contact::FeatureSimplePresence
             << Tp::Contact::FeatureCapabilities
             << Tp::Contact::FeatureLocation
             << Tp::Contact::FeatureInfo
             << Tp::Contact::FeatureAvatarData;

    connect(contactManager->upgradeContacts(contacts, features),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onContactsUpgraded(Tp::PendingOperation *)));
}

void ChatWindow::handleChannels(const Tp::MethodInvocationContextPtr<> &context,
                                const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection,
                                const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied,
                                const QDateTime &userActionTime,
                                const QVariantMap &handlerInfo)
{
    qDebug() << "handling channel";
    if (channels.size() == 1) {
        mChannel = Tp::TextChannelPtr::dynamicCast(channels[0]);
        mConnection = connection;
        mContext = context;
        initializeConnection(mConnection);
    }
    else {
        qDebug() << "more than 1 channel";
    }
}

void ChatWindow::onReturnPressed()
{
    if (mModel) {
        mModel->sendMessage(mInput->text());
        mInput->setText(QString());
    }
}

void ChatWindow::onContactsUpgraded(Tp::PendingOperation *)
{
    initialize(mChannel);
}

void ChatWindow::onConnectionReady(Tp::PendingOperation *)
{
    initializeContacts(mConnection->contactManager());
}

