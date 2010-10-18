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

#ifndef _TelepathyQt4_chatwindow_h_HEADER_GUARD_
#define _TelepathyQt4_chatwindow_h_HEADER_GUARD_

#include <QScopedPointer>
#include <QWidget>

#include <TelepathyQt4/AbstractClientHandler>
#include <TelepathyQt4/Types>

#include <TelepathyQt4/ui/ConversationModel>

class QDeclarativeView;
class QLineEdit;

class ChatWindow : public QWidget, public Tp::AbstractClientHandler
{
    Q_OBJECT
public:
    ChatWindow(QWidget *parent = 0);

    virtual void handleChannels(const Tp::MethodInvocationContextPtr<> &context,
                                const Tp::AccountPtr &account,
                                const Tp::ConnectionPtr &connection,
                                const QList<Tp::ChannelPtr> &channels,
                                const QList<Tp::ChannelRequestPtr> &requestsSatisfied,
                                const QDateTime &userActionTime,
                                const QVariantMap &handlerInfo);

    bool bypassApproval() const { return false; }
private:

    QScopedPointer<Tp::ConversationModel> mModel;
    QDeclarativeView *mConversation;
    QLineEdit *mInput;

    void initialize(const Tp::TextChannelPtr &channel);

    Tp::ChannelClassList channelClassList() {
        Tp::ChannelClassList filters;
        QMap<QString, QDBusVariant> filter;
        filter.insert(QString::fromLatin1(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"),
                      QDBusVariant(QString::fromLatin1(TELEPATHY_INTERFACE_CHANNEL_TYPE_TEXT)));
        filter.insert(QString::fromLatin1(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"),
                      QDBusVariant((uint) Tp::HandleTypeContact));
        filters.append(filter);

        filter.clear();
        filter.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"),
                      QDBusVariant(QString::fromLatin1(TELEPATHY_INTERFACE_CHANNEL_TYPE_TEXT)));
        filter.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"),
                      QDBusVariant((uint) Tp::HandleTypeRoom));
        filters.append(filter);

        return filters;
    }

private Q_SLOTS:

    void onReturnPressed();

};

#endif // _TelepathyQt4_chatwindow_h_HEADER_GUARD_

