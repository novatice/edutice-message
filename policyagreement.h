#ifndef POLICYAGREEMENT_H
#define POLICYAGREEMENT_H

#include "qnetworkaccessmanager.h"
#include "qobjectdefs.h"
#include <QObject>

struct Agreement {
  bool success;
  int policyId;
  QString username;

  QString asJson();
};

class PolicyAgreement : public QObject {
  Q_OBJECT
public:
  PolicyAgreement(QString username, int policyId, QObject *parent = nullptr);
  Q_INVOKABLE void agree();
  Q_INVOKABLE void disagree();
  Q_PROPERTY(bool isRunning MEMBER m_isRunning NOTIFY isRunningChanged);

  // default timeout for a request is 5000 msecs
  static constexpr int DEFAULT_TIMEOUT = 5000;

signals:
  void agreed();
  void agreedError();
  void disagreed();
  void isRunningChanged(const bool &isRunning);

private:
  void sendAgreement(Agreement agreement);
  QNetworkAccessManager *manager;
  bool m_isRunning{false};
  int m_policyId;
  QString m_username;
};

#endif // POLICYAGREEMENT_H
