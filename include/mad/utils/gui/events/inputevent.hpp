#pragma once

#include "event.hpp"

namespace mad { namespace utils { namespace gui {

struct InputEvent : public Event
{
  enum Modifier
  {
    NoModifier    = 0,
    CtrlModifier  = 1<<0,
    ShiftModifier = 1<<1,
    AltModifier   = 1<<2
  };

  Modifier modifiers = NoModifier;
};

}}} // namespace mad::utils::gui
