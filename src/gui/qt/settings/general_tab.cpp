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

#include "gui/qt/settings/general_tab.h"

#include <QtWidgets/QFormLayout>
#include <QtWidgets/QToolTip>
#include <QtWidgets/QVBoxLayout>

#include "gui/qt/helpers.h"

namespace loot {
GeneralTab::GeneralTab(QWidget* parent) : QWidget(parent, Qt::Dialog) {
  setupUi();
}

void GeneralTab::initialiseInputs(const LootSettings& settings,
                                  const std::vector<std::string>& themes) {
  while (defaultGameComboBox->count() > 1) {
    defaultGameComboBox->removeItem(1);
  }

  for (const auto& game : settings.getGameSettings()) {
    defaultGameComboBox->addItem(
        QString::fromUtf8(game.Name()),
        QVariant(QString::fromUtf8(game.FolderName())));
  }

  auto gameIndex = defaultGameComboBox->findData(
      QVariant(QString::fromUtf8(settings.getGame())));
  defaultGameComboBox->setCurrentIndex(gameIndex);

  while (languageComboBox->count() > 0) {
    languageComboBox->removeItem(0);
  }

  for (const auto& language : settings.getLanguages()) {
    languageComboBox->addItem(QString::fromUtf8(language.name),
                              QVariant(QString::fromUtf8(language.locale)));
  }

  auto languageIndex = languageComboBox->findData(
      QVariant(QString::fromUtf8(settings.getLanguage())));
  languageComboBox->setCurrentIndex(languageIndex);

  while (themeComboBox->count() > 0) {
    themeComboBox->removeItem(0);
  }

  for (const auto& theme : themes) {
    auto qtTheme = QString::fromUtf8(theme);
    themeComboBox->addItem(qtTheme, QVariant(qtTheme));
  }

  auto themeIndex =
      themeComboBox->findData(QVariant(QString::fromUtf8(settings.getTheme())));
  themeComboBox->setCurrentIndex(themeIndex);

  updateMasterlistCheckbox->setChecked(settings.updateMasterlist());
  checkUpdatesCheckbox->setChecked(settings.isLootUpdateCheckEnabled());
  loggingCheckbox->setChecked(settings.isDebugLoggingEnabled());

  preludeUrlInput->setText(
      QString::fromUtf8(settings.getPreludeRepositoryURL()));
  preludeBranchInput->setText(
      QString::fromUtf8(settings.getPreludeRepositoryBranch()));
}

void GeneralTab::recordInputValues(LootSettings& settings) {
  auto defaultGame =
      defaultGameComboBox->currentData().toString().toStdString();
  auto language = languageComboBox->currentData().toString().toStdString();
  auto theme = themeComboBox->currentData().toString().toStdString();
  auto updateMasterlist = updateMasterlistCheckbox->isChecked();
  auto checkForUpdates = checkUpdatesCheckbox->isChecked();
  auto enableDebugLogging = loggingCheckbox->isChecked();
  auto preludeUrl = preludeUrlInput->text().toStdString();
  auto preludeBranch = preludeBranchInput->text().toStdString();

  settings.setDefaultGame(defaultGame);
  settings.setLanguage(language);
  settings.setTheme(theme);
  settings.updateMasterlist(updateMasterlist);
  settings.enableLootUpdateCheck(checkForUpdates);
  settings.enableDebugLogging(enableDebugLogging);
  settings.setPreludeRepositoryURL(preludeUrl);
  settings.setPreludeRepositoryBranch(preludeBranch);
}

bool GeneralTab::areInputValuesValid() const {
  if (preludeUrlInput->text().isEmpty()) {
    QToolTip::showText(preludeUrlInput->mapToGlobal(QPoint(0, 0)),
                       preludeUrlInput->toolTip(),
                       preludeUrlInput);
    return false;
  }

  if (preludeBranchInput->text().isEmpty()) {
    QToolTip::showText(preludeBranchInput->mapToGlobal(QPoint(0, 0)),
                       preludeBranchInput->toolTip(),
                       preludeBranchInput);
    return false;
  }

  return true;
}

void GeneralTab::setupUi() {
  defaultGameLabel = new QLabel(this);
  defaultGameComboBox = new QComboBox(this);
  defaultGameComboBox->addItem(QString(), QVariant(QString("auto")));

  languageLabel = new QLabel(this);
  languageComboBox = new QComboBox(this);

  themeLabel = new QLabel(this);
  themeComboBox = new QComboBox(this);

  updateMasterlistLabel = new QLabel(this);
  updateMasterlistCheckbox = new QCheckBox(this);

  checkUpdatesLabel = new QLabel(this);
  checkUpdatesCheckbox = new QCheckBox(this);

  loggingLabel = new QLabel(this);
  loggingCheckbox = new QCheckBox(this);

  preludeUrlLabel = new QLabel(this);
  preludeUrlInput = new QLineEdit(this);

  preludeBranchLabel = new QLabel(this);
  preludeBranchInput = new QLineEdit(this);

  auto generalLayout = new QFormLayout(this);

  generalLayout->addRow(defaultGameLabel, defaultGameComboBox);
  generalLayout->addRow(languageLabel, languageComboBox);
  generalLayout->addRow(themeLabel, themeComboBox);
  generalLayout->addRow(updateMasterlistLabel, updateMasterlistCheckbox);
  generalLayout->addRow(checkUpdatesLabel, checkUpdatesCheckbox);
  generalLayout->addRow(loggingLabel, loggingCheckbox);
  generalLayout->addRow(preludeUrlLabel, preludeUrlInput);
  generalLayout->addRow(preludeBranchLabel, preludeBranchInput);

  translateUi();
}

void GeneralTab::translateUi() {
  defaultGameLabel->setText(translate("Default Game"));
  languageLabel->setText(translate("Language"));
  themeLabel->setText(translate("Theme"));
  updateMasterlistLabel->setText(translate("Update masterlist before sorting"));
  checkUpdatesLabel->setText(translate("Check for LOOT updates on startup"));
  loggingLabel->setText(translate("Enable debug logging"));
  preludeUrlLabel->setText(translate("Masterlist prelude repository URL"));
  preludeBranchLabel->setText(
      translate("Masterlist prelude repository branch"));

  languageLabel->setToolTip(
      translate("Language changes will be applied after LOOT is restarted."));
  themeLabel->setToolTip(
      translate("Theme changes will be applied after LOOT is restarted."));
  loggingLabel->setToolTip(
      translate("The output is logged to the LOOTDebugLog.txt file."));

  preludeUrlInput->setToolTip(translate("A prelude URL is required."));
  preludeBranchInput->setToolTip(translate("A prelude branch is required."));

  defaultGameComboBox->setItemText(0, translate("Autodetect"));
}
}
