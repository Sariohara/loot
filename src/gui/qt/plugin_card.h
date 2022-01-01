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

#ifndef LOOT_GUI_QT_PLUGIN_CARD
#define LOOT_GUI_QT_PLUGIN_CARD

#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

#include "gui/qt/filters_states.h"
#include "gui/qt/general_info_frame.h"
#include "gui/plugin_item.h"

namespace loot {
class PluginCard : public QFrame {
  Q_OBJECT
public:
  PluginCard(QWidget* parent);

  void setContent(const PluginItem& plugin,
                  const CardContentFiltersState& filters);

  void setSearchResult(bool isSearchResult, bool isCurrentSearchResult);

private:
  QLabel* nameLabel;
  QLabel* crcLabel;
  QLabel* versionLabel;
  QLabel* isActiveLabel;
  QLabel* masterFileLabel;
  QLabel* lightPluginLabel;
  QLabel* emptyPluginLabel;
  QLabel* loadsArchiveLabel;
  QLabel* isCleanLabel;
  QLabel* hasUserEditsLabel;
  QLabel* currentTagsHeaderLabel;
  QLabel* currentTagsLabel;
  QLabel* addTagsHeaderLabel;
  QLabel* addTagsLabel;
  QLabel* removeTagsHeaderLabel;
  QLabel* removeTagsLabel;
  QGroupBox* tagsGroupBox;
  MessagesWidget* messagesWidget;

  void setupUi();

  void translateUi();
};

// current, add and remove bash tags, messages, and whether this is the general
// info card or not.
typedef std::tuple<QString, QString, QString, std::vector<std::string>, bool>
    SizeHintCacheKey;

class PluginCardDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  PluginCardDelegate(QListView* parent);
  ~PluginCardDelegate();

  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;

  QSize sizeHint(const QStyleOptionViewItem& option,
                 const QModelIndex& index) const override;

  QWidget* createEditor(QWidget* parent,
                        const QStyleOptionViewItem& option,
                        const QModelIndex& index) const override;

  void setEditorData(QWidget* editor, const QModelIndex& index) const override;

  void setModelData(QWidget* editor,
                    QAbstractItemModel* model,
                    const QModelIndex& index) const override;

private:
  mutable std::map<SizeHintCacheKey, QSize> sizeHintCache;
  mutable std::map<SizeHintCacheKey, QWidget*> sizeHintWidgetCache;

  std::pair<QWidget*, bool> getSizeHintWidget(QWidget* parent,
                                              SizeHintCacheKey cacheKey) const;

  static GeneralInfoFrame* setGeneralInfoFrameContent(
      GeneralInfoFrame* generalInfoFrame,
      const QModelIndex& index);
  static void setPluginCardContent(PluginCard* card, const QModelIndex& index);

  static QSize calculateSize(QWidget* widget,
                             const QStyleOptionViewItem& option);
  static QSize calculateSize(QWidget* widget, int availableWidth);
};
}

#endif
