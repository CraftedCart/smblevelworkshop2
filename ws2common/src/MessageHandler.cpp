#include "MessageHandler.hpp"
#include <stdio.h>

namespace WS2Common {

    /**
     * @brief Custom message handler for logging - Install it with qInstallMessageHandler(messageHandler)
     *
     * @param type The message type
     * @param context The message context
     * @param msg The message
     */
    void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        Q_UNUSED(context);

        QByteArray localMsg = msg.toLocal8Bit();

        //Only print with ANSI escape codes on UNIX systems - not the best solution, but it should suffice for most of the time

        switch (type) {
            case QtDebugMsg:
                fprintf(stdout, "%s\n", localMsg.constData());
                break;
            case QtInfoMsg:
                fprintf(stdout, "%s\n", localMsg.constData());
                break;
            case QtWarningMsg:
#ifdef Q_OS_UNIX
                fprintf(stderr, "\033[1;33mWarning\033[0m: %s\n", localMsg.constData());
#else
                fprintf(stderr, "Warning: %s\n", localMsg.constData());
#endif
                break;
            case QtCriticalMsg:
#ifdef Q_OS_UNIX
                fprintf(stderr, "\033[31mCritical\033[0m: %s\n", localMsg.constData());
#else
                fprintf(stderr, "Critical: %s\n", localMsg.constData());
#endif
                break;
            case QtFatalMsg:
#ifdef Q_OS_UNIX
                fprintf(stderr, "\033[31mFatal\033[0m: %s\n", localMsg.constData());
#else
                fprintf(stderr, "Fatal: %s\n", localMsg.constData());
#endif
                abort();
        }
    }
}
