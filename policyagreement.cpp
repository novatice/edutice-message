#include "policyagreement.h"
#include "qbuffer.h"
#include "qdebug.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qtimer.h"
#include <QNetworkReply>
#include <iostream>
#include <thread>

QString Agreement::asJson() {
  QJsonObject jsonObject;
  jsonObject.insert("agreed", this->success);
  jsonObject.insert("policyId", this->policyId);
  jsonObject.insert("username", this->username);
  jsonObject.insert("agreementId", this->agreementId);

  return QJsonDocument(jsonObject).toJson();
}

PolicyAgreement::PolicyAgreement(QString username, int policyId,
                                 QString agreementId, QObject *parent)
    : QObject{parent},
      m_policyId(policyId), m_username{username}, m_agreementId{agreementId} {

  this->manager = new QNetworkAccessManager(this);

  qDebug() << manager;
}

void PolicyAgreement::agree() {
  // send request
  qDebug() << "agree";
  this->sendAgreement(
      Agreement{true, this->m_policyId, this->m_username, this->m_agreementId});
}

void PolicyAgreement::disagree() {
  // send request
  qDebug() << "disagree";
  this->sendAgreement(Agreement{false, this->m_policyId, this->m_username,
                                this->m_agreementId});
}

void PolicyAgreement::sendAgreement(Agreement agreement) {
  auto timer = new QTimer(this);
  timer->setSingleShot(true);
  timer->setInterval(DEFAULT_TIMEOUT);

  auto request = new QNetworkRequest(QUrl("http://127.0.0.1:7002"));
  request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader,
                     "application/json");

  auto buffer = new QBuffer();
  buffer->open(QBuffer::ReadWrite);
  buffer->write(agreement.asJson().toUtf8());
  buffer->close();

  QNetworkReply *reply = this->manager->post(*request, buffer);
  timer->start();

  this->m_isRunning = true;
  emit this->isRunningChanged(true);

  QObject::connect(timer, &QTimer::timeout, timer, [reply]() {
    qDebug() << "timer is done !";
    if (!reply->isFinished()) {
      qDebug() << "aborting request !";
      reply->abort();
    }
  });

  QObject::connect(reply, &QNetworkReply::finished, reply,
                   [this, reply, agreement, timer]() {
                     this->m_isRunning = false;
                     emit this->isRunningChanged(false);
                     timer->stop();

                     // if agreement was not true, don't bother to check if http
                     // request was a success
                     if (agreement.success) {
                       auto error = reply->error();
                       std::cout << "error: " << error;
                       if (error == QNetworkReply::NetworkError::NoError) {
                         emit this->agreed();
                       } else {
                         emit this->agreedError();
                       }
                     } else {
                       emit this->disagreed();
                     }
                   });
}
