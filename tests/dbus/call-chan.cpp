#include <QtCore/QDebug>
#include <QtTest/QtTest>

#define TP_QT4_ENABLE_LOWLEVEL_API

#include <TelepathyQt4Yell/CallChannel>

#include <TelepathyQt4/ChannelFactory>
#include <TelepathyQt4/Connection>
#include <TelepathyQt4/ConnectionLowlevel>
#include <TelepathyQt4/ContactFactory>
#include <TelepathyQt4/ContactManager>
#include <TelepathyQt4/PendingChannel>
#include <TelepathyQt4/PendingContacts>
#include <TelepathyQt4/PendingReady>
#include <TelepathyQt4/Debug>

#include <telepathy-glib/debug.h>

#include <tests/lib/glib/call/conn.h>
#include <tests/lib/test.h>

class TestCallChan : public Test
{
    Q_OBJECT

public:
    TestCallChan(QObject *parent = 0)
        : Test(parent), mConnService(0)
    { }

protected Q_SLOTS:
    void expectRequestContactsFinished(Tp::PendingOperation *op);
    void expectCreateChannelFinished(Tp::PendingOperation *op);
    void expectRequestContentFinished(Tp::PendingOperation *op);
    void expectSuccessfulRequestReceiving(Tp::PendingOperation *op);

    void onContentRemoved(const Tpy::CallContentPtr &content);
    void onLocalSendingStateChanged(Tpy::SendingState localSendingState);
    void onRemoteSendingStateChanged(
            const QHash<Tp::ContactPtr, Tpy::SendingState> &remoteSendingStates);

    void onLocalHoldStateChanged(Tp::LocalHoldState state, Tp::LocalHoldStateReason reason);

    void onChanInvalidated(Tp::DBusProxy *proxy, const QString &errorName,
            const QString &errorMessage);
    void onNewChannels(const Tp::ChannelDetailsList &details);

private Q_SLOTS:
    void initTestCase();
    void init();

    void testOutgoingCall();
    void testIncomingCall();
    void testHold();

    void cleanup();
    void cleanupTestCase();

private:
    ExampleCallConnection *mConnService;

    Tp::ConnectionPtr mConn;
    QString mConnName;
    QString mConnPath;
    Tpy::CallChannelPtr mChan;
    QList<Tp::ContactPtr> mRequestContactsReturn;
    Tpy::CallContentPtr mRequestContentReturn;
    Tpy::CallContentPtr mContentRemoved;
    Tpy::SendingState mLSSCReturn;
    QQueue<uint> mLocalHoldStates;
    QQueue<uint> mLocalHoldStateReasons;

    // Remote sending state changed state-machine
    enum {
        RSSCStateInitial,
        RSSCStatePendingSend,
        RSSCStateSending,
        RSSCStateDone
    } mRSSCState;
    int mSuccessfulRequestReceivings;
};

void TestCallChan::expectRequestContactsFinished(Tp::PendingOperation *op)
{
    if (!op->isFinished()) {
        qWarning() << "unfinished";
        mLoop->exit(1);
        return;
    }

    if (op->isError()) {
        qWarning().nospace() << op->errorName()
            << ": " << op->errorMessage();
        mLoop->exit(2);
        return;
    }

    if (!op->isValid()) {
        qWarning() << "inconsistent results";
        mLoop->exit(3);
        return;
    }

    Tp::PendingContacts *pc = qobject_cast<Tp::PendingContacts*>(op);
    mRequestContactsReturn = pc->contacts();
    mLoop->exit(0);
}

void TestCallChan::expectCreateChannelFinished(Tp::PendingOperation* op)
{
    if (!op->isFinished()) {
        qWarning() << "unfinished";
        mLoop->exit(1);
        return;
    }

    if (op->isError()) {
        qWarning().nospace() << op->errorName()
            << ": " << op->errorMessage();
        mLoop->exit(2);
        return;
    }

    if (!op->isValid()) {
        qWarning() << "inconsistent results";
        mLoop->exit(3);
        return;
    }

    Tp::PendingChannel *pc = qobject_cast<Tp::PendingChannel*>(op);
    mChan = Tpy::CallChannelPtr::qObjectCast(pc->channel());
    mLoop->exit(0);
}

