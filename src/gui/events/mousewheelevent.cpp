#include "gui/events/mousewheelevent.hpp"

namespace mad { namespace utils { namespace gui {

//
// MouseWheelEvent
//

MouseWheelEvent::MouseWheelEvent(ScrollPhase scrollPhase, int stepsDelta, const QPoint& pos)
  : MouseEvent(Wheel, pos),
    scrollPhase(scrollPhase),
    stepsDelta(stepsDelta)
{
}

void MouseWheelEvent::accept(EventProcessor& processor) const
{
  if (auto mouseWheelEventProcessor = dynamic_cast<MouseWheelEventProcessor*>(&processor))
    mouseWheelEventProcessor->process(*this);
  else
    MouseEvent::accept(processor);
}

}}} // namespace mad::utils::gui
