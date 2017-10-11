/**
 * @file
 * @brief Header for the XMLConfigParser namespace
 */

#include "ws2common/Stage.hpp"

namespace WS2Common {
    namespace Config {
        namespace XMLConfigParser {

            /**
             * @brief Parses and XML config and converts it into a Stage object
             *
             * @param config The XML as a string to parse
             *
             * @return A pointer to stage created
             */
            Stage* parseStage(QString config);
        }
    }
}

