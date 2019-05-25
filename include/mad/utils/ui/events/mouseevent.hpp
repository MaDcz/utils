#pragma once

#include "inputevent.hpp"

#include <QPoint>

namespace mad { namespace utils { namespace ui {

class MouseEventProcessor;

struct MouseEvent : public InputEvent
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
    RightButtonReleased,
    Wheel
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

}}} // namespace mad::utils::ui
