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

#include "gui/qt/groups_editor/groups_editor_dialog.h"

#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <boost/format.hpp>
#include <boost/locale.hpp>

#include "gui/qt/helpers.h"

namespace loot {
GroupsEditorDialog::GroupsEditorDialog(QWidget* parent,
                                       PluginItemModel* pluginItemModel) :
    QDialog(parent), pluginItemModel(pluginItemModel) {
  setupUi();
}

void GroupsEditorDialog::setGroups(
    const std::vector<Group>& masterlistGroups,
    const std::vector<Group>& userGroups,
    const std::set<std::string>& installedPluginGroups) {
  graphWidget->setGroups(masterlistGroups, userGroups, installedPluginGroups);
  groupPluginsTitle->setVisible(false);
  groupPluginsList->setVisible(false);
}

std::vector<Group> loot::GroupsEditorDialog::getUserGroups() const {
  return graphWidget->getUserGroups();
}

void GroupsEditorDialog::setupUi() {
  setWindowModality(Qt::WindowModal);

  graphWidget = new GraphWidget(this);
  graphWidget->setObjectName("graphWidget");
  groupPluginsTitle = new QLabel(this);
  groupPluginsTitle->setVisible(false);

  groupPluginsList = new QListWidget(this);
  groupPluginsList->setVisible(false);
  groupPluginsList->setSelectionMode(QAbstractItemView::NoSelection);

  auto verticalSpacer =
      new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

  groupNameInputLabel = new QLabel(this);
  groupNameInput = new QLineEdit(this);
  groupNameInput->setObjectName("groupNameInput");

  addGroupButton = new QPushButton(this);
  addGroupButton->setObjectName("addGroupButton");
  addGroupButton->setDisabled(true);

  auto buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
  buttonBox->setObjectName("dialogButtons");

  auto dialogLayout = new QVBoxLayout(this);
  auto mainLayout = new QHBoxLayout(this);
  auto sidebarLayout = new QVBoxLayout(this);
  auto formLayout = new QFormLayout(this);

  formLayout->addRow(groupNameInputLabel, groupNameInput);
  formLayout->addWidget(addGroupButton);

  sidebarLayout->addWidget(groupPluginsTitle);
  sidebarLayout->addWidget(groupPluginsList, 1);
  sidebarLayout->addSpacerItem(verticalSpacer);
  sidebarLayout->addLayout(formLayout);

  mainLayout->addWidget(graphWidget, 1);
  mainLayout->addLayout(sidebarLayout);

  dialogLayout->addLayout(mainLayout);
  dialogLayout->addWidget(buttonBox);

  translateUi();

  QMetaObject::connectSlotsByName(this);
}

void GroupsEditorDialog::translateUi() {
  setWindowTitle(translate("Groups Editor"));

  groupNameInputLabel->setText(translate("Group name"));
  addGroupButton->setText(translate("Add a new group"));
}

void GroupsEditorDialog::on_graphWidget_groupSelected(const QString& name) {
  groupPluginsList->clear();
  groupPluginsTitle->clear();

  auto groupName = name.toStdString();

  for (const auto& plugin : pluginItemModel->getPluginItems()) {
    auto pluginGroup = plugin.group.value_or(DEFAULT_GROUP_NAME);

    if (pluginGroup == groupName) {
      groupPluginsList->addItem(QString::fromUtf8(plugin.name));
    }
  }

  if (groupPluginsList->count() == 0) {
    auto text = translate("No plugins are in this group.");
    auto item = new QListWidgetItem();

    auto font = item->font();
    font.setItalic(true);
    item->setFont(font);
    item->setText(text);

    groupPluginsList->addItem(item);
  }

  auto titleText =
      (boost::format(boost::locale::translate("Plugins in %s")) % groupName)
          .str();

  groupPluginsTitle->setText(QString::fromUtf8(titleText));

  groupPluginsTitle->setVisible(true);
  groupPluginsList->setVisible(true);
}

void GroupsEditorDialog::on_groupNameInput_textChanged(const QString& text) {
  addGroupButton->setDisabled(text.isEmpty());
}

void GroupsEditorDialog::on_addGroupButton_clicked(bool checked) {
  if (groupNameInput->text().isEmpty()) {
    return;
  }

  auto name = groupNameInput->text().toStdString();
  if (!graphWidget->addGroup(name)) {
    QMessageBox::critical(this, "LOOT", translate("Group already exists!"));

    return;
  }

  groupNameInput->clear();
}

void GroupsEditorDialog::on_dialogButtons_accepted() { accept(); }

void GroupsEditorDialog::on_dialogButtons_rejected() { reject(); }
}
