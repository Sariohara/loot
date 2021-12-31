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

#include "gui/qt/plugin_editor/main_tab.h"

#include <QtWidgets/QFormLayout>

#include "gui/qt/helpers.h"
#include "gui/plugin_item.h"

namespace loot {
MainTab::MainTab(QWidget* parent) :
    QWidget(parent), nonUserGroupName(DEFAULT_GROUP_NAME) {
  setupUi();
}

void MainTab::initialiseInputs(const std::vector<std::string>& groups,
                               const std::optional<std::string>& nonUserGroup,
                               const std::optional<std::string>& userGroup) {
  groupComboBox->clear();

  for (const auto& group : groups) {
    groupComboBox->addItem(QString::fromUtf8(group));
  }

  nonUserGroupName = nonUserGroup.value_or(DEFAULT_GROUP_NAME);

  if (userGroup.has_value()) {
    groupComboBox->setCurrentText(QString::fromUtf8(userGroup.value()));
  } else {
    groupComboBox->setCurrentText(QString::fromUtf8(nonUserGroupName));
  }
}

std::optional<std::string> MainTab::getUserMetadata() const {
  // Only return the selected group if it's different from the nonUserGroupName.
  auto name = groupComboBox->currentText().toStdString();

  if (name == nonUserGroupName) {
    return std::nullopt;
  }

  return name;
}

void MainTab::setupUi() {
  groupLabel = new QLabel(this);
  groupComboBox = new QComboBox(this);

  auto layout = new QFormLayout(this);

  layout->addRow(groupLabel, groupComboBox);

  translateUi();
}

void MainTab::translateUi() { groupLabel->setText(translate("Group")); }
}
