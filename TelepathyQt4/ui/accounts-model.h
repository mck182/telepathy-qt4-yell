#include <QAbstractListModel>

#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/Types>

#ifndef ACCOUNTS_MODEL_H
#define ACCOUNTS_MODEL_H

class AccountsModel : public QAbstractListModel
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
      RequestedPresenceRole,
      ConnectionStatusRole,
      ConnectionRole
    };

public:

    explicit AccountsModel(Tp::AccountManagerPtr am);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
            int role = Qt::DisplayRole) const;

private:

    Tp::AccountManagerPtr mAM;
    QList<Tp::AccountPtr> mAccounts;

private Q_SLOTS:

    void onAMReady(Tp::PendingOperation *);
};

#endif // ACCOUNTS_MODEL_H
