
#include <tests/lib/glib/echo2/conn.h>
#include <QtCore/QEventLoop>
#include <QtTest/QtTest>

#define TP_QT4_ENABLE_LOWLEVEL_API

#include <TelepathyQt4/Debug>
#include <TelepathyQt4/Types>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/ConnectionLowlevel>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/PendingAccount>
#include <TelepathyQt4/Models/AccountsModel>
#include <telepathy-glib/debug.h>

#include <tests/lib/test.h>

using namespace Tp;

class TestAccountsModelBasics : public Test
{
    Q_OBJECT

public:
    TestAccountsModelBasics(QObject *parent = 0)
        : Test(parent),
          mAccountCountChanged(false),
          mRowsAboutToBeInsertedStart(-1),
          mRowsAboutToBeInsertedEnd(-1),
          mRowsInsertedStart(-1),
          mRowsInsertedEnd(-1),
          mRowsAboutToBeRemovedStart(-1),
          mRowsAboutToBeRemovedEnd(-1),
          mRowsRemovedStart(-1),
          mRowsRemovedEnd(-1)
    { }

protected Q_SLOTS:
    void onAccountCountChanged();
    void onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
    void onRowsInserted(const QModelIndex &parent, int start, int end);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void onRowsRemoved(const QModelIndex &parent, int start, int end);

private Q_SLOTS:
    void initTestCase();
    void init();

    void testBasics();

    void cleanup();
    void cleanupTestCase();

private:
    Tp::AccountManagerPtr mAM;

    QString mConnName, mConnPath;
    ExampleEcho2Connection *mConnService;
    ConnectionPtr mConn;
    Tp::AccountsModel *mAccountsModel;

    bool mAccountCountChanged;
    int mRowsAboutToBeInsertedStart;
    int mRowsAboutToBeInsertedEnd;
    int mRowsInsertedStart;
    int mRowsInsertedEnd;
    int mRowsAboutToBeRemovedStart;
    int mRowsAboutToBeRemovedEnd;
    int mRowsRemovedStart;
    int mRowsRemovedEnd;
};

void TestAccountsModelBasics::onAccountCountChanged()
{
    qDebug() << "Account count changed!";
    mAccountCountChanged = true;
}

void TestAccountsModelBasics::onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    qDebug() << "Rows about to be inserted" << parent << start << end;
    Q_UNUSED(parent)
    mRowsAboutToBeInsertedStart = start;
    mRowsAboutToBeInsertedEnd = end;
}

void TestAccountsModelBasics::onRowsInserted(const QModelIndex &parent, int start, int end)
{
    qDebug() << "Rows inserted" << parent << start << end;
    Q_UNUSED(parent)
    mRowsInsertedStart = start;
    mRowsInsertedEnd = end;
}

void TestAccountsModelBasics::onRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end)
{
    qDebug() << "Rows about to be removed" << parent << start << end;
    Q_UNUSED(parent)
    mRowsAboutToBeRemovedStart = start;
    mRowsAboutToBeRemovedEnd = end;
}

void TestAccountsModelBasics::onRowsRemoved(const QModelIndex &parent, int start, int end)
{
    qDebug() << "Rows removed" << parent << start << end;
    Q_UNUSED(parent)
    mRowsRemovedStart = start;
    mRowsRemovedEnd = end;
}

