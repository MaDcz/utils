#include "gui/features/cursorguidelinesfeature.hpp"

#include "gui/events/mouseevent.hpp"

#include <QPainter>
#include <QPen>
#include <QPoint>

namespace mad { namespace utils { namespace gui {

//
// CursorGuidelinesFeature::State
//

class CursorGuidelinesFeature::State : public Feature::State,
                                       public virtual MouseEventProcessor
{
public:
  State(Feature& feature);

  void paint(QPainter& painter) const override;

  using MouseEventProcessor::process;

  void process(const MouseEvent& event) override;

private:
  bool m_mouseHovering = false;
  QPoint m_cursorPos;
  QPen m_pen;
};

CursorGuidelinesFeature::State::State(Feature& feature)
  : Feature::State(feature)
{
  m_pen = QPen(QColor(Qt::gray));
  m_pen.setStyle(Qt::DotLine);
}

void CursorGuidelinesFeature::State::paint(QPainter& painter) const
{
  if (m_mouseHovering)
  {
    painter.setPen(m_pen);

    auto wndRect = painter.window();

    painter.drawLine(m_cursorPos.x(), 0, m_cursorPos.x(), wndRect.height());
    painter.drawLine(0, m_cursorPos.y(), wndRect.width(), m_cursorPos.y());
  }
}

void CursorGuidelinesFeature::State::process(const MouseEvent& event)
{
  switch (event.type)
  {
  case MouseEvent::Enter:
    m_mouseHovering = true;
    m_cursorPos = event.pos;
    break;

  case MouseEvent::Move:
    m_cursorPos = event.pos;
    break;

  case MouseEvent::Leave:
    m_mouseHovering = false;
    break;

  default:
    break;
  }
}

//
// CursorGuidelinesFeature
//

CursorGuidelinesFeature::CursorGuidelinesFeature()
  : Feature(std::make_unique<State>(*this))
{
}

}}} // namespace mad::utils::gui
