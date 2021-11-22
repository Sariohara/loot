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

#include "gui/qt/plugin_card.h"

#include <QtCore/QAbstractProxyModel>
#include <QtCore/QStringBuilder>
#include <QtCore/QStringList>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

#include "gui/qt/counters.h"
#include "gui/qt/helpers.h"
#include "gui/qt/icons.h"
#include "gui/qt/plugin_item_model.h"

namespace loot {
QString getTagsText(const std::vector<std::string> tags, bool hideTags) {
  if (hideTags) {
    return "";
  }

  QStringList tagsList;
  for (const auto& tag : tags) {
    tagsList.append(QString::fromUtf8(tag));
  }

  if (tagsList.isEmpty()) {
    return "";
  }

  return tagsList.join(", ");
}

std::vector<SimpleMessage> filterMessages(
    const std::vector<SimpleMessage>& messages,
    const CardContentFiltersState& filters) {
  std::vector<SimpleMessage> filteredMessages;

  if (!filters.hideAllPluginMessages) {
    for (const auto& message : messages) {
      if (message.type != MessageType::say || !filters.hideNotes) {
        filteredMessages.push_back(message);
      }
    }
  }

  return filteredMessages;
}

std::vector<std::string> getMessageTexts(
    const std::vector<SimpleMessage>& messages) {
  std::vector<std::string> texts;
  for (const auto& message : messages) {
    texts.push_back(message.text);
  }

  return texts;
}

SizeHintCacheKey getSizeHintCacheKey(const QModelIndex& index) {
  if (index.row() == 0) {
    auto generalInfo = index.data(RawDataRole).value<GeneralInformation>();

    return SizeHintCacheKey(QString(),
                            QString(),
                            QString(),
                            getMessageTexts(generalInfo.generalMessages),
                            true);
  } else {
    auto pluginItem = index.data(RawDataRole).value<PluginItem>();
    auto filters =
        index.data(CardContentFiltersRole).value<CardContentFiltersState>();

    return SizeHintCacheKey(
        getTagsText(pluginItem.currentTags, filters.hideBashTags),
        getTagsText(pluginItem.addTags, filters.hideBashTags),
        getTagsText(pluginItem.removeTags, filters.hideBashTags),
        getMessageTexts(filterMessages(pluginItem.messages, filters)),
        false);
  }
}

PluginCard::PluginCard(QWidget* parent) : QFrame(parent) { setupUi(); }

void PluginCard::setContent(const PluginItem& plugin,
                            const CardContentFiltersState& filters) {
  nameLabel->setText(QString::fromUtf8(plugin.name));

  if (plugin.crc.has_value() && !filters.hideCRCs) {
    auto crc = crcToString(plugin.crc.value());
    crcLabel->setText(QString::fromUtf8(crc));
  } else {
    crcLabel->clear();
  }

  if (plugin.version.has_value() && !filters.hideVersionNumbers) {
    versionLabel->setText(QString::fromUtf8(plugin.version.value()));
  } else {
    versionLabel->clear();
  }

  isActiveLabel->setVisible(plugin.isActive);
  masterFileLabel->setVisible(plugin.isMaster);
  lightPluginLabel->setVisible(plugin.isLightPlugin);
  emptyPluginLabel->setVisible(plugin.isEmpty);
  loadsArchiveLabel->setVisible(plugin.loadsArchive);
  isCleanLabel->setVisible(plugin.cleaningUtility.has_value());
  hasUserEditsLabel->setVisible(plugin.hasUserMetadata);

  auto tagsText = getTagsText(plugin.currentTags, filters.hideBashTags);
  currentTagsLabel->setText(tagsText);
  currentTagsLabel->setVisible(!tagsText.isEmpty());
  currentTagsHeaderLabel->setVisible(!tagsText.isEmpty());

  tagsText = getTagsText(plugin.addTags, filters.hideBashTags);
  addTagsLabel->setText(tagsText);
  addTagsLabel->setVisible(!tagsText.isEmpty());
  addTagsHeaderLabel->setVisible(!tagsText.isEmpty());

  tagsText = getTagsText(plugin.removeTags, filters.hideBashTags);
  removeTagsLabel->setText(tagsText);
  removeTagsLabel->setVisible(!tagsText.isEmpty());
  removeTagsHeaderLabel->setVisible(!tagsText.isEmpty());

  auto showBashTags = !currentTagsLabel->text().isEmpty() ||
                      !addTagsLabel->text().isEmpty() ||
                      !removeTagsLabel->text().isEmpty();

  tagsGroupBox->setVisible(showBashTags);

  auto messages = filterMessages(plugin.messages, filters);
  messagesWidget->setMessages(messages);
  messagesWidget->setVisible(!messages.empty());

  if (plugin.cleaningUtility.has_value()) {
    auto cleanText =
        (boost::format(boost::locale::translate("Verified clean by %s")) %
         plugin.cleaningUtility.value())
            .str();
    isCleanLabel->setToolTip(QString::fromUtf8(cleanText));
  } else {
    isCleanLabel->setToolTip(QString());
  }

  tagsGroupBox->layout()->activate();
  layout()->activate();
}

void PluginCard::setSearchResult(bool isSearchResult,
                                 bool isCurrentSearchResult) {
  setProperty("isSearchResult", isSearchResult);
  setProperty("isCurrentSearchResult", isCurrentSearchResult);

  // Trigger styling changes.
  style()->unpolish(this);
  style()->polish(this);
}

void PluginCard::setupUi() {
  nameLabel = new QLabel(this);
  crcLabel = new QLabel(this);
  versionLabel = new QLabel(this);

  isActiveLabel = new QLabel(this);
  isActiveLabel->setVisible(false);
  masterFileLabel = new QLabel(this);
  masterFileLabel->setVisible(false);
  lightPluginLabel = new QLabel(this);
  lightPluginLabel->setVisible(false);
  emptyPluginLabel = new QLabel(this);
  emptyPluginLabel->setVisible(false);
  loadsArchiveLabel = new QLabel(this);
  loadsArchiveLabel->setVisible(false);
  isCleanLabel = new QLabel(this);
  isCleanLabel->setVisible(false);
  hasUserEditsLabel = new QLabel(this);
  hasUserEditsLabel->setVisible(false);

  tagsGroupBox = new QGroupBox(this);
  tagsGroupBox->setVisible(false);

  currentTagsHeaderLabel = new QLabel(this);
  currentTagsHeaderLabel->setVisible(false);

  currentTagsLabel = new QLabel(this);
  currentTagsLabel->setWordWrap(true);
  currentTagsLabel->setSizePolicy(QSizePolicy::Preferred,
                                  QSizePolicy::Preferred);
  currentTagsLabel->setVisible(false);

  addTagsHeaderLabel = new QLabel(this);
  addTagsHeaderLabel->setVisible(false);

  addTagsLabel = new QLabel(this);
  addTagsLabel->setWordWrap(true);
  addTagsLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  addTagsLabel->setVisible(false);

  removeTagsHeaderLabel = new QLabel(this);
  removeTagsHeaderLabel->setVisible(false);

  removeTagsLabel = new QLabel(this);
  removeTagsLabel->setWordWrap(true);
  removeTagsLabel->setSizePolicy(QSizePolicy::Preferred,
                                 QSizePolicy::Preferred);
  removeTagsLabel->setVisible(false);

  messagesWidget = new MessagesWidget(this);
  messagesWidget->setVisible(false);

  isActiveLabel->setPixmap(IconFactory::getIsActiveIcon());
  masterFileLabel->setPixmap(IconFactory::getMasterFileIcon());
  lightPluginLabel->setPixmap(IconFactory::getLightPluginIcon());
  emptyPluginLabel->setPixmap(IconFactory::getEmptyPluginIcon());
  loadsArchiveLabel->setPixmap(IconFactory::getLoadsArchiveIcon());
  isCleanLabel->setPixmap(IconFactory::getIsCleanIcon());
  hasUserEditsLabel->setPixmap(IconFactory::getHasUserMetadataIcon());

  auto layout = new QVBoxLayout(this);
  layout->setSizeConstraint(QLayout::SetMinimumSize);
  auto headerLayout = new QHBoxLayout(this);
  headerLayout->setSizeConstraint(QLayout::SetMinimumSize);

  auto tagsLayout = new QGridLayout(tagsGroupBox);
  tagsLayout->setSizeConstraint(QLayout::SetMinimumSize);
  tagsLayout->setColumnStretch(1, 1);

  headerLayout->addWidget(nameLabel);
  headerLayout->addWidget(crcLabel);
  headerLayout->addWidget(versionLabel);
  headerLayout->addStretch();
  headerLayout->addWidget(isActiveLabel);
  headerLayout->addWidget(masterFileLabel);
  headerLayout->addWidget(lightPluginLabel);
  headerLayout->addWidget(emptyPluginLabel);
  headerLayout->addWidget(loadsArchiveLabel);
  headerLayout->addWidget(isCleanLabel);
  headerLayout->addWidget(hasUserEditsLabel);

  tagsLayout->addWidget(currentTagsHeaderLabel, 0, 0, Qt::AlignTop);
  tagsLayout->addWidget(currentTagsLabel, 0, 1, Qt::AlignTop);
  tagsLayout->addWidget(addTagsHeaderLabel, 1, 0, Qt::AlignTop);
  tagsLayout->addWidget(addTagsLabel, 1, 1, Qt::AlignTop);
  tagsLayout->addWidget(removeTagsHeaderLabel, 2, 0, Qt::AlignTop);
  tagsLayout->addWidget(removeTagsLabel, 2, 1, Qt::AlignTop);

  layout->addLayout(headerLayout);
  layout->addWidget(messagesWidget);
  layout->addWidget(tagsGroupBox);

  translateUi();
}

void PluginCard::translateUi() {
  isActiveLabel->setToolTip(translate("Active Plugin"));
  masterFileLabel->setToolTip(translate("Master File"));
  lightPluginLabel->setToolTip(translate("Light Plugin"));
  emptyPluginLabel->setToolTip(translate("Empty Plugin"));
  loadsArchiveLabel->setToolTip(translate("Loads Archive"));
  hasUserEditsLabel->setToolTip(translate("Has User Metadata"));

  tagsGroupBox->setTitle(translate("Bash Tags"));
  currentTagsHeaderLabel->setText(translate("Current"));
  addTagsHeaderLabel->setText(translate("Add"));
  removeTagsHeaderLabel->setText(translate("Remove"));
}

PluginCardDelegate::PluginCardDelegate(QListView* parent) :
    QStyledItemDelegate(parent) {}

PluginCardDelegate::~PluginCardDelegate() {
  for (auto entry : sizeHintWidgetCache) {
    entry.second->deleteLater();
  }
}

void PluginCardDelegate::paint(QPainter* painter,
                               const QStyleOptionViewItem& option,
                               const QModelIndex& index) const {
  return QStyledItemDelegate::paint(painter, option, index);
}

QSize PluginCardDelegate::sizeHint(const QStyleOptionViewItem& option,
                                   const QModelIndex& index) const {
  if (!index.isValid()) {
    return QStyledItemDelegate::sizeHint(option, index);
  }

  auto styleOption = QStyleOptionViewItem(option);
  initStyleOption(&styleOption, index);

  // Cache size hints by SizeHintCacheKey because that contains all the data
  // that the card size could depend on, aside from the available width, and
  // so it means that different plugins with cards of the same size can share
  // cached size data.
  // It's a bit inefficient to do all the message and tag transformations here
  // and again when setting the actual card content (if that happens), but in
  // practice the cost is negligible.
  auto cacheKey = getSizeHintCacheKey(index);

  auto it = sizeHintCache.find(cacheKey);
  if (it != sizeHintCache.end()) {
    // Found a cached size, check if its width matches the current available
    // width.
    auto widthChanged = it->second.width() != styleOption.rect.width();
    if (!widthChanged) {
      // The cached size is valid, return it.
      return it->second;
    }
  }

  // Cache widgets by cache key because multiple plugins will have cards
  // of the same dimensions (e.g. because they have no messages or tags),
  // and calculating widget layouts is relatively expensive so it's good
  // to be able to do that once for a given set of data that may affect
  // plugin size.
  QWidget* parentWidget = qobject_cast<QListView*>(parent());
  auto [widget, isNewWidget] = getSizeHintWidget(parentWidget, cacheKey);

  // If the widget is new, initialise it with the relevant data so that a
  // correct size hint can be calculated. If the widget is not new then it
  // already has appropriate data and a size just needs to be calculated
  // for the current available width.
  if (isNewWidget) {
    if (index.row() == 0) {
      setGeneralInfoFrameContent(qobject_cast<GeneralInfoFrame*>(widget),
                                 index);
    } else {
      setPluginCardContent(qobject_cast<PluginCard*>(widget), index);
    }
  }

  auto sizeHint = calculateSize(widget, styleOption);

  sizeHintCache.insert_or_assign(cacheKey, sizeHint);

  return sizeHint;
}

QWidget* PluginCardDelegate::createEditor(QWidget* parent,
                                          const QStyleOptionViewItem& option,
                                          const QModelIndex& index) const {
  if (!index.isValid()) {
    return nullptr;
  }

  auto styleOption = QStyleOptionViewItem(option);
  initStyleOption(&styleOption, index);

  if (index.row() == 0) {
    return new GeneralInfoFrame(parent);
  } else {
    return new PluginCard(parent);
  }
}

void PluginCardDelegate::setEditorData(QWidget* editor,
                                       const QModelIndex& index) const {
  if (!index.isValid()) {
    return;
  }

  if (index.row() == 0) {
    setGeneralInfoFrameContent(qobject_cast<GeneralInfoFrame*>(editor), index);
  } else {
    auto searchResultData =
        index.data(SearchResultRole).value<SearchResultData>();

    auto card = qobject_cast<PluginCard*>(editor);
    setPluginCardContent(card, index);

    card->setSearchResult(searchResultData.isResult,
                          searchResultData.isCurrentResult);
  }
}

void PluginCardDelegate::setModelData(QWidget* editor,
                                      QAbstractItemModel* model,
                                      const QModelIndex& index) const {
  // Do nothing, it's not actually an editor.
}

std::pair<QWidget*, bool> PluginCardDelegate::getSizeHintWidget(
    QWidget* parent,
    SizeHintCacheKey cacheKey) const {
  auto it = sizeHintWidgetCache.find(cacheKey);
  if (it != sizeHintWidgetCache.end()) {
    return std::make_pair(it->second, false);
  }

  QWidget* widget = nullptr;
  if (std::get<4>(cacheKey)) {
    widget = new GeneralInfoFrame(parent);
  } else {
    widget = new PluginCard(parent);
  }

  sizeHintWidgetCache.emplace(cacheKey, widget);

  return std::make_pair(widget, true);
}

GeneralInfoFrame* PluginCardDelegate::setGeneralInfoFrameContent(
    GeneralInfoFrame* generalInfoFrame,
    const QModelIndex& index) {
  auto generalInfo = index.data(RawDataRole).value<GeneralInformation>();
  auto counters = index.data(CountersRole).value<GeneralInformationCounters>();

  generalInfoFrame->setMasterlistInfo(generalInfo.masterlistRevision);
  generalInfoFrame->setPreludeInfo(generalInfo.preludeRevision);
  generalInfoFrame->setMessageCounts(
      counters.warnings, counters.errors, counters.totalMessages);
  generalInfoFrame->setPluginCounts(
      counters.active, counters.dirty, counters.totalPlugins);
  generalInfoFrame->setGeneralMessages(generalInfo.generalMessages);

  return generalInfoFrame;
}

void PluginCardDelegate::setPluginCardContent(PluginCard* card,
                                              const QModelIndex& index) {
  auto pluginItem = index.data(RawDataRole).value<PluginItem>();
  auto filters =
      index.data(CardContentFiltersRole).value<CardContentFiltersState>();
  auto searchResultData =
      index.data(SearchResultRole).value<SearchResultData>();

  card->setContent(pluginItem, filters);
}

QSize PluginCardDelegate::calculateSize(QWidget* widget,
                                        const QStyleOptionViewItem& option) {
  return calculateSize(widget, option.rect.width());
}

QSize PluginCardDelegate::calculateSize(QWidget* widget, int availableWidth) {
  auto minLayoutWidth = widget->layout()->minimumSize().width();

  auto width =
      availableWidth > minLayoutWidth ? availableWidth : minLayoutWidth;
  auto height = widget->hasHeightForWidth()
                    ? widget->layout()->minimumHeightForWidth(width)
                    : widget->minimumHeight();

  return QSize(width, height);
}
}
