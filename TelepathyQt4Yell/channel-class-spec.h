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

#ifndef _TelepathyQt4Yell_channel_class_spec_h_HEADER_GUARD_
#define _TelepathyQt4Yell_channel_class_spec_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_YELL_HEADER
#error IN_TELEPATHY_QT4_YELL_HEADER
#endif

#include <TelepathyQt4Yell/Constants>
#include <TelepathyQt4Yell/Global>

#include <TelepathyQt4/ChannelClassSpec>

namespace Tpy
{

class TELEPATHY_QT4_YELL_EXPORT ChannelClassSpec : public Tp::ChannelClassSpec
{
public:
    ChannelClassSpec();
    ChannelClassSpec(const Tp::ChannelClass &cc);
    ChannelClassSpec(const QVariantMap &props);
    ChannelClassSpec(const QString &channelType, Tp::HandleType targetHandleType,
            const QVariantMap &otherProperties = QVariantMap());
    ChannelClassSpec(const QString &channelType, Tp::HandleType targetHandleType, bool requested,
            const QVariantMap &otherProperties = QVariantMap());
    ChannelClassSpec(const ChannelClassSpec &other,
            const QVariantMap &additionalProperties = QVariantMap());
    ~ChannelClassSpec();

    bool hasMediaCallInitialAudioFlag() const
    {
        return qdbus_cast<bool>(
                property(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialAudio")));
    }

    void setMediaCallInitialAudioFlag()
    {
        setProperty(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialAudio"),
                QVariant::fromValue(true));
    }

    void unsetMediaCallInitialAudioFlag()
    {
        unsetProperty(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialAudio"));
    }

    bool hasMediaCallInitialVideoFlag() const
    {
        return qdbus_cast<bool>(
                property(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialVideo")));
    }

    void setMediaCallInitialVideoFlag()
    {
        setProperty(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialVideo"),
                QVariant::fromValue(true));
    }

    void unsetMediaCallInitialVideoFlag()
    {
        unsetProperty(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialVideo"));
    }

    static ChannelClassSpec mediaCall(const QVariantMap &additionalProperties = QVariantMap());
    static ChannelClassSpec audioCall(const QVariantMap &additionalProperties =
            QVariantMap());
    static ChannelClassSpec videoCall(const QVariantMap &additionalProperties =
            QVariantMap());
    static ChannelClassSpec videoCallWithAudio(const QVariantMap &additionalProperties =
            QVariantMap());
};

} // Tpy

Q_DECLARE_METATYPE(Tpy::ChannelClassSpec);

#endif
