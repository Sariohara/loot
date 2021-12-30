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

#ifndef LOOT_GUI_QT_PLUGIN_EDITOR_TABLE_TAB
#define LOOT_GUI_QT_PLUGIN_EDITOR_TABLE_TAB

#include <loot/metadata/file.h>
#include <loot/metadata/location.h>
#include <loot/metadata/message.h>
#include <loot/metadata/plugin_cleaning_data.h>
#include <loot/metadata/tag.h>

#include <QtCore/QAbstractTableModel>
#include <QtGui/QShowEvent>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

#include "gui/state/loot_settings.h"

namespace loot {
class BaseTableTab : public QWidget {
  Q_OBJECT
public:
  BaseTableTab(QWidget* parent);

signals:
  void tableRowCountChanged(bool hasUserMetadata);

protected:
  QAbstractItemModel* getTableModel() const;
  void setTableModel(QAbstractItemModel* model);
  void setColumnHidden(int column, bool hide);
  void setItemDelegateForColumn(int column, QStyledItemDelegate* delegate);
  void acceptDrops();

  virtual bool hasUserMetadata() const = 0;

  void showEvent(QShowEvent* event) override;

private:
  QTableView* tableView;
  QPushButton* addNewRowButton;
  QPushButton* deleteRowButton;
  bool columnsFitted;

  void setupUi();
  void translateUi();

private slots:
  void on_addNewRowButton_clicked(bool checked);
  void on_deleteRowButton_clicked(bool checked);

  void on_selectionModel_selectionChanged(const QItemSelection& selected,
                                          const QItemSelection& deselected);

  void on_model_rowsInserted();
  void on_model_rowsRemoved();
};

class FileTableTab : public BaseTableTab {
  Q_OBJECT
public:
  FileTableTab(QWidget* parent,
               const std::vector<LootSettings::Language>& languages,
               const std::string& language,
               const QStringList& completions);

  void initialiseInputs(const std::vector<File>& nonUserMetadata,
                        const std::vector<File>& userMetadata);

  std::vector<File> getUserMetadata() const;

  bool hasUserMetadata() const override;

private:
  const std::vector<LootSettings::Language>& languages;
  const std::string& language;
  const QStringList& completions;
};

class MessageContentTableWidget : public BaseTableTab {
  Q_OBJECT
public:
  MessageContentTableWidget(
      QWidget* parent,
      const std::vector<LootSettings::Language>& languages);

  void initialiseInputs(const std::vector<MessageContent>& metadata);

  std::vector<MessageContent> getMetadata() const;

  bool hasUserMetadata() const override;

private:
  std::vector<std::pair<QString, QVariant>> languages;
  std::map<std::string, QVariant> languageMap;
};

class MessageTableTab : public BaseTableTab {
  Q_OBJECT
public:
  MessageTableTab(QWidget* parent,
                  const std::vector<LootSettings::Language>& languages,
                  const std::string& language);

  void initialiseInputs(const std::vector<Message>& nonUserMetadata,
                        const std::vector<Message>& userMetadata);

  std::vector<Message> getUserMetadata() const;

  bool hasUserMetadata() const override;

private:
  const std::vector<LootSettings::Language>& languages;
  const std::string& language;
};

class LocationTableTab : public BaseTableTab {
  Q_OBJECT
public:
  using BaseTableTab::BaseTableTab;

  void initialiseInputs(const std::vector<Location>& nonUserMetadata,
                        const std::vector<Location>& userMetadata);

  std::vector<Location> getUserMetadata() const;

  bool hasUserMetadata() const override;
};

class CleaningDataTableTab : public BaseTableTab {
  Q_OBJECT
public:
  CleaningDataTableTab(QWidget* parent,
                       const std::vector<LootSettings::Language>& languages,
                       const std::string& language);

  void initialiseInputs(const std::vector<PluginCleaningData>& nonUserMetadata,
                        const std::vector<PluginCleaningData>& userMetadata);

  void hideCounts(bool hide);

  std::vector<PluginCleaningData> getUserMetadata() const;

  bool hasUserMetadata() const override;

private:
  const std::vector<LootSettings::Language>& languages;
  const std::string& language;
};

class TagTableTab : public BaseTableTab {
  Q_OBJECT
public:
  TagTableTab(QWidget* parent, const QStringList& completions);

  void initialiseInputs(const std::vector<Tag>& nonUserMetadata,
                        const std::vector<Tag>& userMetadata);

  std::vector<Tag> getUserMetadata() const;

  bool hasUserMetadata() const override;

private:
  const QStringList& completions;
};
}

#endif
