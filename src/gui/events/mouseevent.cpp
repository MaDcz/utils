#include "gui/events/mouseevent.hpp"

namespace mad { namespace utils { namespace gui {

MouseEvent::MouseEvent(Type type, const QPoint& pos)
  : type(type),
    pos(pos)
{
}

void MouseEvent::accept(EventProcessor& processor) const
{
  if (auto mouseEventProcessor = dynamic_cast<MouseEventProcessor*>(&processor))
    mouseEventProcessor->process(*this);
  else
    Event::accept(processor);
}

}}} // namespace mad::utils::gui