void TestCallChan::expectRequestContentFinished(Tp::PendingOperation *op)
{
    if (!op->isFinished()) {
        qWarning() << "unfinished";
        mLoop->exit(1);
        return;
    }

    if (op->isError()) {
        qWarning().nospace() << op->errorName()
            << ": " << op->errorMessage();
        mLoop->exit(2);
        return;
    }

    if (!op->isValid()) {
        qWarning() << "inconsistent results";
        mLoop->exit(3);
        return;
    }

    Tpy::PendingCallContent *pmc = qobject_cast<Tpy::PendingCallContent*>(op);
    mRequestContentReturn = pmc->content();
    mLoop->exit(0);
}

void TestCallChan::onLocalSendingStateChanged(const Tpy::SendingState state)
{
    qDebug() << "local sending state changed";
    mLSSCReturn = state;
    mLoop->exit(0);
}

void TestCallChan::expectSuccessfulRequestReceiving(Tp::PendingOperation *op)
{
    if (!op->isFinished()) {
        qWarning() << "unfinished";
        mLoop->exit(1);
        return;
    }

    if (op->isError()) {
        qWarning().nospace() << op->errorName()
            << ": " << op->errorMessage();
        mLoop->exit(2);
        return;
    }

    if (!op->isValid()) {
        qWarning() << "inconsistent results";
        mLoop->exit(3);
        return;
    }

    if (++mSuccessfulRequestReceivings == 2 && mRSSCState == RSSCStateDone) {
        mLoop->exit(0);
    }
}

void TestCallChan::onContentRemoved(const Tpy::CallContentPtr &content)
{
    mContentRemoved = content;
    mLoop->exec(0);
}

void TestCallChan::onRemoteSendingStateChanged(
        const QHash<Tp::ContactPtr, Tpy::SendingState> &states)
{
    // There should be no further events
    QVERIFY(mRSSCState != RSSCStateDone);

    QCOMPARE(states.size(), 1);
    Tp::ContactPtr otherContact = states.keys().first();

    Tpy::CallContentPtr content = mChan->contentsForType(Tp::MediaStreamTypeVideo).first();
    QVERIFY(content);

    Tpy::CallStreamPtr stream = content->streams().first();
    QVERIFY(stream);

    if (mRSSCState == RSSCStateInitial) {
        QCOMPARE(states[otherContact], Tpy::SendingStatePendingSend);
        mRSSCState = RSSCStatePendingSend;
    } else if (mRSSCState == RSSCStatePendingSend) {
        QCOMPARE(states[otherContact], Tpy::SendingStateSending);
        mRSSCState = RSSCStateSending;

        QVERIFY(connect(stream->requestReceiving(otherContact, false),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulRequestReceiving(Tp::PendingOperation*))));
    } else if (mRSSCState == RSSCStateSending) {
        QCOMPARE(states[otherContact], Tpy::SendingStateNone);
        mRSSCState = RSSCStateDone;

        if (mSuccessfulRequestReceivings == 2) {
            mLoop->exit(0);
        }
    }

    qDebug() << "remote sending state changed to" << states[otherContact];
}

void TestCallChan::onLocalHoldStateChanged(Tp::LocalHoldState localHoldState,
        Tp::LocalHoldStateReason localHoldStateReason)
{
    mLocalHoldStates.append(localHoldState);
    mLocalHoldStateReasons.append(localHoldStateReason);
    mLoop->exit(0);
}

void TestCallChan::onChanInvalidated(Tp::DBusProxy *proxy,
        const QString &errorName, const QString &errorMessage)
{
    qDebug() << "chan invalidated:" << errorName << "-" << errorMessage;
    mLoop->exit(0);
}

void TestCallChan::onNewChannels(const Tp::ChannelDetailsList &channels)
{
    qDebug() << "new channels";
    Q_FOREACH (const Tp::ChannelDetails &details, channels) {
        QString channelType = details.properties.value(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType")).toString();
        bool requested = details.properties.value(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".Requested")).toBool();
        qDebug() << " channelType:" << channelType;
        qDebug() << " requested  :" << requested;

        if (channelType == TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL && !requested) {
            mChan = Tpy::CallChannel::create(mConn, details.channel.path(), details.properties);
            mLoop->exit(0);
        }
    }
}

