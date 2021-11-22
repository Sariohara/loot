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

#include "gui/qt/groups_editor/edge.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtMath>
#include <QtWidgets/QGraphicsScene>

#include "gui/qt/groups_editor/node.h"

namespace loot {
Edge::Edge(Node *sourceNode, Node *destNode, bool isUserMetadata) :
    source(sourceNode), dest(destNode), isUserMetadata_(isUserMetadata) {
  setAcceptedMouseButtons(Qt::RightButton);
  source->addEdge(this);
  dest->addEdge(this);
  adjust();
}

Node *Edge::sourceNode() const { return source; }

Node *Edge::destNode() const { return dest; }

bool Edge::isUserMetadata() const { return isUserMetadata_; }

void Edge::adjust() {
  if (!source || !dest) {
    return;
  }

  QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
  auto length = line.length();

  prepareGeometryChange();

  if (length > qreal(20.0)) {
    QPointF edgeOffset((line.dx() * Node::RADIUS) / length,
                       (line.dy() * Node::RADIUS) / length);
    sourcePoint = line.p1() + edgeOffset;
    destPoint = line.p2() - edgeOffset;
  } else {
    sourcePoint = line.p1();
    destPoint = line.p1();
  }
}

QRectF Edge::boundingRect() const {
  if (!source || !dest) {
    return QRectF();
  }

  auto padding = (LINE_WIDTH + ARROW_HYPOTENUSE) / 2.0;

  return QRectF(sourcePoint,
                QSizeF(destPoint.x() - sourcePoint.x(),
                       destPoint.y() - sourcePoint.y()))
      .normalized()
      .adjusted(-padding, -padding, padding, padding);
}

void Edge::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *,
                 QWidget *) {
  if (!source || !dest) {
    return;
  }

  auto colorGroup = isUserMetadata_ ? QPalette::Active : QPalette::Disabled;
  auto color = QGuiApplication::palette().color(colorGroup, QPalette::Text);

  auto polygon = createLineWithArrow(sourcePoint, destPoint);

  if (polygon.isEmpty()) {
    return;
  }

  painter->setPen(
      QPen(color, LINE_WIDTH, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->setBrush(color);
  painter->drawPolygon(polygon);
}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  if (!isUserMetadata_ || event->button() != Qt::RightButton) {
    event->ignore();
    return;
  }

  // Remove this edge: remove it from its source and dest nodes, then remove
  // it from the scene, then delete the object.
  source->removeEdge(this);
  dest->removeEdge(this);
  scene()->removeItem(this);
  delete this;
  // DO NOT use "this" past this line.
}

QPolygonF createLineWithArrow(QPointF startPos, QPointF endPos) {
  QLineF line(startPos, endPos);

  if (qFuzzyCompare(line.length(), qreal(0.0))) {
    return QPolygonF();
  }

  auto angleOfLine = std::atan2(-line.dy(), line.dx());
  auto angleOfArrowEdge1 = angleOfLine - M_PI / 3;
  auto angleOfArrowEdge2 = angleOfArrowEdge1 - M_PI / 3;

  auto destArrowP1 =
      endPos + QPointF(sin(angleOfArrowEdge1) * ARROW_HYPOTENUSE,
                       cos(angleOfArrowEdge1) * ARROW_HYPOTENUSE);
  auto destArrowP2 =
      endPos + QPointF(sin(angleOfArrowEdge2) * ARROW_HYPOTENUSE,
                       cos(angleOfArrowEdge2) * ARROW_HYPOTENUSE);

  return QPolygonF() << line.p1() << line.p2() << destArrowP1 << destArrowP2
                     << line.p2();
}
}
