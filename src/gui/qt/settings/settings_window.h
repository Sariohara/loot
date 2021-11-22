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

#ifndef LOOT_GUI_QT_SETTINGS_SETTINGS_WINDOW
#define LOOT_GUI_QT_SETTINGS_SETTINGS_WINDOW

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

#include "gui/qt/settings/game_tab.h"
#include "gui/qt/settings/general_tab.h"
#include "gui/qt/settings/new_game_tab.h"

namespace loot {
class VerticalTabStyle : public QProxyStyle {
public:
  QSize sizeFromContents(ContentsType type,
                         const QStyleOption *option,
                         const QSize &size,
                         const QWidget *widget) const;

  void drawControl(ControlElement element,
                   const QStyleOption *option,
                   QPainter *painter,
                   const QWidget *widget) const;
};

class SettingsWindow : public QDialog {
  Q_OBJECT
public:
  SettingsWindow(QWidget *parent);

  void initialiseInputs(const LootSettings &settings,
                        const std::vector<std::string> &themes,
                        const std::optional<std::string> &currentGameFolder);

  void recordInputValues(LootSettings &settings);

private:
  QAbstractButton *addGameButton;
  QTabWidget *tabs;
  GeneralTab *generalTab;
  NewGameTab *newGameTab;
  VerticalTabStyle verticalTabStyle;

  void setupUi();
  void translateUi();

private slots:
  void on_dialogButtons_accepted();
  void on_dialogButtons_rejected();
  void on_newGameTab_accepted();

  void onGameSettingsDeleted();
};
}

#endif
