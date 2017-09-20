/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#include "subscribe.h"
#include <google/protobuf/text_format.h>
#include "debughelper.h"

#if defined(Q_OS_IOS)
namespace gpb = google_public::protobuf;
#else
namespace gpb = google::protobuf;
#endif

using namespace nzmqt;

namespace machinetalk { namespace common {

/** Generic Subscribe implementation */
Subscribe::Subscribe(QObject *parent)
    : QObject(parent)
    , m_ready(false)
    , m_debugName("Subscribe")
    , m_socketUri("")
    , m_context(nullptr)
    , m_socket(nullptr)
    , m_state(State::Down)
    , m_previousState(State::Down)
    , m_errorString("")
    , m_heartbeatInterval(2500)
    , m_heartbeatLiveness(0)
    , m_heartbeatResetLiveness(5)
{

    m_heartbeatTimer.setSingleShot(true);
    connect(&m_heartbeatTimer, &QTimer::timeout, this, &Subscribe::heartbeatTimerTick);
    // state machine
    connect(this, &Subscribe::fsmDownStart,
            this, &Subscribe::fsmDownStartEvent);
    connect(this, &Subscribe::fsmTryingFullUpdateReceived,
            this, &Subscribe::fsmTryingFullUpdateReceivedEvent);
    connect(this, &Subscribe::fsmTryingStop,
            this, &Subscribe::fsmTryingStopEvent);
    connect(this, &Subscribe::fsmUpHeartbeatTimeout,
            this, &Subscribe::fsmUpHeartbeatTimeoutEvent);
    connect(this, &Subscribe::fsmUpHeartbeatTick,
            this, &Subscribe::fsmUpHeartbeatTickEvent);
    connect(this, &Subscribe::fsmUpAnyMsgReceived,
            this, &Subscribe::fsmUpAnyMsgReceivedEvent);
    connect(this, &Subscribe::fsmUpStop,
            this, &Subscribe::fsmUpStopEvent);

    m_context = new PollingZMQContext(this, 1);
    connect(m_context, &PollingZMQContext::pollError,
            this, &Subscribe::socketError);
    m_context->start();
}

Subscribe::~Subscribe()
{
    stopSocket();

    if (m_context != nullptr)
    {
        m_context->stop();
        m_context->deleteLater();
        m_context = nullptr;
    }
}

/** Add a topic that should be subscribed **/
void Subscribe::addSocketTopic(const QByteArray &name)
{
    m_socketTopics.insert(name);
}

/** Removes a topic from the list of topics that should be subscribed **/
void Subscribe::removeSocketTopic(const QByteArray &name)
{
    m_socketTopics.remove(name);
}

/** Clears the the topics that should be subscribed **/
void Subscribe::clearSocketTopics()
{
    m_socketTopics.clear();
}

/** Connects the 0MQ sockets */
bool Subscribe::startSocket()
{
    m_socket = m_context->createSocket(ZMQSocket::TYP_SUB, this);
    m_socket->setLinger(0);

    try {
        m_socket->connectTo(m_socketUri);
    }
    catch (const zmq::error_t &e) {
        const QString errorString = QString("Error %1: ").arg(e.num()) + QString(e.what());
        qCritical() << m_debugName << ":" << errorString;
        return false;
    }

    connect(m_socket, &ZMQSocket::messageReceived,
            this, &Subscribe::processSocketMessage);


    for (const auto &topic: m_socketTopics)
    {
        m_socket->subscribeTo(topic);
    }

#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "sockets connected" << m_socketUri);
#endif

    return true;
}

/** Disconnects the 0MQ sockets */
void Subscribe::stopSocket()
{
    if (m_socket != nullptr)
    {
        m_socket->close();
        m_socket->deleteLater();
        m_socket = nullptr;
    }
}

void Subscribe::resetHeartbeatLiveness()
{
    m_heartbeatLiveness = m_heartbeatResetLiveness;
}

void Subscribe::resetHeartbeatTimer()
{
    if (m_heartbeatTimer.isActive())
    {
        m_heartbeatTimer.stop();
    }

    if (m_heartbeatInterval > 0)
    {
        m_heartbeatTimer.setInterval(m_heartbeatInterval);
        m_heartbeatTimer.start();
    }
}

void Subscribe::startHeartbeatTimer()
{
    resetHeartbeatTimer();
}

void Subscribe::stopHeartbeatTimer()
{
    m_heartbeatTimer.stop();
}

void Subscribe::heartbeatTimerTick()
{
    m_heartbeatLiveness -= 1;
    if (m_heartbeatLiveness == 0)
    {
         if (m_state == State::Up)
         {
             emit fsmUpHeartbeatTimeout(QPrivateSignal());
         }
         return;
    }
    if (m_state == State::Up)
    {
        emit fsmUpHeartbeatTick(QPrivateSignal());
    }
}

/** Processes all message received on socket */
void Subscribe::processSocketMessage(const QList<QByteArray> &messageList)
{
    Container &rx = m_socketRx;

    if (messageList.length() < 2)  // in case we received insufficient data
    {
        return;
    }

    // we only handle the first two messges
    const auto &topic = messageList.first();
    rx.ParseFromArray(messageList.last().data(), messageList.last().size());

#ifdef QT_DEBUG
    std::string s;
    gpb::TextFormat::PrintToString(rx, &s);
    DEBUG_TAG(3, m_debugName, "received message" << QString::fromStdString(s));
#endif

    // react to any incoming message

    if (m_state == State::Up)
    {
        emit fsmUpAnyMsgReceived(QPrivateSignal());
    }

    // react to ping message
    if (rx.type() == MT_PING)
    {
        return; // ping is uninteresting
    }

    // react to full update message
    else if (rx.type() == MT_FULL_UPDATE)
    {
        if (rx.has_pparams())
        {
            ProtocolParameters pparams = rx.pparams();
            m_heartbeatInterval = pparams.keepalive_timer();
        }

        if (m_state == State::Trying)
        {
            emit fsmTryingFullUpdateReceived(QPrivateSignal());
        }
    }

    emit socketMessageReceived(topic, rx);
}

void Subscribe::socketError(int errorNum, const QString &errorMsg)
{
    const QString errorString = QString("Error %1: ").arg(errorNum) + errorMsg;
    qCritical() << errorString;
}

void Subscribe::fsmDown()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State DOWN");
#endif
    m_state = State::Down;
    emit stateChanged(m_state);
}

void Subscribe::fsmDownStartEvent()
{
    if (m_state == State::Down)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event START");
#endif
        // handle state change
        emit fsmDownExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        startSocket();
     }
}

