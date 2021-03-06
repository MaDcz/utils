#pragma once

namespace mad { namespace utils { namespace ui {

class EventProcessor;

struct Event
{
  virtual ~Event() {}

  virtual void accept(EventProcessor& processor) const;
};

class EventProcessor
{
public:
  virtual ~EventProcessor() {}

  virtual void process(const Event&) {}
};

}}} // namespace mad::utils::ui
