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

#include "gui/qt/settings/settings_window.h"

#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

#include "gui/qt/helpers.h"

namespace loot {
QSize VerticalTabStyle::sizeFromContents(ContentsType type,
                                         const QStyleOption *option,
                                         const QSize &size,
                                         const QWidget *widget) const {
  QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
  if (type == QStyle::CT_TabBarTab) {
    s.transpose();
  }
  return s;
}

void VerticalTabStyle::drawControl(ControlElement element,
                                   const QStyleOption *option,
                                   QPainter *painter,
                                   const QWidget *widget) const {
  if (element == CE_TabBarTabLabel) {
    if (const QStyleOptionTab *tab =
            qstyleoption_cast<const QStyleOptionTab *>(option)) {
      QStyleOptionTab opt(*tab);
      opt.shape = QTabBar::RoundedNorth;
      QProxyStyle::drawControl(element, &opt, painter, widget);
      return;
    }
  }
  QProxyStyle::drawControl(element, option, painter, widget);
}

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent) { setupUi(); }

void SettingsWindow::initialiseInputs(
    const LootSettings &settings,
    const std::vector<std::string> &themes,
    const std::optional<std::string> &currentGameFolder) {
  generalTab->initialiseInputs(settings, themes);

  while (tabs->count() > 2) {
    auto tab = tabs->widget(1);
    tabs->removeTab(1);
    tab->deleteLater();
  }

  for (const auto game : settings.getGameSettings()) {
    auto isCurrentGame = game.FolderName() == currentGameFolder;
    auto gameTab = new GameTab(game, this, isCurrentGame);
    tabs->insertTab(tabs->count() - 1, gameTab, gameTab->getName());

    connect(gameTab,
            &GameTab::gameSettingsDeleted,
            this,
            &SettingsWindow::onGameSettingsDeleted);
  }
}

void SettingsWindow::recordInputValues(LootSettings &settings) {
  generalTab->recordInputValues(settings);

  std::vector<GameSettings> gameSettings;
  // First tab is for general settings, last tab is for adding new games.
  for (int i = 1; i < tabs->count() - 1; i += 1) {
    auto gameTab = qobject_cast<GameTab *>(tabs->widget(i));
    gameSettings.push_back(gameTab->getGameSettings());
  }

  settings.storeGameSettings(gameSettings);
}

void SettingsWindow::setupUi() {
  setWindowModality(Qt::WindowModal);

  auto dialogLayout = new QVBoxLayout(this);
  tabs = new QTabWidget(this);
  tabs->setTabPosition(QTabWidget::West);
  tabs->tabBar()->setStyle(&verticalTabStyle);

  auto buttonBox = new QDialogButtonBox(
      QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
  buttonBox->setObjectName("dialogButtons");

  generalTab = new GeneralTab(this);

  tabs->addTab(generalTab, QString());

  newGameTab = new NewGameTab(this);
  newGameTab->setObjectName("newGameTab");

  tabs->addTab(newGameTab, QString());

  dialogLayout->addWidget(tabs);
  dialogLayout->addWidget(buttonBox);

  translateUi();

  QMetaObject::connectSlotsByName(this);
}

void SettingsWindow::translateUi() {
  setWindowTitle(translate("Settings"));

  tabs->setTabText(0, translate("General"));
  tabs->setTabText(tabs->count() - 1, translate("Add new game..."));
}

void SettingsWindow::on_dialogButtons_accepted() {
  if (!generalTab->areInputValuesValid()) {
    // Switch back to the general tab so that the tooltip that the
    // tab displays is above the relevant input widget. Although
    // the change of tab happens after tooltip display, it's fast
    // enough that there's no visual weirdness.
    tabs->setCurrentIndex(0);
    return;
  }

  accept();
}

void SettingsWindow::on_dialogButtons_rejected() { reject(); }

void SettingsWindow::on_newGameTab_accepted() {
  auto gameTypeText = newGameTab->getGameType().toStdString();
  auto gameType = GameTab::GAME_TYPES_BY_FOLDER.at(gameTypeText);

  auto name = newGameTab->getGameName().toStdString();
  auto lootFolder = newGameTab->getGameFolder();

  // Check that there isn't an existing game with the same folder.
  auto lowercaseLootFolder = lootFolder.toLower();
  for (auto i = 1; i < tabs->count() - 1; i += 1) {
    auto gameTab = qobject_cast<GameTab *>(tabs->widget(i));
    auto otherFolder = gameTab->getLootFolder();

    if (lowercaseLootFolder == otherFolder.toLower()) {
      newGameTab->showFolderConflictError();
      return;
    }
  }

  GameSettings game(gameType, lootFolder.toStdString());
  game.SetName(name);

  auto gameTab = new GameTab(game, this, false);
  auto newTabIndex =
      tabs->insertTab(tabs->count() - 1, gameTab, gameTab->getName());

  connect(gameTab,
          &GameTab::gameSettingsDeleted,
          this,
          &SettingsWindow::onGameSettingsDeleted);

  tabs->setCurrentIndex(newTabIndex);

  newGameTab->reset();
}

void SettingsWindow::onGameSettingsDeleted() {
  auto gameTab = qobject_cast<QWidget *>(sender());
  auto index = tabs->indexOf(gameTab);

  if (index > -1) {
    tabs->removeTab(index);
    gameTab->deleteLater();
  }
}
}
