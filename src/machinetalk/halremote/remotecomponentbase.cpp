/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#include "remotecomponentbase.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk { namespace halremote {

/** Generic Remote Component Base implementation */
RemoteComponentBase::RemoteComponentBase(QObject *parent)
    : QObject(parent)
    , QQmlParserStatus()
    , m_componentCompleted(false)
    , m_ready(false)
    , m_debugName("Remote Component Base")
    , m_halrcmdChannel(nullptr)
    , m_halrcompChannel(nullptr)
    , m_state(State::Down)
    , m_previousState(State::Down)
    , m_errorString("")
{
    // initialize halrcmd channel
    m_halrcmdChannel = new common::RpcClient(this);
    m_halrcmdChannel->setDebugName(m_debugName + " - halrcmd");
    connect(m_halrcmdChannel, &common::RpcClient::socketUriChanged,
            this, &RemoteComponentBase::halrcmdUriChanged);
    connect(m_halrcmdChannel, &common::RpcClient::stateChanged,
            this, &RemoteComponentBase::halrcmdChannelStateChanged);
    connect(m_halrcmdChannel, &common::RpcClient::socketMessageReceived,
            this, &RemoteComponentBase::processHalrcmdChannelMessage);
    // initialize halrcomp channel
    m_halrcompChannel = new halremote::HalrcompSubscribe(this);
    m_halrcompChannel->setDebugName(m_debugName + " - halrcomp");
    connect(m_halrcompChannel, &halremote::HalrcompSubscribe::socketUriChanged,
            this, &RemoteComponentBase::halrcompUriChanged);
    connect(m_halrcompChannel, &halremote::HalrcompSubscribe::stateChanged,
            this, &RemoteComponentBase::halrcompChannelStateChanged);
    connect(m_halrcompChannel, &halremote::HalrcompSubscribe::socketMessageReceived,
            this, &RemoteComponentBase::processHalrcompChannelMessage);

    connect(m_halrcmdChannel, &common::RpcClient::heartbeatIntervalChanged,
            this, &RemoteComponentBase::halrcmdHeartbeatIntervalChanged);

    connect(m_halrcompChannel, &halremote::HalrcompSubscribe::heartbeatIntervalChanged,
            this, &RemoteComponentBase::halrcompHeartbeatIntervalChanged);
    // state machine
    connect(this, &RemoteComponentBase::fsmDownEntered,
            this, &RemoteComponentBase::fsmDownEntry);
    connect(this, &RemoteComponentBase::fsmDownExited,
            this, &RemoteComponentBase::fsmDownExit);
    connect(this, &RemoteComponentBase::fsmSyncedEntered,
            this, &RemoteComponentBase::fsmSyncedEntry);
    connect(this, &RemoteComponentBase::fsmErrorEntered,
            this, &RemoteComponentBase::fsmErrorEntry);
    connect(this, &RemoteComponentBase::fsmDownConnect,
            this, &RemoteComponentBase::fsmDownConnectEvent);
    connect(this, &RemoteComponentBase::fsmTryingHalrcmdUp,
            this, &RemoteComponentBase::fsmTryingHalrcmdUpEvent);
    connect(this, &RemoteComponentBase::fsmTryingDisconnect,
            this, &RemoteComponentBase::fsmTryingDisconnectEvent);
    connect(this, &RemoteComponentBase::fsmBindHalrcompBindMsgSent,
            this, &RemoteComponentBase::fsmBindHalrcompBindMsgSentEvent);
    connect(this, &RemoteComponentBase::fsmBindNoBind,
            this, &RemoteComponentBase::fsmBindNoBindEvent);
    connect(this, &RemoteComponentBase::fsmBindingBindConfirmed,
            this, &RemoteComponentBase::fsmBindingBindConfirmedEvent);
    connect(this, &RemoteComponentBase::fsmBindingBindRejected,
            this, &RemoteComponentBase::fsmBindingBindRejectedEvent);
    connect(this, &RemoteComponentBase::fsmBindingHalrcmdTrying,
            this, &RemoteComponentBase::fsmBindingHalrcmdTryingEvent);
    connect(this, &RemoteComponentBase::fsmBindingDisconnect,
            this, &RemoteComponentBase::fsmBindingDisconnectEvent);
    connect(this, &RemoteComponentBase::fsmSyncingHalrcmdTrying,
            this, &RemoteComponentBase::fsmSyncingHalrcmdTryingEvent);
    connect(this, &RemoteComponentBase::fsmSyncingHalrcompUp,
            this, &RemoteComponentBase::fsmSyncingHalrcompUpEvent);
    connect(this, &RemoteComponentBase::fsmSyncingSyncFailed,
            this, &RemoteComponentBase::fsmSyncingSyncFailedEvent);
    connect(this, &RemoteComponentBase::fsmSyncingDisconnect,
            this, &RemoteComponentBase::fsmSyncingDisconnectEvent);
    connect(this, &RemoteComponentBase::fsmSyncPinsSynced,
            this, &RemoteComponentBase::fsmSyncPinsSyncedEvent);
    connect(this, &RemoteComponentBase::fsmSyncedHalrcompTrying,
            this, &RemoteComponentBase::fsmSyncedHalrcompTryingEvent);
    connect(this, &RemoteComponentBase::fsmSyncedHalrcmdTrying,
            this, &RemoteComponentBase::fsmSyncedHalrcmdTryingEvent);
    connect(this, &RemoteComponentBase::fsmSyncedSetRejected,
            this, &RemoteComponentBase::fsmSyncedSetRejectedEvent);
    connect(this, &RemoteComponentBase::fsmSyncedHalrcompSetMsgSent,
            this, &RemoteComponentBase::fsmSyncedHalrcompSetMsgSentEvent);
    connect(this, &RemoteComponentBase::fsmSyncedDisconnect,
            this, &RemoteComponentBase::fsmSyncedDisconnectEvent);
    connect(this, &RemoteComponentBase::fsmErrorDisconnect,
            this, &RemoteComponentBase::fsmErrorDisconnectEvent);
}

