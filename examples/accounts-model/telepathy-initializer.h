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

#ifndef _TelepathyQt4_telepathy_initializer_h_HEADER_GUARD_
#define _TelepathyQt4_telepathy_initializer_h_HEADER_GUARD_

#include <TelepathyQt4/ui/AccountsModel>
#include <TelepathyQt4/Types>

class TelepathyInitializer : public QObject
{
    Q_OBJECT

public:
    
    TelepathyInitializer(const Tp::AccountManagerPtr &am);

    void run();
    Tp::AccountsModel *accountModel() const;

Q_SIGNALS:
    
    void finished(TelepathyInitializer *initializer);

private Q_SLOTS:
    void onAMReady(Tp::PendingOperation *);
    void onAccountReady(Tp::PendingOperation *);
    void onConnectionReady(Tp::PendingOperation *);
    void onContactsUpgraded(Tp::PendingOperation *);

private:
    
    void initializeConnections();
    void checkFinished();

    Tp::AccountManagerPtr mAM;
    Tp::AccountsModel *mAccountsModel;
    int numConnections;
};

#endif // _TelepathyQt4_telepathy_initializer_h_HEADER_GUARD_

