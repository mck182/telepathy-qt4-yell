/*
 * This file is part of TelepathyQt4Yell
 *
 * Copyright © 2009-2011 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright © 2009 Nokia Corporation
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

#include <TelepathyQt4Yell/Farstream/Channel>

#include "TelepathyQt4Yell/Farstream/_gen/channel.moc.hpp"

#include <TelepathyQt4/Channel>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4Yell/CallChannel>
#include <TelepathyQt4Yell/Farstream/global.h>

#include <telepathy-glib/channel.h>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/dbus.h>

namespace Tpy
{

FarstreamChannel::FarstreamChannel()
    : mTfChannel(0)
{
}

FarstreamChannel::~FarstreamChannel()
{
}

void FarstreamChannel::createFarstreamChannel(const CallChannelPtr &channel)
{
    if (!channel->handlerStreamingRequired()) {
        qWarning() << "Handler streaming not required";
        Q_EMIT farstreamChannelCreated(0);
        return;
    }

    TpDBusDaemon *dbus = tp_dbus_daemon_dup(0);
    if (!dbus) {
        qWarning() << "Unable to connect to D-Bus";
        Q_EMIT farstreamChannelCreated(0);
        return;
    }

    Tp::ConnectionPtr connection = channel->connection();
    TpConnection *gconnection = tp_connection_new(dbus,
            connection->busName().toAscii(),
            connection->objectPath().toAscii(),
            0);
    g_object_unref(dbus);
    dbus = 0;
    if (!gconnection) {
        qWarning() << "Unable to construct TpConnection";
        Q_EMIT farstreamChannelCreated(0);
        return;
    }

    TpChannel *gchannel = tp_channel_new(gconnection,
            channel->objectPath().toAscii(),
            TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL.latin1(),
            (TpHandleType) channel->targetHandleType(),
            channel->targetHandle(),
            0);
    g_object_unref(gconnection);
    gconnection = 0;

    if (!gchannel) {
        qWarning() << "Unable to construct TpChannel";
        Q_EMIT farstreamChannelCreated(0);
        return;
    }

    tf_channel_new_async(gchannel, FarstreamChannel::onTfChannelNewFinish, this);
}

void FarstreamChannel::onTfChannelNewFinish(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
    qDebug() << "FarstreamChannel::onTfChannelNewFinish: ";
    qDebug() << "    source_object=" << source_object;
    qDebug() << "    result=" << res;

    FarstreamChannel *self = reinterpret_cast<FarstreamChannel *>(user_data);

    GError *error = NULL;
    TfChannel *ret = TF_CHANNEL(g_async_initable_new_finish(G_ASYNC_INITABLE(source_object), res, &error));

    if (error) {
        qDebug() << "FarstreamChannel::onTfChannelNewFinish: error " << error->message;
        g_clear_error(&error);
        Q_EMIT self->farstreamChannelCreated(0);
    } else {
        Q_EMIT self->farstreamChannelCreated(ret);
    }

    g_object_unref(source_object);
}

TfChannel *FarstreamChannel::farstreamChannel()
{
    return mTfChannel;
}

} // Tpy
