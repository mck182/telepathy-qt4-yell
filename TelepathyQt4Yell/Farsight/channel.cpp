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

#include <TelepathyQt4Yell/Farsight/Channel>

#include <TelepathyQt4/Channel>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4Yell/CallChannel>
#include <TelepathyQt4Yell/Farsight/global.h>

#include <telepathy-glib/channel.h>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/dbus.h>

#include "TelepathyQt4Yell/Farsight/_gen/channel.moc.hpp"

namespace Tpy
{

FarsightChannel::FarsightChannel() :
    mTfChannel(0)
{
}

FarsightChannel::~FarsightChannel()
{
}

void FarsightChannel::createFarsightChannel(const CallChannelPtr &channel)
{
    if (!channel->handlerStreamingRequired()) {
        qWarning() << "Handler streaming not required";
        Q_EMIT farsightChannelCreated(true);
        return;
    }

    TpDBusDaemon *dbus = tp_dbus_daemon_dup(0);
    if (!dbus) {
        qWarning() << "Unable to connect to D-Bus";
        Q_EMIT farsightChannelCreated(true);
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
        Q_EMIT farsightChannelCreated(true);
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
        Q_EMIT farsightChannelCreated(true);
        return;
    }

    tf_channel_new_async(gchannel, FarsightChannel::on_tf_channel_new_finish, this);
}

void FarsightChannel::on_tf_channel_new_finish (GObject *source_object, GAsyncResult *res, gpointer user_data)
{
    qDebug() << "FarsightChannel::on_tf_channel_new_finish: ";
    qDebug() << "    source_object=" << source_object;
    qDebug() << "    result=" << res;

    FarsightChannel * self = reinterpret_cast<FarsightChannel *> (user_data);

    GError *error = NULL;
    TfChannel *ret = (TfChannel *) TF_CHANNEL (g_async_initable_new_finish ( G_ASYNC_INITABLE (source_object), res, &error));
    self->mTfChannel = ret;

    if (error) {
	qDebug() << "FarsightChannel::on_tf_channel_new_finish: error " << error->message;
        g_clear_error (&error);
        Q_EMIT self->farsightChannelCreated(true);
    } else {
        Q_EMIT self->farsightChannelCreated(false);
    }

    g_object_unref(source_object);
}

TfChannel *FarsightChannel::farsightChannel()
{
    return mTfChannel;
}

} // Tpy