RemoteComponentBase::~RemoteComponentBase()
{
}

/** Add a topic that should be subscribed **/
void RemoteComponentBase::addHalrcompTopic(const QByteArray &name)
{
    m_halrcompChannel->addSocketTopic(name);
}

/** Removes a topic from the list of topics that should be subscribed **/
void RemoteComponentBase::removeHalrcompTopic(const QByteArray &name)
{
    m_halrcompChannel->removeSocketTopic(name);
}

/** Clears the the topics that should be subscribed **/
void RemoteComponentBase::clearHalrcompTopics()
{
    m_halrcompChannel->clearSocketTopics();
}

void RemoteComponentBase::startHalrcmdChannel()
{
    m_halrcmdChannel->setReady(true);
}

void RemoteComponentBase::stopHalrcmdChannel()
{
    m_halrcmdChannel->setReady(false);
}

void RemoteComponentBase::startHalrcompChannel()
{
    m_halrcompChannel->setReady(true);
}

void RemoteComponentBase::stopHalrcompChannel()
{
    m_halrcompChannel->setReady(false);
}

/** Processes all message received on halrcmd */
void RemoteComponentBase::processHalrcmdChannelMessage(const Container &rx)
{

    // react to halrcomp bind confirm message
    if (rx.type() == MT_HALRCOMP_BIND_CONFIRM)
    {

        if (m_state == State::Binding)
        {
            emit fsmBindingBindConfirmed(QPrivateSignal());
        }
    }

    // react to halrcomp bind reject message
    else if (rx.type() == MT_HALRCOMP_BIND_REJECT)
    {

        // update error string with note
        m_errorString = "";
        for (int i = 0; i < rx.note_size(); ++i)
        {
            m_errorString.append(QString::fromStdString(rx.note(i)) + "\n");
        }
        emit errorStringChanged(m_errorString);

        if (m_state == State::Binding)
        {
            emit fsmBindingBindRejected(QPrivateSignal());
        }
    }

    // react to halrcomp set reject message
    else if (rx.type() == MT_HALRCOMP_SET_REJECT)
    {

        // update error string with note
        m_errorString = "";
        for (int i = 0; i < rx.note_size(); ++i)
        {
            m_errorString.append(QString::fromStdString(rx.note(i)) + "\n");
        }
        emit errorStringChanged(m_errorString);

        if (m_state == State::Synced)
        {
            emit fsmSyncedSetRejected(QPrivateSignal());
        }
    }

    emit halrcmdMessageReceived(rx);
}

/** Processes all message received on halrcomp */
void RemoteComponentBase::processHalrcompChannelMessage(const QByteArray &topic, const Container &rx)
{

    // react to halrcomp full update message
    if (rx.type() == MT_HALRCOMP_FULL_UPDATE)
    {
        handleHalrcompFullUpdateMessage(topic, rx);
    }

    // react to halrcomp incremental update message
    else if (rx.type() == MT_HALRCOMP_INCREMENTAL_UPDATE)
    {
        handleHalrcompIncrementalUpdateMessage(topic, rx);
    }

    // react to halrcomp error message
    else if (rx.type() == MT_HALRCOMP_ERROR)
    {

        // update error string with note
        m_errorString = "";
        for (int i = 0; i < rx.note_size(); ++i)
        {
            m_errorString.append(QString::fromStdString(rx.note(i)) + "\n");
        }
        emit errorStringChanged(m_errorString);

        if (m_state == State::Syncing)
        {
            emit fsmSyncingSyncFailed(QPrivateSignal());
        }
        handleHalrcompErrorMessage(topic, rx);
    }

    emit halrcompMessageReceived(topic, rx);
}

