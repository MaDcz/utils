#include "gui/events/resizeevent.hpp"

namespace mad { namespace utils { namespace gui {

ResizeEvent::ResizeEvent(const QSize& oldSize, const QSize& newSize)
  : oldSize(oldSize),
    newSize(newSize)
{
}

void ResizeEvent::accept(EventProcessor& processor) const
{
  if (auto resizeEventProcessor = dynamic_cast<ResizeEventProcessor*>(&processor))
    resizeEventProcessor->process(*this);
  else
    Event::accept(processor);
}

}}} // namespace mad::utils::gui
