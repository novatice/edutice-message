#ifndef OPTIONS_H
#define OPTIONS_H

#include "qcommandlineparser.h"
#include "qstring.h"
#include <memory>

// taken from qt example at:
// https://doc.qt.io/qt-5/qcommandlineparser.html#how-to-use-qcommandlineparser-in-complex-applications
enum CommandLineParseResult {
  CommandLineOk,
  CommandLineError,
  CommandLineHelpRequested
};

struct Options {
  QString url;
};

struct MessageModeOptions : Options {
  bool withoutCloseButton;
};

struct PolicyModeOptions : Options {
  QString user;
  int policyId;
  int duration;
};

CommandLineParseResult parseMessageModeOptions(QCommandLineParser &parser,
                                               MessageModeOptions *options,
                                               QString *error);

CommandLineParseResult parsePolicyModeOptions(QCommandLineParser &parser,
                                              PolicyModeOptions *options,
                                              QString *error);

#endif // OPTIONS_H
