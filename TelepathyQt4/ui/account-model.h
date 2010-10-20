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

#ifndef _TelepathyQt4_account_model_h_HEADER_GUARD_
#define _TelepathyQt4_account_model_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_HEADER
#error IN_TELEPATHY_QT4_HEADER
#endif

#include <QAbstractListModel>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/TextChannel>

namespace Tp
{

class TELEPATHY_QT4_EXPORT AccountModel : public QAbstractListModel
{
    Q_OBJECT

    enum Role {
      ValidRole = Qt::UserRole,
      EnabledRole,
      ConnectionManagerRole,
      ProtocolNameRole,
      DisplayNameRole,
      NicknameRole,
      ConnectsAutomaticallyRole,
      ChangingPresenceRole,
      AutomaticPresenceRole,
      CurrentPresenceRole,
      CurrentStatusMessage,
      RequestedPresenceRole,
      RequestedStatusMessage,
      ConnectionStatusRole,
      ConnectionRole
    };

public:

    explicit AccountModel(const Tp::AccountManagerPtr &am, QObject *parent = 0);
    virtual ~AccountModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    AccountPtr accountForIndex(const QModelIndex &index) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Q_INVOKABLE void setAccountEnabled(int row, bool value);
    Q_INVOKABLE void setAccountStatus(int row, const QString &value);
    Q_INVOKABLE void setAccountStatusMessage(int row, const QString &value);

private:

    Tp::AccountManagerPtr mAM;
    QList<Tp::AccountPtr> mAccounts;

    void setupAccount(const Tp::AccountPtr &account);

private Q_SLOTS:

    void onAMReady(Tp::PendingOperation *);
    void onNewAccount(const Tp::AccountPtr &account);
    void onAccountChanged();
    void onAccountRemoved();
};

}

#endif // _TelepathyQt4_account_model_h_HEADER_GUARD_

