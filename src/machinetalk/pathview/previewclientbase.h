/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef PATHVIEW_PREVIEW_CLIENT_BASE_H
#define PATHVIEW_PREVIEW_CLIENT_BASE_H
#include <QObject>
#include <QSet>
#include <QQmlParserStatus>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>
#include "../pathview/previewsubscribe.h"
#include "../pathview/previewsubscribe.h"

namespace machinetalk { namespace pathview {

class PreviewClientBase
    : public QObject
    , public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString previewUri READ previewUri WRITE setPreviewUri NOTIFY previewUriChanged)
    Q_PROPERTY(QString previewstatusUri READ previewstatusUri WRITE setPreviewstatusUri NOTIFY previewstatusUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_ENUMS(State)

public:
    explicit PreviewClientBase(QObject *parent = nullptr);
    ~PreviewClientBase();

    enum class State {
        Down = 0,
        Trying = 1,
        Previewtrying = 2,
        Statustrying = 3,
        Up = 4,
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

    QString previewUri() const
    {
        return m_previewChannel->socketUri();
    }

    QString previewstatusUri() const
    {
        return m_previewstatusChannel->socketUri();
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

    void setPreviewUri(const QString &uri)
    {
        m_previewChannel->setSocketUri(uri);
    }

    void setPreviewstatusUri(const QString &uri)
    {
        m_previewstatusChannel->setSocketUri(uri);
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

    void addPreviewTopic(const QByteArray &name);
    void removePreviewTopic(const QByteArray &name);
    void clearPreviewTopics();
    void addPreviewstatusTopic(const QByteArray &name);
    void removePreviewstatusTopic(const QByteArray &name);
    void clearPreviewstatusTopics();

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_componentCompleted;
    bool m_ready;
    QString m_debugName;

    QSet<QByteArray> m_previewTopics;  // the topics we are interested in
    pathview::PreviewSubscribe *m_previewChannel;
    QSet<QByteArray> m_previewstatusTopics;  // the topics we are interested in
    pathview::PreviewSubscribe *m_previewstatusChannel;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;
    // more efficient to reuse a protobuf Messages
    Container m_previewRx;
    Container m_previewstatusRx;

private slots:

    void startPreviewChannel();
    void stopPreviewChannel();
    void previewChannelStateChanged(pathview::PreviewSubscribe::State state);
    void processPreviewChannelMessage(const QByteArray &topic, const Container &rx);

    void startPreviewstatusChannel();
    void stopPreviewstatusChannel();
    void previewstatusChannelStateChanged(pathview::PreviewSubscribe::State state);
    void processPreviewstatusChannelMessage(const QByteArray &topic, const Container &rx);

    void fsmDown();
    void fsmDownConnectEvent();
    void fsmTrying();
    void fsmTryingStatusUpEvent();
    void fsmTryingPreviewUpEvent();
    void fsmTryingDisconnectEvent();
    void fsmPreviewtrying();
    void fsmPreviewtryingPreviewUpEvent();
    void fsmPreviewtryingStatusTryingEvent();
    void fsmPreviewtryingDisconnectEvent();
    void fsmStatustrying();
    void fsmStatustryingStatusUpEvent();
    void fsmStatustryingPreviewTryingEvent();
    void fsmStatustryingDisconnectEvent();
    void fsmUp();
    void fsmUpEntry();
    void fsmUpExit();
    void fsmUpPreviewTryingEvent();
    void fsmUpStatusTryingEvent();
    void fsmUpDisconnectEvent();

    virtual void handlePreviewMessage(const QByteArray &topic, const Container &rx) = 0;
    virtual void handleInterpStatMessage(const QByteArray &topic, const Container &rx) = 0;
    virtual void setConnected() = 0;
    virtual void clearConnected() = 0;

signals:
    void previewUriChanged(const QString &uri);
    void previewstatusUriChanged(const QString &uri);
    void previewMessageReceived(const QByteArray &topic, const Container &rx);
    void previewstatusMessageReceived(const QByteArray &topic, const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(PreviewClientBase::State state);
    void errorStringChanged(const QString &errorString);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownConnect(QPrivateSignal);
    void fsmTryingEntered(QPrivateSignal);
    void fsmTryingExited(QPrivateSignal);
    void fsmTryingStatusUp(QPrivateSignal);
    void fsmTryingPreviewUp(QPrivateSignal);
    void fsmTryingDisconnect(QPrivateSignal);
    void fsmPreviewtryingEntered(QPrivateSignal);
    void fsmPreviewtryingExited(QPrivateSignal);
    void fsmPreviewtryingPreviewUp(QPrivateSignal);
    void fsmPreviewtryingStatusTrying(QPrivateSignal);
    void fsmPreviewtryingDisconnect(QPrivateSignal);
    void fsmStatustryingEntered(QPrivateSignal);
    void fsmStatustryingExited(QPrivateSignal);
    void fsmStatustryingStatusUp(QPrivateSignal);
    void fsmStatustryingPreviewTrying(QPrivateSignal);
    void fsmStatustryingDisconnect(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    void fsmUpPreviewTrying(QPrivateSignal);
    void fsmUpStatusTrying(QPrivateSignal);
    void fsmUpDisconnect(QPrivateSignal);
};

} } // namespace machinetalk::pathview
#endif // PATHVIEW_PREVIEW_CLIENT_BASE_H
