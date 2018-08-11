#include "ws2common/MessageHandler.hpp"
#include "ws2common/EnumGameVersion.hpp"
#include "ws2common/config/XMLConfigParser.hpp"
#include "ws2common/config/StagedefConfigParser.hpp"
#include "ws2common/model/ModelLoader.hpp"
#include "ws2lz/SMB2LzExporter.hpp"
#include <QCoreApplication>
#include <QTranslator>
#include <QCommandLineParser>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QBuffer>
#include <QDataStream>
#include <QHash>
#include <QDebug>

int main(int argc, char *argv[]) {
    qInstallMessageHandler(WS2Common::messageHandler);

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("ws2lzfrontend");

    //Load translations
    QTranslator translator;
    if (translator.load(QLocale(), QLatin1String("lang"), QLatin1String("_"), QDir(QCoreApplication::applicationDirPath()).filePath("../share/ws2editor/lang"))) {
        app.installTranslator(&translator);
    } else if (translator.load(QLocale(), QLatin1String("lang"), QLatin1String("_"), QCoreApplication::applicationDirPath())) {
        //If the software was never installed after build, the translations will be alongside the executable
        app.installTranslator(&translator);
    } else if (translator.load("lang_en_US", QDir(QCoreApplication::applicationDirPath()).filePath("../share/ws2editor/lang"))) {
        //If we can't find a suitable translation, try en_US
        app.installTranslator(&translator);
    } else if (translator.load("lang_en_US", QCoreApplication::applicationDirPath())) {
        //If we can't find a suitable translation, try en_US - if not intalled
        app.installTranslator(&translator);
    }

    QCommandLineParser parser;
    parser.setApplicationDescription("An interface for ws2lz, to export LZ files for use within Super Monkey Ball");
    parser.addHelpOption();

    parser.addOptions({
            {{"c", "config"}, QCoreApplication::translate("main", "Input path to the XML configuration file.\n"
                    "Cannot be used with -r"), QCoreApplication::translate("main", "config file")},
            {{"d", "stagedef-config"}, QCoreApplication::translate("main", "Input path to a stagedef (.stagedef / .lz.raw) to use as a stage configuration.\n"
                    "This will automatically detect whether an SMB 1 or SMB 2 config is being used.\n"
                    "Cannot be used with -c"), QCoreApplication::translate("main", "stagedef file")},
            {{"s", "compressed-output"}, QCoreApplication::translate("main", "Output path to a compressed stagedef (.lz) file."), QCoreApplication::translate("main", "output file")},
            {{"o", "output"}, QCoreApplication::translate("main", "Output path to a stagedef file (.stagedef / .lz.raw)."), QCoreApplication::translate("main", "stagedef")},
            {{"g", "game-version"}, QCoreApplication::translate("main", "The version of SMB to generate an stagedef file for (1/2/deluxe)."), QCoreApplication::translate("main", "version")}
            });

    parser.process(app);

    //Some variables
    WS2Common::EnumGameVersion gameVersion = WS2Common::EnumGameVersion::SUPER_MONKEY_BALL_2;

    if (app.arguments().length() == 1) {
        //No arguments - print help
        parser.showHelp(EXIT_FAILURE);
    }

    if (!parser.parse(QCoreApplication::arguments())) {
        QString err = parser.errorText();
        qCritical().noquote() << err;
        return EXIT_FAILURE;
    }

    //Check for a valid input
    if (!(parser.isSet("c") || parser.isSet("d"))) {
        qCritical().noquote() << QCoreApplication::translate("main", "No confiuration file specified. Use --help for more info.");
        return EXIT_FAILURE;
    } else if (parser.isSet("c") && parser.isSet("d")) {
        qCritical().noquote() << QCoreApplication::translate("main", "Cannot specify both an XML config and a raw LZ config. Use --help for more info.");
        return EXIT_FAILURE;
    }

    //Check for a valid output
    if (!(parser.isSet("o") || parser.isSet("s"))) {
        qCritical().noquote() << QCoreApplication::translate("main", "No output file specified. Use --help for more info.");
        return EXIT_FAILURE;
    }

    //Check that the game version was specified, otherwise default to SMB 2
    if (!parser.isSet("g")) {
        qWarning().noquote() << QCoreApplication::translate("main", "No game version specified - defaulting to Super Monkey Ball 2 - Use --game-version to specify a different one");
    } else {
        //The --game-version switch was set
        //Check that the game version is valid
        if (parser.value("g") == "1") {
            gameVersion = WS2Common::EnumGameVersion::SUPER_MONKEY_BALL_1;
        } else if (parser.value("g") == "2") {
            gameVersion = WS2Common::EnumGameVersion::SUPER_MONKEY_BALL_2;
        } else if (parser.value("g") == "deluxe") {
            gameVersion = WS2Common::EnumGameVersion::SUPER_MONKEY_BALL_DELUXE;
        } else {
            //Invalid version
            qCritical().noquote() << QCoreApplication::translate("main", "Invalid game version specified. Use --help for more info.");
        }
    }

    WS2Common::Stage *stage;

    qInfo() << "Reading configuration...";
    if (parser.isSet("c")) {
        QFile configFile(parser.value("c"));
        configFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString config = configFile.readAll();
        configFile.close();

        //Get the config file directory, for relative file paths in the config
        QFileInfo configFileInfo(configFile);
        QDir configFileDir = configFileInfo.dir();

        qInfo() << "Parsing configuration...";
        WS2Common::Config::XMLConfigParser confParser;
        stage = confParser.parseStage(config, configFileDir);
        qInfo() << stage->getRootNode();
    } else {
        // Raw LZ config
        QFile configFile(parser.value("d"));
        configFile.open(QIODevice::ReadOnly);
        QByteArray bytes = configFile.readAll();

        WS2Common::Config::StagedefConfigParser confParser;
        stage = confParser.parseStage(bytes);
        qInfo() << stage->getRootNode();
    }

    qInfo() << "Loading models...";
    //resources is only really used to conserve some memory, by preventing the creation of duplicate textures
    QMutex resourcesMutex;
    QVector<WS2Common::Resource::AbstractResource*> resources;
    QHash<QString, WS2Common::Resource::ResourceMesh*> models; //name, mesh - Using a hashmap as it will have a quicker lookup

    //Load each model
    foreach(QUrl url, stage->getModels()) {
        QFile file(url.toLocalFile()); //Assumes the URL is local //TODO: Allow network locations maybe
        QVector<WS2Common::Resource::ResourceMesh*> meshVec = WS2Common::Model::ModelLoader::loadModel(file, &resources, &resourcesMutex);
        foreach(WS2Common::Resource::ResourceMesh* mesh, meshVec) {
            models[mesh->getId()] = mesh; //TODO: Use some getMeshName function or something if I ever add that
        }
    }

    qInfo() << "Exporting file...";
    QBuffer buf;
    buf.open(QIODevice::ReadWrite);
    QDataStream dStream(&buf);

    if (gameVersion == WS2Common::EnumGameVersion::SUPER_MONKEY_BALL_1) {
        qCritical() << "SMB 1 export not yet implemented";
        return EXIT_FAILURE;
    } else if (gameVersion == WS2Common::EnumGameVersion::SUPER_MONKEY_BALL_2) {
        WS2Lz::SMB2LzExporter exporter;

        exporter.setModels(models);
        exporter.generate(dStream, *stage);
    }

    buf.seek(0);
    //Write the uncompressed file if requested
    if (parser.isSet("o")) {
        QFile o(parser.value("o"));
        o.open(QIODevice::WriteOnly);
        o.write(buf.data());
        o.close();
    }

    //TODO: Compression

    buf.close();

    //Cleanup
    delete stage;
    qDeleteAll(resources);
    //No need to qDeleteAll(models) - The resources vector contains the models that are all deleted

    return EXIT_SUCCESS;
}
