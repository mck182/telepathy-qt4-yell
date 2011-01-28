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

#include <TelepathyQt4Yell/ChannelFactory>

#include <TelepathyQt4Yell/CallChannel>
#include <TelepathyQt4Yell/ChannelClassSpec>

namespace Tpy
{

/**
 * \class ChannelFactory
 * \ingroup clientchannel
 * \headerfile TelepathyQt4Yell/channel-factory.h <TelepathyQt4Yell/ChannelFactory>
 *
 * \brief Constructs Channel objects
 */

/**
 * Create a new ChannelFactory object for the given \a bus.
 *
 * The returned factory will construct channel subclasses provided by TelepathyQt4Yell/TelepathyQt4
 * as appropriate for the channel immutable properties, but not make any features ready.
 *
 * \param bus The QDBusConnection the proxies constructed using this factory should use.
 * \return An ChannelFactoryPtr pointing to the newly created factory.
 */
ChannelFactoryPtr ChannelFactory::create(const QDBusConnection &bus)
{
    return ChannelFactoryPtr(new ChannelFactory(bus));
}

/**
 * Class constructor.
 *
 * The constructed factory will construct channel subclasses provided by
 * TelepathyQt4Yell/TelepathyQt4 as appropriate for the channel immutable properties,
 * but not make any features ready.
 *
 * \param bus The QDBusConnection the proxies constructed using this factory should use.
 */
ChannelFactory::ChannelFactory(const QDBusConnection &bus)
    : Tp::ChannelFactory(bus)
{
    setSubclassForMediaCalls<CallChannel>();
}

/**
 * Class destructor.
 */
ChannelFactory::~ChannelFactory()
{
}

void ChannelFactory::setConstructorForMediaCalls(const ConstructorConstPtr &ctor,
        const QVariantMap &additionalProps)
{
    setConstructorFor(ChannelClassSpec::mediaCall(additionalProps), ctor);
}

} // Tpy