void RemoteComponentBase::sendHalrcmdMessage(ContainerType type, Container &tx)
{
    m_halrcmdChannel->sendSocketMessage(type, tx);
    if (type == MT_HALRCOMP_BIND)
    {

        if (m_state == State::Bind)
        {
            emit fsmBindHalrcompBindMsgSent(QPrivateSignal());
        }
    }
    else if (type == MT_HALRCOMP_SET)
    {

        if (m_state == State::Synced)
        {
            emit fsmSyncedHalrcompSetMsgSent(QPrivateSignal());
        }
    }
}

void RemoteComponentBase::sendHalrcompBind(Container &tx)
{
    sendHalrcmdMessage(MT_HALRCOMP_BIND, tx);
}

void RemoteComponentBase::sendHalrcompSet(Container &tx)
{
    sendHalrcmdMessage(MT_HALRCOMP_SET, tx);
}

void RemoteComponentBase::fsmDown()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
    m_state = State::Down;
    emit stateChanged(m_state);
}
void RemoteComponentBase::fsmDownEntry()
{
    setDisconnected();
}
void RemoteComponentBase::fsmDownExit()
{
    setConnecting();
}

void RemoteComponentBase::fsmDownConnectEvent()
{
    if (m_state == State::Down)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event CONNECT");
#endif
        // handle state change
        emit fsmDownExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        addPins();
        startHalrcmdChannel();
     }
}

void RemoteComponentBase::fsmTrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
    m_state = State::Trying;
    emit stateChanged(m_state);
}

void RemoteComponentBase::fsmTryingHalrcmdUpEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCMD UP");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmBind();
        emit fsmBindEntered(QPrivateSignal());
        // execute actions
        bindComponent();
     }
}

void RemoteComponentBase::fsmTryingDisconnectEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHalrcmdChannel();
        stopHalrcompChannel();
        removePins();
     }
}

void RemoteComponentBase::fsmBind()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State BIND");
#endif
    m_state = State::Bind;
    emit stateChanged(m_state);
}

void RemoteComponentBase::fsmBindHalrcompBindMsgSentEvent()
{
    if (m_state == State::Bind)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCOMP BIND MSG SENT");
#endif
        // handle state change
        emit fsmBindExited(QPrivateSignal());
        fsmBinding();
        emit fsmBindingEntered(QPrivateSignal());
        // execute actions
     }
}

void RemoteComponentBase::fsmBindNoBindEvent()
{
    if (m_state == State::Bind)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event NO BIND");
#endif
        // handle state change
        emit fsmBindExited(QPrivateSignal());
        fsmSyncing();
        emit fsmSyncingEntered(QPrivateSignal());
        // execute actions
        startHalrcompChannel();
     }
}

void RemoteComponentBase::fsmBinding()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State BINDING");
#endif
    m_state = State::Binding;
    emit stateChanged(m_state);
}

void RemoteComponentBase::fsmBindingBindConfirmedEvent()
{
    if (m_state == State::Binding)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event BIND CONFIRMED");
#endif
        // handle state change
        emit fsmBindingExited(QPrivateSignal());
        fsmSyncing();
        emit fsmSyncingEntered(QPrivateSignal());
        // execute actions
        startHalrcompChannel();
     }
}

void RemoteComponentBase::fsmBindingBindRejectedEvent()
{
    if (m_state == State::Binding)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event BIND REJECTED");
#endif
        // handle state change
        emit fsmBindingExited(QPrivateSignal());
        fsmError();
        emit fsmErrorEntered(QPrivateSignal());
        // execute actions
        stopHalrcmdChannel();
     }
}

void RemoteComponentBase::fsmBindingHalrcmdTryingEvent()
{
    if (m_state == State::Binding)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCMD TRYING");
#endif
        // handle state change
        emit fsmBindingExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
     }
}

void RemoteComponentBase::fsmBindingDisconnectEvent()
{
    if (m_state == State::Binding)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmBindingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHalrcmdChannel();
        stopHalrcompChannel();
        removePins();
     }
}

void RemoteComponentBase::fsmSyncing()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State SYNCING");
#endif
    m_state = State::Syncing;
    emit stateChanged(m_state);
}

void RemoteComponentBase::fsmSyncingHalrcmdTryingEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCMD TRYING");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        stopHalrcompChannel();
     }
}

void RemoteComponentBase::fsmSyncingHalrcompUpEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCOMP UP");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmSync();
        emit fsmSyncEntered(QPrivateSignal());
        // execute actions
     }
}

void RemoteComponentBase::fsmSyncingSyncFailedEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event SYNC FAILED");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmError();
        emit fsmErrorEntered(QPrivateSignal());
        // execute actions
        stopHalrcompChannel();
        stopHalrcmdChannel();
     }
}

