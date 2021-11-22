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

#include "gui/qt/messages_widget.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QStyle>
#include <QtWidgets/QVBoxLayout>

namespace loot {
static constexpr const char* MESSAGE_TYPE_PROPERTY = "messageType";

QString getPropertyValue(MessageType messageType) {
  switch (messageType) {
    case MessageType::warn:
      return "warn";
    case MessageType::error:
      return "error";
    default:
      return "say";
  }
}

QLabel* getMessageLabel(const SimpleMessage& message) {
  auto label = new QLabel();
  label->setTextFormat(Qt::TextFormat::MarkdownText);

  label->setWordWrap(true);
  label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  label->setOpenExternalLinks(true);

  auto leftMargin = label->style()->pixelMetric(QStyle::PM_LayoutLeftMargin);
  auto topMargin = label->style()->pixelMetric(QStyle::PM_LayoutTopMargin);
  auto rightMargin = label->style()->pixelMetric(QStyle::PM_LayoutRightMargin);
  auto bottomMargin =
      label->style()->pixelMetric(QStyle::PM_LayoutBottomMargin);

  label->setContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);

  label->setProperty(MESSAGE_TYPE_PROPERTY, getPropertyValue(message.type));
  label->setText(QString::fromUtf8(message.text));

  return label;
}

MessagesWidget::MessagesWidget(QWidget* parent) : QWidget(parent) { setupUi(); }

bool MessagesWidget::willChangeContent(
    const std::vector<SimpleMessage>& messages) const {
  std::vector<std::pair<QVariant, std::string>> currentMessages;

  if (messages.size() != layout()->count()) {
    return true;
  }

  for (int i = 0; i < layout()->count(); i += 1) {
    auto label = qobject_cast<QLabel*>(layout()->itemAt(i)->widget());
    auto messageType = label->property(MESSAGE_TYPE_PROPERTY);
    auto text = label->text().toStdString();
    currentMessages.push_back(std::make_pair(messageType, text));
  }

  std::vector<std::pair<QVariant, std::string>> newMessages;
  for (const auto& message : messages) {
    auto messageType = getPropertyValue(message.type);
    newMessages.push_back(std::make_pair(messageType, message.text));
  }

  return currentMessages != newMessages;
}

void MessagesWidget::setMessages(const std::vector<SimpleMessage>& messages) {
  if (!willChangeContent(messages)) {
    // Avoid expensive layout changes.
    return;
  }

  QLayoutItem* child;
  while ((child = layout()->takeAt(0)) != nullptr) {
    delete child->widget();
    delete child;
  }

  for (const auto& message : messages) {
    auto label = getMessageLabel(message);
    layout()->addWidget(label);
  }

  layout()->activate();
}

void MessagesWidget::setupUi() {
  auto layout = new QVBoxLayout(this);
  layout->setSizeConstraint(QLayout::SetMinimumSize);

  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
}

}
