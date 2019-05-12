#pragma once

#include "event.hpp"

#include <QSize>

namespace mad { namespace utils { namespace gui {

struct ResizeEvent : public Event
{
  QSize oldSize;
  QSize newSize;

  ResizeEvent(const QSize& oldSize, const QSize& newSize);

  void accept(EventProcessor& processor) const override;
};

class ResizeEventProcessor : public virtual EventProcessor
{
public:
  using EventProcessor::process;

  virtual void process(const ResizeEvent& event) = 0;
};

}}} // namespace mad::utils::gui