void Subscribe::fsmTrying()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State TRYING");
#endif
    m_state = State::Trying;
    emit stateChanged(m_state);
}

void Subscribe::fsmTryingFullUpdateReceivedEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event FULL UPDATE RECEIVED");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmUp();
        emit fsmUpEntered(QPrivateSignal());
        // execute actions
        resetHeartbeatLiveness();
        startHeartbeatTimer();
     }
}

void Subscribe::fsmTryingStopEvent()
{
    if (m_state == State::Trying)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STOP");
#endif
        // handle state change
        emit fsmTryingExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHeartbeatTimer();
        stopSocket();
     }
}

void Subscribe::fsmUp()
{
#ifdef QT_DEBUG
    DEBUG_TAG(1, m_debugName, "State UP");
#endif
    m_state = State::Up;
    emit stateChanged(m_state);
}

void Subscribe::fsmUpHeartbeatTimeoutEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HEARTBEAT TIMEOUT");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmTrying();
        emit fsmTryingEntered(QPrivateSignal());
        // execute actions
        stopHeartbeatTimer();
        stopSocket();
        startSocket();
     }
}

void Subscribe::fsmUpHeartbeatTickEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event HEARTBEAT TICK");
#endif
        // execute actions
        resetHeartbeatTimer();
     }
}

void Subscribe::fsmUpAnyMsgReceivedEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event ANY MSG RECEIVED");
#endif
        // execute actions
        resetHeartbeatLiveness();
        resetHeartbeatTimer();
     }
}

void Subscribe::fsmUpStopEvent()
{
    if (m_state == State::Up)
    {
#ifdef QT_DEBUG
        DEBUG_TAG(1, m_debugName, "Event STOP");
#endif
        // handle state change
        emit fsmUpExited(QPrivateSignal());
        fsmDown();
        emit fsmDownEntered(QPrivateSignal());
        // execute actions
        stopHeartbeatTimer();
        stopSocket();
     }
}

/** start trigger function */
void Subscribe::start()
{
    if (m_state == State::Down) {
        emit fsmDownStart(QPrivateSignal());
    }
}

/** stop trigger function */
void Subscribe::stop()
{
    if (m_state == State::Trying) {
        emit fsmTryingStop(QPrivateSignal());
    }
    else if (m_state == State::Up) {
        emit fsmUpStop(QPrivateSignal());
    }
}

} } // namespace machinetalk::common
