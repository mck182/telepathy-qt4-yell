/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2010 Collabora Ltd. <info@collabora.co.uk>
 *   @Author George Goldberg <george.goldberg@collabora.co.uk>
 *   @Author Alvaro Soliverez <alvaro.soliverez@collabora.co.uk>
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

#include "contact-item.h"
#include "TelepathyQt4/ui/_gen/contact-item.moc.hpp"

using namespace Tp;

ContactItem::ContactItem()
 : m_contact(0)
{

}

void ContactItem::setContact(ContactPtr contact)
{
    m_contact = contact;
}

ContactItem::~ContactItem()
{
}

QString ContactItem::id() const
{
    return m_contact->id();
}

ContactPtr ContactItem::contact()
{
    return m_contact;
}

QString ContactItem::alias() const
{
    return m_contact->alias();
}

AvatarData ContactItem::avatarData() const
{
    return m_contact->avatarData();
}

QString ContactItem::presenceStatus() const
{
    return m_contact->presenceStatus();
}

uint ContactItem::presenceType() const
{
    return m_contact->presenceType();
}

QString ContactItem::presenceMessage() const
{
    return m_contact->presenceMessage();
}

int ContactItem::subscriptionState() const
{
    return m_contact->subscriptionState();
}

int ContactItem::publishState() const
{
    return m_contact->publishState();
}

bool ContactItem::isBlocked() const
{
    return m_contact->isBlocked();
}

QStringList ContactItem::groups() const
{
    return m_contact->groups();
}



