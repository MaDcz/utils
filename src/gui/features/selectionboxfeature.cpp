#include "gui/features/selectionboxfeature.hpp"

#include "gui/events/mouseevent.hpp"

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QRect>

namespace mad { namespace utils { namespace gui {

//
// SelectionFeature::State
// SelectionFeature::IdleState
// SelectionFeature::DragState
//

class SelectionBoxFeature::State : public Feature::State
{
public:
  State(Feature& feature);

  void paint(QPainter& painter) const override;

protected:
  const QRect& selectionRectangle() const;

  void setSelectionRectangle(const QRect& rect);

private:
  QRect m_selectionRect;
};

class SelectionBoxFeature::IdleState : public virtual State,
                                       public virtual MouseEventProcessor
{
public:
  explicit IdleState(Feature& feature);

  IdleState(const QRect& selectionRect, Feature& feature);

  using State::process;

  void process(const MouseEvent& event) override;
};

class SelectionBoxFeature::DragState : public virtual State,
                                       public virtual MouseEventProcessor
{
public:
  DragState(const QPoint& startPos, Feature& feature);

  using State::process;

  void process(const MouseEvent& event) override;

private:
  void updateSelectionRectangle(const QPoint& endPos);

private:
  QPoint m_startPos;
};

//
// SelectionFeature::State implementation
//

SelectionBoxFeature::State::State(Feature& feature)
  : Feature::State(feature)
{
  assert(!m_selectionRect.isValid());
}

void SelectionBoxFeature::State::paint(QPainter& painter) const
{
  if (m_selectionRect.isValid())
  {
    static const auto pen = QPen(QColor(Qt::gray));

    painter.setPen(pen);
    painter.drawRect(m_selectionRect);
  }
}

const QRect& SelectionBoxFeature::State::selectionRectangle() const
{
  return m_selectionRect;
}

void SelectionBoxFeature::State::setSelectionRectangle(const QRect& rect)
{
  m_selectionRect = rect;
}

//
// SelectionFeature::IdleState implementation
//

SelectionBoxFeature::IdleState::IdleState(Feature& feature)
  : State(feature)
{
}

SelectionBoxFeature::IdleState::IdleState(const QRect& selectionRect, Feature& feature)
  : IdleState(feature)
{
  setSelectionRectangle(selectionRect);
}

void SelectionBoxFeature::IdleState::process(const MouseEvent& event)
{
  if (event.type == MouseEvent::LeftButtonPressed)
    updateState(std::make_unique<DragState>(event.pos, feature()));
}

//
// SelectionFeature::DragState implementation
//

SelectionBoxFeature::DragState::DragState(const QPoint& startPos, Feature& feature)
  : State(feature),
    m_startPos(startPos)
{
  setSelectionRectangle(QRect(startPos, startPos));
}

void SelectionBoxFeature::DragState::process(const MouseEvent& event)
{
  switch (event.type)
  {
  case MouseEvent::Move:
    updateSelectionRectangle(event.pos);
    break;

  case MouseEvent::LeftButtonPressed:
    updateSelectionRectangle(event.pos);
    updateState(std::make_unique<IdleState>(selectionRectangle(), feature()));
    break;

  default:
    break;
  }
}

void SelectionBoxFeature::DragState::updateSelectionRectangle(const QPoint& endPos)
{
  QPoint topLeft, bottomRight;

  if (m_startPos.x() <= endPos.x())
  {
    topLeft.setX(m_startPos.x());
    bottomRight.setX(endPos.x());
  }
  else
  {
    topLeft.setX(endPos.x());
    bottomRight.setX(m_startPos.x());
  }

  if (m_startPos.y() <= endPos.y())
  {
    topLeft.setY(m_startPos.y());
    bottomRight.setY(endPos.y());
  }
  else
  {
    topLeft.setY(endPos.y());
    bottomRight.setY(m_startPos.y());
  }

  setSelectionRectangle(QRect(topLeft, bottomRight));
}

//
// SelectionFeature
//

SelectionBoxFeature::SelectionBoxFeature()
  : Feature(std::make_unique<IdleState>(*this))
{
}

}}} // namespace mad::utils::gui
