#include "options.h"
#include "qcommandlineoption.h"
#include "qcommandlineparser.h"
#include <iostream>
#include <memory>

CommandLineParseResult parseMessageModeOptions(QCommandLineParser &parser,
                                               MessageModeOptions *options,
                                               QString *errorMessage) {
  auto helpOpt = parser.addHelpOption();

  QCommandLineOption withoutCloseBtnOpt(
      "without-close-button", "Lancer l'application sans bouton de fermeture");
  parser.addOption(withoutCloseBtnOpt);

  parser.addPositionalArgument("url", "L'url à charger (adresse ou fichier)");

  if (!parser.parse(QCoreApplication::arguments())) {
    *errorMessage = parser.errorText();
    return CommandLineError;
  }

  if (parser.isSet(helpOpt)) {
    return CommandLineHelpRequested;
  }

  bool withoutCloseBtn = parser.isSet(withoutCloseBtnOpt);

  QStringList argsList = parser.positionalArguments();

  if (argsList.length() < 1) {
    *errorMessage = "Missing url argument";
    return CommandLineError;
  }

  options->url = argsList.at(0);
  options->withoutCloseButton = withoutCloseBtn;

  return CommandLineOk;
}

CommandLineParseResult parsePolicyModeOptions(QCommandLineParser &parser,
                                              PolicyModeOptions *options,
                                              QString *errorMessage) {
  QCommandLineOption userOpt("user", "The user accepting the policy", "user");
  parser.addOption(userOpt);

  QCommandLineOption policyIdOpt("policyId", "The id of the policy to accept",
                                 "policyId");
  parser.addOption(policyIdOpt);

  QCommandLineOption durationOpt("duration",
                                 "The numbr of seconds during which the policy "
                                 "can be accepted (default is 600)",
                                 "duration", "600");
  parser.addOption(durationOpt);

  parser.addPositionalArgument("url", "The url to load (address or file)");

  if (!parser.parse(QCoreApplication::arguments())) {
    *errorMessage = parser.errorText();
    return CommandLineError;
  }

  QStringList argsList = parser.positionalArguments();

  if (argsList.length() < 1) {
    *errorMessage = "Missing url argument";
    return CommandLineError;
  }

  options->url = argsList.at(0);

  if (!parser.isSet(policyIdOpt)) {
    *errorMessage = "Missing policyId option";
    return CommandLineError;
  }

  auto policyIdValue = parser.value(policyIdOpt);

  bool isPolicyIdInt = false;
  auto policyId = policyIdValue.toInt(&isPolicyIdInt);

  if (!isPolicyIdInt) {
    *errorMessage = "policyId should be an integer";
    return CommandLineError;
  }

  options->policyId = policyId;

  if (!parser.isSet(userOpt)) {
    *errorMessage = "Missing user option";
    return CommandLineError;
  }

  options->user = parser.value(userOpt);

  options->duration = parser.value(durationOpt).toInt();

  return CommandLineOk;
}