void RemoteComponentBase::fsmSyncingDisconnectEvent()
{
    if (m_state == State::Syncing)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmSyncingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHalrcmdChannel();
        stopHalrcompChannel();
        removePins();
     }
}

void RemoteComponentBase::fsmSync()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State SYNC");
#endif
    m_state = State::Sync;
    emit stateChanged(m_state);
}

void RemoteComponentBase::fsmSyncPinsSyncedEvent()
{
    if (m_state == State::Sync)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event PINS SYNCED");
#endif
        // handle state change
        emit fsmSyncExited(QPrivateSignal());
        fsmSynced();
        emit fsmSyncedEntered(QPrivateSignal());
        // execute actions
     }
}

void RemoteComponentBase::fsmSynced()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State SYNCED");
#endif
    m_state = State::Synced;
    emit stateChanged(m_state);
}
void RemoteComponentBase::fsmSyncedEntry()
{
    setConnected();
}

void RemoteComponentBase::fsmSyncedHalrcompTryingEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCOMP TRYING");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmSyncing();
        emit fsmSyncingEntered(QPrivateSignal());
        // execute actions
        unsyncPins();
        setTimeout();
     }
}

void RemoteComponentBase::fsmSyncedHalrcmdTryingEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCMD TRYING");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        stopHalrcompChannel();
        unsyncPins();
        setTimeout();
     }
}

void RemoteComponentBase::fsmSyncedSetRejectedEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event SET REJECTED");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmError();
        emit fsmErrorEntered(QPrivateSignal());
        // execute actions
        stopHalrcompChannel();
        stopHalrcmdChannel();
     }
}

void RemoteComponentBase::fsmSyncedHalrcompSetMsgSentEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HALRCOMP SET MSG SENT");
#endif
        // execute actions
     }
}

void RemoteComponentBase::fsmSyncedDisconnectEvent()
{
    if (m_state == State::Synced)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmSyncedExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHalrcmdChannel();
        stopHalrcompChannel();
        removePins();
     }
}

void RemoteComponentBase::fsmError()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State ERROR");
#endif
    m_state = State::Error;
    emit stateChanged(m_state);
}
void RemoteComponentBase::fsmErrorEntry()
{
    setError();
}

void RemoteComponentBase::fsmErrorDisconnectEvent()
{
    if (m_state == State::Error)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event DISCONNECT");
#endif
        // handle state change
        emit fsmErrorExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHalrcmdChannel();
        stopHalrcompChannel();
        removePins();
     }
}

void RemoteComponentBase::halrcmdChannelStateChanged(common::RpcClient::State state)
{

    if (state == common::RpcClient::State::Trying)
    {
        if (m_state == State::Syncing)
        {
            emit fsmSyncingHalrcmdTrying(QPrivateSignal());
        }
        else if (m_state == State::Synced)
        {
            emit fsmSyncedHalrcmdTrying(QPrivateSignal());
        }
        else if (m_state == State::Binding)
        {
            emit fsmBindingHalrcmdTrying(QPrivateSignal());
        }
    }

    else if (state == common::RpcClient::State::Up)
    {
        if (m_state == State::Trying)
        {
            emit fsmTryingHalrcmdUp(QPrivateSignal());
        }
    }
}

void RemoteComponentBase::halrcompChannelStateChanged(halremote::HalrcompSubscribe::State state)
{

    if (state == halremote::HalrcompSubscribe::State::Trying)
    {
        if (m_state == State::Synced)
        {
            emit fsmSyncedHalrcompTrying(QPrivateSignal());
        }
    }

    else if (state == halremote::HalrcompSubscribe::State::Up)
    {
        if (m_state == State::Syncing)
        {
            emit fsmSyncingHalrcompUp(QPrivateSignal());
        }
    }
}

/** no bind trigger function */
void RemoteComponentBase::noBind()
{
    if (m_state == State::Bind) {
        emit fsmBindNoBind(QPrivateSignal());
    }
}

/** pins synced trigger function */
void RemoteComponentBase::pinsSynced()
{
    if (m_state == State::Sync) {
        emit fsmSyncPinsSynced(QPrivateSignal());
    }
}

/** start trigger function */
void RemoteComponentBase::start()
{
    if (m_state == State::Down) {
        emit fsmDownConnect(QPrivateSignal());
    }
}

/** stop trigger function */
void RemoteComponentBase::stop()
{
    if (m_state == State::Trying) {
        emit fsmTryingDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Binding) {
        emit fsmBindingDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Syncing) {
        emit fsmSyncingDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Synced) {
        emit fsmSyncedDisconnect(QPrivateSignal());
    }
    else if (m_state == State::Error) {
        emit fsmErrorDisconnect(QPrivateSignal());
    }
}

} } // namespace machinetalk::halremote