void TestCallChan::initTestCase()
{
    initTestCaseImpl();

    g_type_init();
    g_set_prgname("text-streamed-Call-call");
    tp_debug_set_flags("all");
    dbus_g_bus_get(DBUS_BUS_STARTER, 0);

    gchar *name;
    gchar *connPath;
    GError *error = 0;

    mConnService = EXAMPLE_CALL_CONNECTION(g_object_new(
            EXAMPLE_TYPE_CALL_CONNECTION,
            "account", "me@example.com",
            "protocol", "example",
            "simulation-delay", 0,
            NULL));
    QVERIFY(mConnService != 0);
    QVERIFY(tp_base_connection_register(TP_BASE_CONNECTION(mConnService),
                "example", &name, &connPath, &error));
    QVERIFY(error == 0);

    QVERIFY(name != 0);
    QVERIFY(connPath != 0);

    mConnName = QLatin1String(name);
    mConnPath = QLatin1String(connPath);

    g_free(name);
    g_free(connPath);

    Tp::ChannelFactoryPtr channelFactory =
        Tp::ChannelFactory::create(QDBusConnection::sessionBus());
    Tpy::CallChannel::registerWithChannelFactory(channelFactory);
    mConn = Tp::Connection::create(mConnName, mConnPath,
            channelFactory, Tp::ContactFactory::create());
    QCOMPARE(mConn->isReady(), false);

    QVERIFY(connect(mConn->lowlevel()->requestConnect(Tp::Connection::FeatureSelfContact),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QCOMPARE(mConn->isReady(), true);
    QCOMPARE(mConn->status(), Tp::ConnectionStatusConnected);
}

void TestCallChan::init()
{
    initImpl();

    mRequestContactsReturn.clear();
    mRequestContentReturn.reset();
    mLSSCReturn = (Tpy::SendingState) -1;
    mLocalHoldStates.clear();
    mLocalHoldStateReasons.clear();
    mContentRemoved.reset();
}

void TestCallChan::testOutgoingCall()
{
    qDebug() << "requesting contact for alice";

    QVERIFY(connect(mConn->contactManager()->contactsForIdentifiers(QStringList() << QLatin1String("alice")),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectRequestContactsFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mRequestContactsReturn.size() == 1);
    Tp::ContactPtr otherContact = mRequestContactsReturn.first();
    QVERIFY(otherContact);

    qDebug() << "creating the channel";

    QVariantMap request;
    request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"),
                   TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL);
    request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"),
                   (uint) Tp::HandleTypeContact);
    request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"),
                   otherContact->handle()[0]);
    request.insert(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialAudio"),
                   true);
    QVERIFY(connect(mConn->lowlevel()->createChannel(request),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectCreateChannelFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mChan);

    qDebug() << "making the channel ready";

    QVERIFY(connect(mChan->becomeReady(Tpy::CallChannel::FeatureContents),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mChan->isReady(Tpy::CallChannel::FeatureContents));

    QCOMPARE(mChan->contents().size(), 1);
    QCOMPARE(mChan->groupContacts().size(), 2);
    QCOMPARE(mChan->groupLocalPendingContacts().size(), 0);
    QCOMPARE(mChan->groupRemotePendingContacts().size(), 0);
    QCOMPARE(mChan->awaitingLocalAnswer(), false);
    QCOMPARE(mChan->awaitingRemoteAnswer(), false);
    QVERIFY(mChan->groupContacts().contains(mConn->selfContact()));
    QVERIFY(mChan->groupContacts().contains(otherContact));

    qDebug() << "calling requestContent with a bad type";
    // RequestContent with bad type must fail
    QVERIFY(connect(mChan->requestContent(QLatin1String("content1"), (Tp::MediaStreamType) -1),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectRequestContentFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 2);
    QVERIFY(!mRequestContentReturn);

    qDebug() << "calling requestContent with Audio";
    QCOMPARE(mChan->contentsForType(Tp::MediaStreamTypeAudio).size(), 1);

    mRequestContentReturn.reset();
    QVERIFY(connect(mChan->requestContent(QLatin1String("content1"), Tp::MediaStreamTypeAudio),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectRequestContentFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(!mRequestContentReturn.isNull());
    QCOMPARE(mRequestContentReturn->name(), QString(QLatin1String("content1")));
    QCOMPARE(mRequestContentReturn->type(), Tp::MediaStreamTypeAudio);

    QCOMPARE(mChan->contentsForType(Tp::MediaStreamTypeAudio).size(), 2);

    qDebug() << "calling requestContent with Video";
    mRequestContentReturn.reset();
    QVERIFY(connect(mChan->requestContent(QLatin1String("content2"), Tp::MediaStreamTypeVideo),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectRequestContentFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(!mRequestContentReturn.isNull());
    QCOMPARE(mRequestContentReturn->name(), QString(QLatin1String("content2")));
    QCOMPARE(mRequestContentReturn->type(), Tp::MediaStreamTypeVideo);

    // test content removal
    QCOMPARE(mChan->contentsForType(Tp::MediaStreamTypeAudio).size(), 2);

    Tpy::CallContentPtr content;
    content = mChan->contentsForType(Tp::MediaStreamTypeAudio).first();
    QVERIFY(content);

    QVERIFY(connect(mChan.data(),
                    SIGNAL(contentRemoved(const Tpy::CallContentPtr &)),
                    SLOT(onContentRemoved(const Tpy::CallContentPtr &))));
    QVERIFY(connect(mChan->removeContent(content, Tpy::ContentRemovalReasonUserRequested,
                        QLatin1String("User requested"), QLatin1String("leaving")),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    while (mContentRemoved.isNull()) {
        QCOMPARE(mLoop->exec(), 0);
    }
    QCOMPARE(mChan->contentsForType(Tp::MediaStreamTypeAudio).size(), 1);
    QCOMPARE(mContentRemoved, content);

    // test content sending changed
    content = mChan->contentsForType(Tp::MediaStreamTypeVideo).first();
    Tpy::CallStreamPtr stream = content->streams().first();
    QVERIFY(content);

    QVERIFY(connect(stream.data(),
                    SIGNAL(localSendingStateChanged(Tpy::SendingState)),
                    SLOT(onLocalSendingStateChanged(Tpy::SendingState))));
    QVERIFY(connect(stream.data(),
                    SIGNAL(remoteSendingStateChanged(const QHash<Tp::ContactPtr, Tpy::SendingState> &)),
                    SLOT(onRemoteSendingStateChanged(const QHash<Tp::ContactPtr, Tpy::SendingState> &))));

    qDebug() << "stopping sending";

    QCOMPARE(stream->localSendingState(), Tpy::SendingStateSending);
    QVERIFY(stream->members().contains(otherContact));

    QVERIFY(connect(stream->requestSending(false),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);

    QCOMPARE(stream->localSendingState(), Tpy::SendingStateNone);

    qDebug() << "stopping receiving";
    QVERIFY(connect(stream->requestReceiving(otherContact, false),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);

    qDebug() << "waiting until we're not sending and not receiving";

    while (stream->localSendingState() != Tpy::SendingStateNone
            || stream->remoteSendingState(otherContact) != Tpy::SendingStateNone) {
        qDebug() << "re-entering mainloop to wait for local and remote SSC -> None";
        // wait local and remote sending state change
        QCOMPARE(mLoop->exec(), 0);
    }
    QCOMPARE(mLSSCReturn, Tpy::SendingStateNone);
    QCOMPARE(stream->localSendingState(), Tpy::SendingStateNone);
    QCOMPARE(stream->remoteSendingState(otherContact), Tpy::SendingStateNone);

    qDebug() << "re-enabling sending";

    mLSSCReturn = (Tpy::SendingState) -1;

    QVERIFY(connect(stream->requestSending(true),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    while (mLSSCReturn == (Tpy::SendingState) -1) {
        qDebug() << "re-entering mainloop to wait for SSC -> Sending";
        // wait sending state change
        QCOMPARE(mLoop->exec(), 0);
    }
    QCOMPARE(mLSSCReturn, Tpy::SendingStateSending);

    qDebug() << "flushing D-Bus events";
    processDBusQueue(mChan.data());

    qDebug() << "enabling receiving";

    mRSSCState = RSSCStateInitial;
    mSuccessfulRequestReceivings = 0;

    // test content receiving changed
    QVERIFY(connect(stream->requestReceiving(otherContact, true),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulRequestReceiving(Tp::PendingOperation*))));
    while (mSuccessfulRequestReceivings != 2 || mRSSCState != RSSCStateDone) {
        mLoop->processEvents();
    }

    QCOMPARE(static_cast<uint>(mRSSCState), static_cast<uint>(RSSCStateDone));
}

void TestCallChan::testIncomingCall()
{
    mConn->lowlevel()->setSelfPresence(QLatin1String("away"), QLatin1String("preparing for a test"));

    Tp::Client::ConnectionInterfaceRequestsInterface *connRequestsInterface =
        mConn->optionalInterface<Tp::Client::ConnectionInterfaceRequestsInterface>();
    QVERIFY(connect(connRequestsInterface,
                    SIGNAL(NewChannels(const Tp::ChannelDetailsList&)),
                    SLOT(onNewChannels(const Tp::ChannelDetailsList&))));

    mConn->lowlevel()->setSelfPresence(QLatin1String("available"), QLatin1String("call me?"));
    QCOMPARE(mLoop->exec(), 0);

    QVERIFY(mChan);
    QCOMPARE(mChan->contents().size(), 0);

    QVERIFY(connect(mChan->becomeReady(Tpy::CallChannel::FeatureContents),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mChan->isReady(Tpy::CallChannel::FeatureContents));

    Tp::ContactPtr otherContact = *mChan->groupContacts().begin();

    QVERIFY(connect(mChan->accept(),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QCOMPARE(mChan->groupContacts().size(), 2);
    QCOMPARE(mChan->groupLocalPendingContacts().size(), 0);
    QCOMPARE(mChan->groupRemotePendingContacts().size(), 0);
    QCOMPARE(mChan->awaitingLocalAnswer(), false);
    QVERIFY(mChan->groupContacts().contains(mConn->selfContact()));

    QCOMPARE(mChan->contents().size(), 1);
    Tpy::CallContentPtr content = mChan->contents().first();
    QCOMPARE(content->channel(), mChan);
    QCOMPARE(content->type(), Tp::MediaStreamTypeAudio);

    qDebug() << "requesting a video stream";

    // Request video stream
    QVERIFY(connect(mChan->requestContent(QLatin1String("video_content"), Tp::MediaStreamTypeVideo),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectRequestContentFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    content = mRequestContentReturn;
    QCOMPARE(content->type(), Tp::MediaStreamTypeVideo);

    QCOMPARE(mChan->contents().size(), 2);
    QVERIFY(mChan->contents().contains(content));

    QCOMPARE(mChan->contentsForType(Tp::MediaStreamTypeAudio).size(), 1);
    QCOMPARE(mChan->contentsForType(Tp::MediaStreamTypeVideo).size(), 1);

    // test content removal
    content = mChan->contentsForType(Tp::MediaStreamTypeAudio).first();
    QVERIFY(content);

    qDebug() << "removing the audio content";

    // call does not have the concept of removing streams, it will remove the content the stream
    // belongs
    QVERIFY(connect(mChan.data(),
                    SIGNAL(contentRemoved(const Tpy::CallContentPtr &)),
                    SLOT(onContentRemoved(const Tpy::CallContentPtr &))));

    QVERIFY(connect(mChan->removeContent(content, Tpy::ContentRemovalReasonUserRequested,
                        QLatin1String("User requested"), QLatin1String("leaving")),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    while (mContentRemoved.isNull()) {
        QCOMPARE(mLoop->exec(), 0);
    }
    QCOMPARE(mContentRemoved, content);
}

void TestCallChan::testHold()
{
    QVERIFY(connect(mConn->contactManager()->contactsForIdentifiers(QStringList() << QLatin1String("bob")),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectRequestContactsFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mRequestContactsReturn.size() == 1);
    Tp::ContactPtr otherContact = mRequestContactsReturn.first();
    QVERIFY(otherContact);

    QVariantMap request;
    request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType"),
                   TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL);
    request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType"),
                   (uint) Tp::HandleTypeContact);
    request.insert(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle"),
                   otherContact->handle()[0]);
    request.insert(TP_QT4_YELL_IFACE_CHANNEL_TYPE_CALL + QLatin1String(".InitialAudio"),
                   true);
    QVERIFY(connect(mConn->lowlevel()->createChannel(request),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectCreateChannelFinished(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mChan);

    QVERIFY(connect(mChan->becomeReady(Tpy::CallChannel::FeatureLocalHoldState),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    QVERIFY(mChan->isReady(Tpy::CallChannel::FeatureLocalHoldState));

    QCOMPARE(static_cast<uint>(mChan->localHoldState()), static_cast<uint>(Tp::LocalHoldStateUnheld));
    QCOMPARE(static_cast<uint>(mChan->localHoldStateReason()), static_cast<uint>(Tp::LocalHoldStateReasonNone));

    QVERIFY(connect(mChan.data(),
                    SIGNAL(localHoldStateChanged(Tp::LocalHoldState, Tp::LocalHoldStateReason)),
                    SLOT(onLocalHoldStateChanged(Tp::LocalHoldState, Tp::LocalHoldStateReason))));
    // Request hold
    QVERIFY(connect(mChan->requestHold(true),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    while (mLocalHoldStates.size() != 2) {
        QCOMPARE(mLoop->exec(), 0);
    }
    QCOMPARE(mLocalHoldStates.first(), static_cast<uint>(Tp::LocalHoldStatePendingHold));
    QCOMPARE(mLocalHoldStateReasons.first(), static_cast<uint>(Tp::LocalHoldStateReasonRequested));
    QCOMPARE(mLocalHoldStates.last(), static_cast<uint>(Tp::LocalHoldStateHeld));
    QCOMPARE(mLocalHoldStateReasons.last(), static_cast<uint>(Tp::LocalHoldStateReasonRequested));
    QCOMPARE(static_cast<uint>(mChan->localHoldState()), static_cast<uint>(Tp::LocalHoldStateHeld));
    QCOMPARE(static_cast<uint>(mChan->localHoldStateReason()), static_cast<uint>(Tp::LocalHoldStateReasonRequested));

    mLocalHoldStates.clear();
    mLocalHoldStateReasons.clear();

    // Request unhold
    QVERIFY(connect(mChan->requestHold(false),
                    SIGNAL(finished(Tp::PendingOperation*)),
                    SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
    QCOMPARE(mLoop->exec(), 0);
    while (mLocalHoldStates.size() != 2) {
        QCOMPARE(mLoop->exec(), 0);
    }
    QCOMPARE(mLocalHoldStates.first(), static_cast<uint>(Tp::LocalHoldStatePendingUnhold));
    QCOMPARE(mLocalHoldStateReasons.first(), static_cast<uint>(Tp::LocalHoldStateReasonRequested));
    QCOMPARE(mLocalHoldStates.last(), static_cast<uint>(Tp::LocalHoldStateUnheld));
    QCOMPARE(mLocalHoldStateReasons.last(), static_cast<uint>(Tp::LocalHoldStateReasonRequested));
    QCOMPARE(static_cast<uint>(mChan->localHoldState()), static_cast<uint>(Tp::LocalHoldStateUnheld));
    QCOMPARE(static_cast<uint>(mChan->localHoldStateReason()), static_cast<uint>(Tp::LocalHoldStateReasonRequested));
}

void TestCallChan::cleanup()
{
    mChan.reset();
    cleanupImpl();
}

void TestCallChan::cleanupTestCase()
{
    if (mConn) {
        QVERIFY(connect(mConn->lowlevel()->requestDisconnect(),
                        SIGNAL(finished(Tp::PendingOperation*)),
                        SLOT(expectSuccessfulCall(Tp::PendingOperation*))));
        QCOMPARE(mLoop->exec(), 0);

        if (mConn->isValid()) {
            QVERIFY(connect(mConn.data(),
                            SIGNAL(invalidated(Tp::DBusProxy *,
                                               const QString &, const QString &)),
                            mLoop,
                            SLOT(quit())));
            QCOMPARE(mLoop->exec(), 0);
        }
    }

    cleanupTestCaseImpl();
}

QTEST_MAIN(TestCallChan)
#include "_gen/call-chan.cpp.moc.hpp"
