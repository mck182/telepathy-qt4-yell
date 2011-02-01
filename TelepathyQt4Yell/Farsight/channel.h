/*
 * This file is part of TelepathyQt4Yell
 *
 * Copyright (C) 2009-2011 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2009 Nokia Corporation
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

#ifndef _TelepathyQt4Yell_Farsight_channel_h_HEADER_GUARD_
#define _TelepathyQt4Yell_Farsight_channel_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_YELL_FARSIGHT_HEADER
#error IN_TELEPATHY_QT4_YELL_FARSIGHT_HEADER
#endif

#include <QObject>

#include <TelepathyQt4Yell/Farsight/global.h>

#include <TelepathyQt4Yell/Types>

#include <gst/gst.h>
#include <telepathy-farsight/channel.h>

namespace Tpy
{

class TELEPATHY_QT4_YELL_FS_EXPORT FarsightChannel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FarsightChannel)

public:
    FarsightChannel();
    virtual ~FarsightChannel();

    void createFarsightChannel(const CallChannelPtr &channel);
    TfChannel *farsightChannel();

Q_SIGNALS:
    void farsightChannelCreated(bool error);

private:
    static void onTfChannelNewFinish(GObject *source_object, GAsyncResult *res, gpointer user_data);

    TfChannel *mTfChannel;
};

} // Tpy

#endif