void TestAccountsModelBasics::initTestCase()
{
    initTestCaseImpl();

    mAM = AccountManager::create(AccountFactory::create(QDBusConnection::sessionBus(),
                Account::FeatureCore | Account::FeatureCapabilities));
    QCOMPARE(mAM->isReady(), false);

    g_type_init();
    g_set_prgname("accounts-model-basics");
    tp_debug_set_flags("all");
    dbus_g_bus_get(DBUS_BUS_STARTER, 0);

    gchar *name;
    gchar *connPath;
    GError *error = 0;

    mConnService = EXAMPLE_ECHO_2_CONNECTION(g_object_new(
            EXAMPLE_TYPE_ECHO_2_CONNECTION,
            "account", "me@example.com",
            "protocol", "foo",
            NULL));
    QVERIFY(mConnService != 0);
    QVERIFY(tp_base_connection_register(TP_BASE_CONNECTION(mConnService),
                "foo", &name, &connPath, &error));
    QVERIFY(error == 0);

    QVERIFY(name != 0);
    QVERIFY(connPath != 0);

    mConnName = QLatin1String(name);
    mConnPath = QLatin1String(connPath);

    mConn = Connection::create(mConnName, mConnPath,
            ChannelFactory::create(QDBusConnection::sessionBus()),
            ContactFactory::create());
    QVERIFY(connect(mConn->lowlevel()->requestConnect(),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QCOMPARE(mConn->isReady(), true);
    QCOMPARE(mConn->status(), ConnectionStatusConnected);

    g_free(name);
    g_free(connPath);
}

void TestAccountsModelBasics::init()
{
    initImpl();
}

void TestAccountsModelBasics::testBasics()
{
    QVERIFY(connect(mAM->becomeReady(),
                    SIGNAL(finished(Tp::PendingOperation *)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation *))));
    QCOMPARE(mLoop->exec(), 0);
    QCOMPARE(mAM->isReady(), true);

    mAccountsModel = new Tp::AccountsModel(mAM, this);

    QCOMPARE(mAccountsModel->columnCount(), 1);

    QVERIFY(connect(mAccountsModel,
                    SIGNAL(accountCountChanged()),
                    SLOT(onAccountCountChanged())));

    QVERIFY(connect(mAccountsModel,
                    SIGNAL(rowsAboutToBeInserted(const QModelIndex&, int, int)),
                    SLOT(onRowsAboutToBeInserted(const QModelIndex&, int, int))));
    QVERIFY(connect(mAccountsModel,
                    SIGNAL(rowsInserted(const QModelIndex&, int, int)),
                    SLOT(onRowsInserted(const QModelIndex&, int, int))));

    QVERIFY(connect(mAccountsModel,
                    SIGNAL(rowsAboutToBeRemoved(const QModelIndex&, int, int)),
                    SLOT(onRowsAboutToBeRemoved(const QModelIndex&, int, int))));
    QVERIFY(connect(mAccountsModel,
                    SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
                    SLOT(onRowsRemoved(const QModelIndex&, int, int))));

    QVariantMap parameters;
    parameters[QLatin1String("account")] = QLatin1String("foobar");
    PendingAccount *pacc = mAM->createAccount(QLatin1String("foo"),
            QLatin1String("bar"), QLatin1String("foobar"), parameters);
    QVERIFY(connect(pacc,
                    SIGNAL(finished(Tp::PendingOperation *)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation *))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(pacc->account());

    // check if the account count signal is being emitted correctly
    while (!mAccountCountChanged && mAccountsModel->accountCount() != 1) {
        mLoop->processEvents();
    }
 
    // Verify the account was added to the model
    QCOMPARE(mAccountsModel->rowCount(), 1);
    QCOMPARE(mAccountsModel->accountCount(), 1);

    AccountPtr acc = Account::create(mAM->dbusConnection(), mAM->busName(),
            QLatin1String("/org/freedesktop/Telepathy/Account/foo/bar/Account0"),
            mAM->connectionFactory(), mAM->channelFactory(), mAM->contactFactory());


    QVERIFY(mAccountsModel->index(0, 0).isValid());

    QCOMPARE(mAccountsModel->index(0, 0).data(Tp::AccountsModel::IdRole).toString(),
             acc->uniqueIdentifier());

    QVERIFY(connect(acc->becomeReady(),
                    SIGNAL(finished(Tp::PendingOperation *)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation *))));

    while (!acc->isReady()) {
        mLoop->processEvents();
    }

    /*
    qDebug() << "creating another account";

    pacc = mAM->createAccount(QLatin1String("spurious"),
            QLatin1String("normal"), QLatin1String("foobar"), parameters);
    QVERIFY(connect(pacc,
                    SIGNAL(finished(Tp::PendingOperation *)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation *))));
    QCOMPARE(mLoop->exec(), 0);

    // check for the rowsAboutToBeInserted and rowsInserted signals
    while (!mRowsInsertedStart != 1) {
        mLoop->processEvents();
    }

    QCOMPARE(mRowsInsertedStart, mRowsInsertedEnd);
    QCOMPARE(mRowsAboutToBeInsertedStart, mRowsInsertedStart);
    QCOMPARE(mRowsAboutToBeInsertedEnd, mRowsInsertedEnd);
    */

    qDebug() << "Going to remove account " << acc->uniqueIdentifier();
    // check for the rowsAboutToBeRemoved and rowsRemoved signals
    QVERIFY(connect(acc->remove(),
                    SIGNAL(finished(Tp::PendingOperation *)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation *))));

    while (mRowsRemovedStart < 0) {
        mLoop->processEvents();
    }

    QCOMPARE(mRowsInsertedStart, mRowsInsertedEnd);
    QCOMPARE(mRowsAboutToBeInsertedStart, mRowsInsertedStart);
    QCOMPARE(mRowsAboutToBeInsertedEnd, mRowsInsertedEnd);
}

void TestAccountsModelBasics::cleanup()
{
    cleanupImpl();
}

void TestAccountsModelBasics::cleanupTestCase()
{
    if (mConn) {
        // Disconnect and wait for invalidated
        QVERIFY(connect(mConn->lowlevel()->requestDisconnect(),
                        SIGNAL(finished(Tp::PendingOperation*)),
                        SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
        QCOMPARE(mLoop->exec(), 0);
    }
    cleanupTestCaseImpl();
}

QTEST_MAIN(TestAccountsModelBasics)
#include "_gen/accounts-model-basics.cpp.moc.hpp"
