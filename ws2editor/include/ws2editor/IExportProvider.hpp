/**
 * @file
 * @brief Header for the IExportProvider interface
 */

#ifndef SMBLEVELWORKSHOP2_WS2EDITOR_PLUGIN_IEXPORTPROVIDER_HPP
#define SMBLEVELWORKSHOP2_WS2EDITOR_PLUGIN_IEXPORTPROVIDER_HPP

#include "ws2editor_export.h"
#include "ws2editor/project/Project.hpp"
#include <QString>
#include <QVector>

namespace WS2Editor {
    class WS2EDITOR_EXPORT IExportProvider : public QObject {
        Q_OBJECT

        public:
            virtual ~IExportProvider() = default;

            /**
             * @brief Returns the type of file exported as a human readable translated string
             *
             * For example, if exporting raw SMB 2 LZs, you may return `tr("Super Monkey Ball 2 Raw LZ [*.lz.raw]")`
             *
             * @return The type of file exported as a human readable translated string
             */
            virtual QString getTranslatedTypeName() = 0;

            /**
             * @brief Returns a map of file extensions usable when exporting files
             *
             * ### Pair first: Human readable name filter
             *
             * Filters should be in the same format as Qt name filters (http://doc.qt.io/qt-5/qfiledialog.html#setNameFilters)
             *
             * Examples include
             * - *.ext1 *.ext2 *.ext3
             * - Various files (*.ext1 *.ext2 *.ext3)
             *
             * You do not need to include "*" in the list as "All files (*)" will be added by ws2editor to the chosable
             * type list for all exporters. If your file format has no specific extension, leave this list empty.
             *
             * ### Pair second: Default file extension
             *
             * Examples include
             * - .lz.raw
             * - .xml
             *
             * @return A vector of file extensions usable when exporting files
             */
            virtual QVector<QPair<QString, QString>> getNameFilters() = 0;

            /**
             * @brief Generates export data and writes it to each target file
             *
             * @param targetFiles A list of files to save to
             * @param project The project to export
             */
            virtual void exportFiles(QStringList targetFiles, Project::Project *project) = 0;
    };
}

#endif

