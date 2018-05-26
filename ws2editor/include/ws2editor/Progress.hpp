/**
 * @file
 * @brief Header for the WS2Editor::Progress class WS2EDITOR_EXPORT
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PROGRESS_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PROGRESS_HPP

#include "ws2editor_export.h"
#include <QObject>
#include <QStack>

//Based off of the push design from https://softwareengineering.stackexchange.com/a/305833

namespace WS2Editor {

    /**
     * @brief Class for progress reporting.
     *
     * Tasks are nested and a new task can be started with begin(stepCount), and ended with end()
     * A task has stepCount operations to complete, and you can increment the operations done with inc()
     */
    class WS2EDITOR_EXPORT Progress : public QObject {
        Q_OBJECT

        public:
            /**
             * @brief Class for use in the progress operation stack
             */
            struct ProgressOperation {
                unsigned int stepCount;
                unsigned int currentStep = 0;
            };

        protected:

            /**
             * @brief opStack, short for Operation Stack
             */
            QStack<ProgressOperation*> opStack;

        public:
            /**
             * @brief Begin a new nested operation
             *
             * @param stepCount The number of steps required to complete this operation
             */
            void begin(unsigned int stepCount);

            /**
             * @brief Ends the current operation
             */
            void end();

            /**
             * @brief Increments the current operation by a single step
             */
            void inc();

            unsigned int getValue();
            unsigned int getMax();
            QStack<ProgressOperation*>& getOpStack();

        signals:
            void valueChanged(unsigned int value);
            void maxChanged(unsigned int max);
    };
}

#endif

