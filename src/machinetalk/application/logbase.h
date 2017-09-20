/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef APPLICATION_LOG_BASE_H
#define APPLICATION_LOG_BASE_H
#include <QObject>
#include <QSet>
#include <QQmlParserStatus>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include "../common/simplesubscribe.h"

namespace machinetalk { namespace application {

class LogBase
    : public QObject
    , public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString logUri READ logUri WRITE setLogUri NOTIFY logUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(State)

public:
    explicit LogBase(QObject *parent = nullptr);
    ~LogBase();

    enum class State {
        Down = 0,
        Trying = 1,
        Up = 2,
    };

    void classBegin() {}
    /** componentComplete is executed when the QML component is fully loaded */
    void componentComplete()
    {
        m_componentCompleted = true;

        if (m_ready == true)    // the component was set to ready before it was completed
        {
            start();
        }
    }

    QString logUri() const
    {
        return m_logChannel->socketUri();
    }

    QString debugName() const
    {
        return m_debugName;
    }

    State state() const
    {
        return m_state;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setLogUri(const QString &uri)
    {
        m_logChannel->setSocketUri(uri);
    }

    void setDebugName(const QString &debugName)
    {
        if (m_debugName == debugName) {
            return;
        }

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready) {
            return;
        }

        m_ready = ready;
        emit readyChanged(ready);

        if (m_componentCompleted == false)
        {
            return;
        }

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }

    void addLogTopic(const QByteArray &name);
    void removeLogTopic(const QByteArray &name);
    void clearLogTopics();

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_componentCompleted;
    bool m_ready;
    QString m_debugName;

    QSet<QByteArray> m_logTopics;      // the topics we are interested in
    common::SimpleSubscribe *m_logChannel;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    Container m_logRx;

private slots:

    void startLogChannel();
    void stopLogChannel();
    void logChannelStateChanged(common::SimpleSubscribe::State state);
    void processLogChannelMessage(const QByteArray &topic, const Container &rx);

    void fsmDown();
    void fsmDownConnectEvent();
    void fsmTrying();
    void fsmTryingLogUpEvent();
    void fsmTryingDisconnectEvent();
    void fsmUp();
    void fsmUpEntry();
    void fsmUpExit();
    void fsmUpDisconnectEvent();

    virtual void handleLogMessageMessage(const QByteArray &topic, const Container &rx) = 0;
    virtual void updateTopics() = 0;
    virtual void setConnected() = 0;
    virtual void clearConnected() = 0;

signals:
    void logUriChanged(const QString &uri);
    void logMessageReceived(const QByteArray &topic, const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(LogBase::State state);
    void errorStringChanged(const QString &errorString);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownConnect(QPrivateSignal);
    void fsmTryingEntered(QPrivateSignal);
    void fsmTryingExited(QPrivateSignal);
    void fsmTryingLogUp(QPrivateSignal);
    void fsmTryingDisconnect(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    void fsmUpDisconnect(QPrivateSignal);
};

} } // namespace machinetalk::application
#endif // APPLICATION_LOG_BASE_H
