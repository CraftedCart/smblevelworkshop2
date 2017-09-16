/**
 * @file
 * @brief Header for the message handler, for logging output
 */

#include <QtGlobal>
#include <QString>

namespace WS2Common {
    void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
}

