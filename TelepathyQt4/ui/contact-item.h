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

#ifndef CONTACT_ITEM_H
#define CONTACT_ITEM_H

#include <QtCore/QList>

#include <TelepathyQt4/Contact>
#include "abstract-tree-item.h"


namespace Tp {

    class ContactItem : public QObject,
                        public AbstractTreeItem
{
    Q_OBJECT

public:
    ContactItem();
    virtual ~ContactItem();

    void setContact(ContactPtr contact);

    QString id() const;
    ContactPtr contact();
    QString alias() const;
    AvatarData avatarData() const;
    QString presenceStatus() const;
    uint presenceType() const;
    QString presenceMessage() const;
    int subscriptionState() const;
    int publishState() const;
    bool isBlocked() const;
    QStringList groups() const;


private:
    ContactPtr m_contact;
};

} // namespace Tp

#endif // CONTACT_ITEM_H



