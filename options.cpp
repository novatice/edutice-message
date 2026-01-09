#include "options.h"
#include "qcommandlineoption.h"
#include "qcommandlineparser.h"
#include "qurl.h"
#include <windows.h>

const static std::wstring g_eduticeSubkey{L"SOFTWARE\\Novatice\\Edutice"};

QString GetStringFromReg(HKEY hKey, std::wstring path, std::wstring value)
{
    DWORD dataSize;
    LONG retCode = RegGetValueW(hKey,
                                path.c_str(),
                                value.c_str(),
                                RRF_RT_REG_SZ,
                                nullptr,
                                nullptr,
                                &dataSize);
    if (retCode != ERROR_SUCCESS) {
        qWarning("Coulndn't get REG_SZ value %s from Registry :: error = %s",
                 value.c_str(),
                 qUtf8Printable(std::to_string(retCode).c_str()));
        return NULL;
    }
    std::wstring data;
    data.resize(dataSize / sizeof(wchar_t));

    retCode = RegGetValueW(hKey,
                           path.c_str(),
                           value.c_str(),
                           RRF_RT_REG_SZ,
                           nullptr,
                           &data[0],
                           &dataSize);
    //we need to do something to be able to log value
    if (retCode != ERROR_SUCCESS) {
        qWarning("Coulndn't get REG_SZ value %s from Registry :: error = %s",
                 value.c_str(),
                 qUtf8Printable(std::to_string(retCode).c_str()));
        return NULL;
    }

    // resizing data from byte to wchar and remove double NULL termination
    data.resize(dataSize / sizeof(wchar_t) - 1);

    return QString::fromWCharArray(data.c_str());
}

bool CheckUrlIsNeosServer(QString arg)
{
    if (arg.startsWith("http") || arg.startsWith("https")) {
        QString serverHostname = GetStringFromReg(HKEY_LOCAL_MACHINE,
                                                  g_eduticeSubkey,
                                                  L"ServerHostname");
        QUrl urlArg = QUrl(arg);
        qDebug(urlArg.host().toStdString().c_str());
        if (serverHostname.isNull() || urlArg.host() != serverHostname) {
            qInfo("Url not from server, stopping application");
            return false;
        } else {
            return true;
        }
    }
    return true;
}

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

  if (!CheckUrlIsNeosServer(argsList.at(0))) {
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

  QCommandLineOption durationOpt("duration",
                                 "The number of seconds during which the policy "
                                 "can be accepted (default is 600)",
                                 "duration",
                                 "600");
  parser.addOption(durationOpt);

  QCommandLineOption agreementIdOpt("agreementId", "The agreement id",
                                    "agreementdId");
  parser.addOption(agreementIdOpt);

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

  if (!CheckUrlIsNeosServer(argsList.at(0))) {
      return CommandLineError;
  }
  options->url = argsList.at(0);

  if (!parser.isSet(userOpt)) {
    *errorMessage = "Missing user option";
    return CommandLineError;
  }

  options->user = parser.value(userOpt);

  if (!parser.isSet(agreementIdOpt)) {
    *errorMessage = "Missing agreementId option";
    return CommandLineError;
  }

  options->agreementId = parser.value(agreementIdOpt);

  options->duration = parser.value(durationOpt).toInt();

  return CommandLineOk;
}

