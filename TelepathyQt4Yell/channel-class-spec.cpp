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

#include <TelepathyQt4Yell/ChannelClassSpec>

namespace Tpy
{

ChannelClassSpec::ChannelClassSpec()
    : Tp::ChannelClassSpec()
{
}

ChannelClassSpec::ChannelClassSpec(const Tp::ChannelClass &cc)
    : Tp::ChannelClassSpec(cc)
{
}

ChannelClassSpec::ChannelClassSpec(const QVariantMap &props)
    : Tp::ChannelClassSpec(props)
{
}

ChannelClassSpec::ChannelClassSpec(const QString &channelType, Tp::HandleType targetHandleType,
        const QVariantMap &otherProperties)
    : Tp::ChannelClassSpec(channelType, targetHandleType, otherProperties)
{
}

ChannelClassSpec::ChannelClassSpec(const QString &channelType, Tp::HandleType targetHandleType,
        bool requested, const QVariantMap &otherProperties)
    : Tp::ChannelClassSpec(channelType, targetHandleType, requested, otherProperties)
{
}

ChannelClassSpec::ChannelClassSpec(const ChannelClassSpec &other,
        const QVariantMap &additionalProperties)
    : Tp::ChannelClassSpec(other, additionalProperties)
{
}

ChannelClassSpec::~ChannelClassSpec()
{
}


ChannelClassSpec ChannelClassSpec::mediaCall(const QVariantMap &additionalProperties)
{
    static ChannelClassSpec spec;

    if (!spec.isValid()) {
        spec = ChannelClassSpec(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL, Tp::HandleTypeContact);
    }

    if (additionalProperties.isEmpty()) {
        return spec;
    } else {
        return ChannelClassSpec(spec, additionalProperties);
    }
}

ChannelClassSpec ChannelClassSpec::audioCall(const QVariantMap &additionalProperties)
{
    static ChannelClassSpec spec;

    if (!spec.isValid()) {
        spec = ChannelClassSpec(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL, Tp::HandleTypeContact);
        spec.setMediaCallInitialAudioFlag();
    }

    if (additionalProperties.isEmpty()) {
        return spec;
    } else {
        return ChannelClassSpec(spec, additionalProperties);
    }
}

ChannelClassSpec ChannelClassSpec::videoCall(const QVariantMap &additionalProperties)
{
    static ChannelClassSpec spec;

    if (!spec.isValid()) {
        spec = ChannelClassSpec(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL, Tp::HandleTypeContact);
        spec.setMediaCallInitialVideoFlag();
    }

    if (additionalProperties.isEmpty()) {
        return spec;
    } else {
        return ChannelClassSpec(spec, additionalProperties);
    }
}

ChannelClassSpec ChannelClassSpec::videoCallWithAudio(const QVariantMap &additionalProperties)
{
    static ChannelClassSpec spec;

    if (!spec.isValid()) {
        spec = ChannelClassSpec(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL, Tp::HandleTypeContact);
        spec.setMediaCallInitialAudioFlag();
        spec.setMediaCallInitialVideoFlag();
    }

    if (additionalProperties.isEmpty()) {
        return spec;
    } else {
        return ChannelClassSpec(spec, additionalProperties);
    }
}

} // Tpy
