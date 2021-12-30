/*  LOOT

    A load order optimisation tool for
    Morrowind, Oblivion, Skyrim, Skyrim Special Edition, Skyrim VR,
    Fallout 3, Fallout: New Vegas, Fallout 4 and Fallout 4 VR.

    Copyright (C) 2021    Oliver Hamlet

    This file is part of LOOT.

    LOOT is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    LOOT is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LOOT.  If not, see
    <https://www.gnu.org/licenses/>.
    */

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>

#include "gui/application_mutex.h"
#include "gui/qt/main_window.h"
#include "gui/qt/style.h"
#include "gui/state/loot_state.h"

int main(int argc, char* argv[]) {
#ifdef _WIN32
  // Check if LOOT is already running
  //---------------------------------

  if (loot::IsApplicationMutexLocked()) {
    // An instance of LOOT is already running, so focus its window then quit.
    HWND hWnd = ::FindWindow(NULL, L"LOOT");
    ::SetForegroundWindow(hWnd);
    return 0;
  }
#endif

  loot::ApplicationMutexGuard mutexGuard;

  QApplication app(argc, argv);

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOptions(
      {{"game",
        QString::fromUtf8("Set the game that LOOT will initially load"),
        QString::fromUtf8("game")},
       {"loot-data-path",
        QString::fromUtf8("Set the directory where LOOT will store its data"),
        QString::fromUtf8("directory")},
       {"auto-sort",
        QString::fromUtf8("Automatically sort the load order on launch")}});
  parser.process(app);

  auto lootDataPath =
      std::filesystem::u8path(parser.value("loot-data-path").toStdString());
  auto startupGameFolder = parser.value("game").toStdString();
  auto autoSort = parser.isSet("auto-sort");

  loot::LootState state("", lootDataPath);
  state.init(startupGameFolder, autoSort);

  // Load Qt's translations.
  QTranslator translator;

  auto loaded =
      translator.load(QLocale(QString::fromUtf8(state.getLanguage())),
                      QString("qt"),
                      QString("_"),
                      QLibraryInfo::path(QLibraryInfo::TranslationsPath));

  if (loaded) {
    app.installTranslator(&translator);
  }

  // Apply theme.
  auto styleSheet =
      loot::loadStyleSheet(state.getResourcesPath(), state.getTheme());
  if (styleSheet.has_value()) {
    qApp->setStyleSheet(styleSheet.value());
    auto palette =
        loot::loadPalette(state.getResourcesPath(), state.getTheme());
    if (palette.has_value()) {
      qApp->setPalette(palette.value());
    }
  } else {
    // Fall back to the default theme, which has a stylesheet but no palette
    // config.
    styleSheet = loot::loadStyleSheet(state.getResourcesPath(), "default");
    if (styleSheet.has_value()) {
      qApp->setStyleSheet(styleSheet.value());
    }
  }

  loot::MainWindow mainWindow(state);

  auto wasMaximised = state.getWindowPosition()
                          .value_or(loot::LootSettings::WindowPosition())
                          .maximised;

  if (wasMaximised) {
    mainWindow.showMaximized();
  } else {
    mainWindow.show();
  }

  mainWindow.initialise();

  return app.exec();
}
