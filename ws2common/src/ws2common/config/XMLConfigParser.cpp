#include "ws2common/config/XMLConfigParser.hpp"
#include <QXmlStreamReader>
#include <QDebug>

namespace WS2Common {
    namespace Config {
        namespace XMLConfigParser {
            Stage* parseStage(QString config) {
                Stage *stage = new Stage();

                //Read and parse the XML
                QXmlStreamReader xml(config);
                while (!xml.atEnd()) {
                    xml.readNext();
                    if (!xml.isStartElement()) continue; //Ignore all end elements

                    if (xml.name() == "superMonkeyBallStage") {

                        //Parse the stages
                        while (!(xml.isEndElement() && xml.name() == "superMonkeyBallStage")) {
                            //Keep reading until the </superMonkeyBallStage> tag
                            xml.readNext();

                            //Aaand to absolutely nothing with it
                            //TODO: Do useful magic
                        }

                    } else {
                        qWarning().noquote() << QString("XML root tag is %1! It should be superMonkeyBallStage").arg(xml.name().toString());
                        //Don't spam the log with every single tag - Get outta here after the root one
                        break;
                    }
                }

                if (xml.hasError()) {
                    //TODO: Throw an exception or something
                    qWarning().noquote() << "XML parsing error:" << xml.errorString();
                }

                return stage;
            }
        }
    }
}

