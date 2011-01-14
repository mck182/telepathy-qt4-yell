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

#ifndef _TelepathyQt4_Models_conversation_model_h_HEADER_GUARD_
#define _TelepathyQt4_Models_conversation_model_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_HEADER
#error IN_TELEPATHY_QT4_HEADER
#endif

#include <TelepathyQt4Yell/Models/Global>

#include <TelepathyQt4/TextChannel>
#include <TelepathyQt4/Types>

#include <QAbstractListModel>

namespace Tp
{

class ConversationItem;

class TELEPATHY_QT4_MODELS_EXPORT ConversationModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ConversationModel)

public:
    enum Role {
        TextRole = Qt::UserRole,
        ContactRole,
        ContactAvatarRole,
        TimeRole,
        TypeRole,
        ItemRole
    };

    explicit ConversationModel(const ContactPtr &self, const TextChannelPtr &channel, QObject *parent = 0);
    virtual ~ConversationModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE void sendMessage(const QString &text);
    Q_INVOKABLE void disconnectChannelQueue();
    Q_INVOKABLE void connectChannelQueue();


    ContactPtr selfContact() const;

protected:
    // work around moc namespace limitations
    typedef Tp::ChannelChatState ChannelChatState;

    void addItem(const ConversationItem *item);
    bool deleteItem(const ConversationItem *item);

    QModelIndex index(const ConversationItem *item) const;

protected Q_SLOTS:
    virtual void onChatStateChanged(const Tp::ContactPtr &contact, Tp::ChannelChatState state);

private Q_SLOTS:
    void onChannelReady(Tp::PendingOperation *op);
    void onMessageReceived(const Tp::ReceivedMessage &message);

private:
    struct Private;
    friend struct Private;
    Private *mPriv;
};

}

#endif // _TelepathyQt4_Models_conversation_model_h_HEADER_GUARD_
