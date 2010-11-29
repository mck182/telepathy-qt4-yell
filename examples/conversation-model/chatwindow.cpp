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

#include "chatwindow.h"
#include "examples/conversation-model/_gen/chatwindow.moc.hpp"

#include <TelepathyQt4/ChannelClassSpecList>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/TextChannel>
#include <TelepathyQt4/PendingContacts>
#include <TelepathyQt4/PendingReady>

#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QLineEdit>
#include <QVBoxLayout>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent),
      AbstractClientHandler(Tp::ChannelClassSpecList(channelClassList()))
{
    resize(800, 500);
}

void ChatWindow::initialize(Tp::ConversationModel *model)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    mConversation = new QDeclarativeView(this);
    mInput = new QLineEdit(this);

    layout->addWidget(mConversation);
    layout->addWidget(mInput);

    mModel.reset(model);
    mConversation->rootContext()->setContextProperty(QString::fromLatin1("conversationModel"),
                                                     mModel.data());
    mConversation->setSource(QUrl::fromLocalFile(QString::fromLatin1("conversation.qml")));
    mConversation->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    connect(mInput, SIGNAL(returnPressed()), SLOT(onReturnPressed()));

    show();
}

void ChatWindow::handleChannels(const Tp::MethodInvocationContextPtr<> &context,
                                const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection,
                                const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied,
                                const QDateTime &userActionTime,
                                const Tp::AbstractClientHandler::HandlerInfo &handlerInfo)
{
    if (channels.size() == 1) {
        Tp::TextChannelPtr channel = Tp::TextChannelPtr::dynamicCast(channels[0]);
        TelepathyInitializer *initializer = new TelepathyInitializer(
            connection, channel, context);
        connect(initializer,
                SIGNAL(finished(Tp::ConversationModel *)),
                SLOT(initialize(Tp::ConversationModel *)));
        initializer->run();
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

TelepathyInitializer::TelepathyInitializer(const Tp::ConnectionPtr &connection,
                                           const Tp::TextChannelPtr &channel,
                                           const Tp::MethodInvocationContextPtr<> &context)
    : mConnection(connection),
      mChannel(channel),
      mContext(context),
      mModel(0)
{
}

void TelepathyInitializer::run()
{
    initializeConnection();
}

void TelepathyInitializer::onContactsUpgraded(Tp::PendingOperation *)
{
    Tp::ConversationModel *model =
        new Tp::ConversationModel(mConnection->selfContact(), mChannel);
    emit finished(model);
    mContext->setFinished();
    deleteLater();
}

void TelepathyInitializer::onConnectionReady(Tp::PendingOperation *)
{
    initializeChannel();
}

void TelepathyInitializer::onChannelReady(Tp::PendingOperation *)
{
    initializeContacts();
}

void TelepathyInitializer::initializeConnection()
{
    Tp::Features features;
    features << Tp::Connection::FeatureCore
             << Tp::Connection::FeatureSelfContact
             << Tp::Connection::FeatureSimplePresence
             << Tp::Connection::FeatureRoster
             << Tp::Connection::FeatureRosterGroups
             << Tp::Connection::FeatureAccountBalance;
    connect(mConnection->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onConnectionReady(Tp::PendingOperation *)));
}

void TelepathyInitializer::initializeContacts()
{
    Tp::ContactManagerPtr contactManager = mConnection->contactManager();

    QList<Tp::ContactPtr> contacts = mChannel->groupContacts().toList();

    Tp::Features features;
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

void TelepathyInitializer::initializeChannel()
{
    Tp::Features features;
    features << Tp::TextChannel::FeatureMessageQueue
             << Tp::TextChannel::FeatureChatState
             << Tp::Channel::FeatureCore;
    connect(mChannel->becomeReady(features),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onChannelReady(Tp::PendingOperation *)));
}

