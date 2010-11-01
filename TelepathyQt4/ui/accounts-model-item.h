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

#ifndef _TelepathyQt4_account_model_item_h_HEADER_GUARD_
#define _TelepathyQt4_account_model_item_h_HEADER_GUARD_

#include "tree-node.h"

#include <TelepathyQt4/Constants>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/Contact>


namespace Tp
{

class TELEPATHY_QT4_EXPORT AccountsModelItem : public TreeNode
{
    Q_OBJECT

public:
    AccountsModelItem(const AccountPtr &account);

    Q_INVOKABLE virtual QVariant data(int role) const;
    virtual bool setData(int role, const QVariant &value);
    Q_INVOKABLE AccountPtr account() const { return mAccount; }

    void setEnabled(bool value);

    Q_INVOKABLE void setStatus(const QString &value);

    Q_INVOKABLE void setStatusMessage(const QString& value);

    Q_INVOKABLE void setNickname(const QString &value);

    Q_INVOKABLE void setPresence(int type, const QString &status, const QString &statusMessage);

Q_SIGNALS:
    void connectionStatusChanged(const QString &accountId,
                                 int status, int statusReason);

private Q_SLOTS:
    void onRemoved();
    void onChanged();
    void onStatusChanged(Tp::ConnectionStatus status,
                         Tp::ConnectionStatusReason statusReason,
                         const QString &error, const QVariantMap &errorDetails);

    void onContactsChanged(const Tp::Contacts &added,
                           const Tp::Contacts &removed);

private:
    AccountPtr mAccount;
};

}

#endif // _TelepathyQt4_account_model_item_h_HEADER_GUARD_

