#pragma once

#include "mouseevent.hpp"

namespace mad { namespace utils { namespace gui {

struct MouseWheelEvent : public MouseEvent
{
  /**
   * @brief Determines the scrolling phase.
   */
  enum ScrollPhase
  {
    ScrollBegin,
    ScrollUpdate,
    ScrollEnd
  };

  /** The scroll phase. */
  ScrollPhase scrollPhase;

  /** The number of steps the wheel was rotated. */
  int stepsDelta = 0;

  MouseWheelEvent(ScrollPhase scrollPhase, int stepsDelta, const QPoint& pos);

  void accept(EventProcessor& processor) const override;
};

class MouseWheelEventProcessor : public EventProcessor
{
public:
  using EventProcessor::process;

  virtual void process(const MouseWheelEvent& event) = 0;
};

}}} // namespace mad::utils::gui
