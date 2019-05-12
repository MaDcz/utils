#include "gui/features/selectionboxfeature.hpp"

#include "gui/events/mouseevent.hpp"
#include "gui/events/resizeevent.hpp"

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPoint>

namespace mad { namespace utils { namespace gui {

//
// SelectionFeature::State
// SelectionFeature::IdleState
// SelectionFeature::DragState
//

class SelectionBoxFeature::State : public Feature::State,
                                   public ResizeEventProcessor
{
public:
  State(SelectionBoxFeature& feature);

  void paint(QPainter& painter) const override;

  using Feature::State::process;

  void process(const ResizeEvent& event) override;

protected:
  SelectionBoxFeature& selectionBoxFeature();

  const QRect& selectionRectangle() const;

  void setSelectionRectangle(const QRect& rect);

private:
  SelectionBoxFeature& m_selectionBoxFeature;
};

class SelectionBoxFeature::IdleState : public virtual State,
                                       public virtual MouseEventProcessor
{
public:
  explicit IdleState(SelectionBoxFeature& feature);

  IdleState(const QRect& selectionRect, SelectionBoxFeature& feature);

  using State::process;

  void process(const MouseEvent& event) override;
};

class SelectionBoxFeature::DragState : public virtual State,
                                       public virtual MouseEventProcessor
{
public:
  DragState(const QPoint& startPos, SelectionBoxFeature& feature);

  using State::process;

  void process(const MouseEvent& event) override;

private:
  QRect computeSelectionRectangle(const QPoint& endPos);

private:
  QPoint m_startPos;
};

//
// SelectionFeature::State implementation
//

SelectionBoxFeature::State::State(SelectionBoxFeature& feature)
  : Feature::State(feature),
    m_selectionBoxFeature(feature)
{
  m_selectionBoxFeature.m_selectionRect = QRect();
}

void SelectionBoxFeature::State::paint(QPainter& painter) const
{
  if (m_selectionBoxFeature.m_selectionRect.isValid())
  {
    static const auto pen = QPen(QColor(Qt::gray));

    painter.setPen(pen);
    painter.drawRect(m_selectionBoxFeature.m_selectionRect);
  }
}

void SelectionBoxFeature::State::process(const ResizeEvent& event)
{
  const auto& rect = selectionRectangle();

  auto xConvRatio = double(event.newSize.width()) / event.oldSize.width();
  auto yConvRatio = double(event.newSize.height()) / event.oldSize.height();

  setSelectionRectangle(QRect(int(rect.left() * xConvRatio), int(rect.top() * yConvRatio),
          int(rect.width() * xConvRatio), int(rect.height() * yConvRatio)));
}

SelectionBoxFeature& SelectionBoxFeature::State::selectionBoxFeature()
{
  return m_selectionBoxFeature;
}

const QRect& SelectionBoxFeature::State::selectionRectangle() const
{
  return m_selectionBoxFeature.m_selectionRect;
}

void SelectionBoxFeature::State::setSelectionRectangle(const QRect& rect)
{
  m_selectionBoxFeature.m_selectionRect = rect;
}

//
// SelectionFeature::IdleState implementation
//

SelectionBoxFeature::IdleState::IdleState(SelectionBoxFeature& feature)
  : State(feature)
{
}

SelectionBoxFeature::IdleState::IdleState(const QRect& selectionRect, SelectionBoxFeature& feature)
  : IdleState(feature)
{
  setSelectionRectangle(selectionRect);
}

void SelectionBoxFeature::IdleState::process(const MouseEvent& event)
{
  if (event.type == MouseEvent::LeftButtonPressed)
    updateState(std::make_unique<DragState>(event.pos, selectionBoxFeature()));
}

//
// SelectionFeature::DragState implementation
//

SelectionBoxFeature::DragState::DragState(const QPoint& startPos, SelectionBoxFeature& feature)
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
    setSelectionRectangle(computeSelectionRectangle(event.pos));
    break;

  case MouseEvent::LeftButtonPressed:
    updateState(std::make_unique<IdleState>(computeSelectionRectangle(event.pos), selectionBoxFeature()));
    break;

  default:
    break;
  }
}

QRect SelectionBoxFeature::DragState::computeSelectionRectangle(const QPoint& endPos)
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

  return QRect(topLeft, bottomRight);
}

//
// SelectionFeature
//

SelectionBoxFeature::SelectionBoxFeature()
  : Feature(std::make_unique<IdleState>(*this))
{
}

const QRect& SelectionBoxFeature::selectionRectangle() const
{
  return m_selectionRect;
}

}}} // namespace mad::utils::gui
