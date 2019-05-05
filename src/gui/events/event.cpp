#include "gui/events/event.hpp"

namespace mad { namespace utils { namespace gui {

void Event::accept(EventProcessor& processor) const
{
  processor.process(*this);
}

}}} // namespace mad::utils::gui
