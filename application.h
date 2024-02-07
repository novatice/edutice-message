#ifndef APPLICATION_H
#define APPLICATION_H

#include "options.h"
#include "policyagreement.h"
#include "qguiapplication.h"
#include "qqmlapplicationengine.h"
#include "qqmlcontext.h"
#include "qtwebengineglobal.h"

template <class T> class Application {
public:
  explicit Application(QGuiApplication &app, QString qmlFile, T options)
      : m_app(app) {
    QtWebEngine::initialize();

    // const QString qmlFile = getQmlFileFor(options->mode);
    m_qmlUrl = QUrl(qmlFile);

    m_engine.addImportPath("qrc:/qml/");

    m_engine.rootContext()->setContextProperty("urlToLoad", options.url);

    QObject::connect(
        &m_engine, &QQmlApplicationEngine::objectCreated, &app,
        [this](QObject *obj, const QUrl &objUrl) {
          if (!obj && m_qmlUrl == objUrl)
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
  };

  int execute() {
    m_engine.load(m_qmlUrl);
    return m_app.exec();
  }

protected:
  const QQmlApplicationEngine &getEngine() { return m_engine; }

private:
  QGuiApplication &m_app;
  QQmlApplicationEngine m_engine;
  QUrl m_qmlUrl;
};

class MessageApplication : public Application<MessageModeOptions> {
public:
  explicit MessageApplication(QGuiApplication &app, MessageModeOptions options)
      : Application<MessageModeOptions>(app, "qrc:/qml/message.qml", options) {
    auto &engine = getEngine();

    engine.rootContext()->setContextProperty("withoutCloseButton",
                                             options.withoutCloseButton);
  };
};

class PolicyApplication : public Application<PolicyModeOptions> {
public:
  explicit PolicyApplication(QGuiApplication &app, PolicyModeOptions options)
      : Application(app, "qrc:/qml/policy.qml", options),
        m_policyAgreement{options.user, options.policyId} {

    auto &engine = getEngine();

    engine.rootContext()->setContextProperty("policyAgreement",
                                             &this->m_policyAgreement);
    engine.rootContext()->setContextProperty("duration", options.duration);
  };

private:
  PolicyAgreement m_policyAgreement;
};

#endif // APPLICATION_H
