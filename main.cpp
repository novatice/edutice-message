#include "application.h"
#include "options.h"
#include "qdebug.h"
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QtWebEngineQuick/QtWebEngineQuick>
#include <iostream>

void treatParsingResult(QCommandLineParser &parser,
                        CommandLineParseResult result, QString *errorMessage) {
  switch (result) {
  case CommandLineOk:
    break;
  case CommandLineError:
    std::cerr << qPrintable(*errorMessage) << std::endl;
  case CommandLineHelpRequested:
    parser.showHelp(-1);
  }
}

int main(int argc, char *argv[]) {
  QGuiApplication qtApp(argc, argv);
  const auto screens = qtApp.screens();

  QCommandLineParser parser;

  auto helpOption = parser.addHelpOption();

  QCommandLineOption modeOpt(
      "mode", "Lancer l'application dans le mode spécifié (MESSAGE ou POLICY)",
      "mode");
  parser.addOption(modeOpt);

  parser.parse(QCoreApplication::arguments());

  auto modeIsSet = parser.isSet(modeOpt);

  if (!modeIsSet && parser.isSet(helpOption)) {
    parser.showHelp(0);
  }

  auto mode = modeIsSet ? parser.value(modeOpt).toLower() : "message";
  qDebug() << "mode: " << mode;

  QString error;
  if (mode == "policy") {
    PolicyModeOptions options;
    auto result = parsePolicyModeOptions(parser, &options, &error);

    treatParsingResult(parser, result, &error);

    return PolicyApplication(qtApp, options).execute();
  } else if (mode == "message") {
    MessageModeOptions options;
    auto result = parseMessageModeOptions(parser, &options, &error);

    treatParsingResult(parser, result, &error);

    return MessageApplication(qtApp, options).execute();
  } else {
    // qFatal("Unknown mode: %s", mode.toStdString().c_str());
    parser.showHelp(-1);
  }
}
