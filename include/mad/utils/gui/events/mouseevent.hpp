#pragma once

#include "event.hpp"

#include <QPoint>

namespace mad { namespace utils { namespace gui {

class MouseEventProcessor;

struct MouseEvent : public Event
{
  enum Type
  {
    Move,
    Enter,
    Leave,
    LeftButtonPressed,
    LeftButtonReleased,
    MiddleButtonPressed,
    MiddleButtonReleased,
    RightButtonPressed,
    RightButtonReleased
  };

  Type type;
  QPoint pos;

  MouseEvent(Type type, const QPoint& pos);

  void accept(EventProcessor& processor) const override;
};

class MouseEventProcessor : public virtual EventProcessor
{
public:
  using EventProcessor::process;

  virtual void process(const MouseEvent& event) = 0;
};

}}} // namespace mad::utils::gui
