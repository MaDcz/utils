#include "ui/events/event.hpp"

namespace mad { namespace utils { namespace ui {

void Event::accept(EventProcessor& processor) const
{
  processor.process(*this);
}

}}} // namespace mad::utils::ui
