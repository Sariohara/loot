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

#ifndef LOOT_GUI_QT_PLUGIN_EDITOR_MAIN_TAB
#define LOOT_GUI_QT_PLUGIN_EDITOR_MAIN_TAB

#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

#include "gui/query/derived_plugin_metadata.h"

namespace loot {
class MainTab : public QWidget {
  Q_OBJECT
public:
  MainTab(QWidget* parent);

  void initialiseInputs(const std::vector<std::string>& groups,
                        const std::optional<std::string>& nonUserGroup,
                        const std::optional<std::string>& userGroup);

  std::optional<std::string> getUserMetadata() const;

private:
  QLabel* groupLabel;
  QComboBox* groupComboBox;
  std::string nonUserGroupName;

  void setupUi();
  void translateUi();
};
}
#endif