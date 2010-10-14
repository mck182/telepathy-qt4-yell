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

#include <TelepathyQt4/ui/AccountModel>
#include "TelepathyQt4/ui/_gen/account-model.moc.hpp"
#include <TelepathyQt4/PendingReady>

namespace Tp
{

AccountModel::AccountModel(const Tp::AccountManagerPtr &am, QObject *parent)
    : QAbstractListModel(parent)
    , mAM(am)
{
    connect(mAM->becomeReady(),
            SIGNAL(finished(Tp::PendingOperation *)),
            SLOT(onAMReady(Tp::PendingOperation *)));
    QHash<int, QByteArray> roles;
    roles[ValidRole] = "valid";
    roles[EnabledRole] = "enabled";
    roles[ConnectionManagerRole] = "connection";
    roles[ProtocolNameRole] = "protocol";
    roles[DisplayNameRole] = "displayName";
    roles[NicknameRole] = "nickname";
    roles[ConnectsAutomaticallyRole] = "connectsAutomatically";
    roles[ChangingPresenceRole] = "changingPresence";
    roles[AutomaticPresenceRole] = "automaticPresence";
    roles[CurrentPresenceRole] = "currentPresence";
    roles[RequestedPresenceRole] = "requestedPresence";
    roles[ConnectionStatusRole] = "connectionStatus";
    roles[ConnectionRole] = "connection";
    setRoleNames(roles);
}

AccountModel::~AccountModel()
{
}

void AccountModel::onAMReady(Tp::PendingOperation *)
{
    QList<Tp::AccountPtr> accounts = mAM->allAccounts();
    beginInsertRows(QModelIndex(), 0, accounts.count() - 1);
    mAccounts = accounts;
    endInsertRows();
}

int AccountModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mAccounts.count();
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= mAccounts.count()) {
        return QVariant();
    }

    Tp::AccountPtr account = mAccounts[index.row()];
    switch (role) {
        case ValidRole:
            return account->isValid();
        case EnabledRole:
            return account->isEnabled();
        case ConnectionManagerRole:
            return account->cmName();
        case ProtocolNameRole:
            return account->protocolName();
        case DisplayNameRole:
        case Qt::DisplayRole:
            return account->displayName();
        case NicknameRole:
            return account->nickname();
        case ConnectsAutomaticallyRole:
            return account->connectsAutomatically();
        case ChangingPresenceRole:
            return account->isChangingPresence();
        case AutomaticPresenceRole:
            return account->automaticPresence().status;
        case CurrentPresenceRole:
            return account->currentPresence().status;
        case RequestedPresenceRole:
            return account->requestedPresence().status;
        case ConnectionStatusRole:
            return account->connectionStatus();
        case ConnectionRole:
            return account->connectionObjectPath();
        default:
            return QVariant();
    }
}

}

