#pragma once

#include "../events/event.hpp"

#include <memory>

class QAction;
class QPainter;

namespace mad { namespace utils { namespace gui {

class Feature
{
protected:
  class State;

public:
  Feature(std::unique_ptr<State>&& initialState);

  virtual ~Feature() {}

  virtual QAction* action();

  State& currentState();

private:
  void updateState(std::unique_ptr<State>&& newState);

private:
  std::unique_ptr<State> m_currentState;
};

class Feature::State : public virtual EventProcessor
{
public:
  explicit State(Feature& feature);

  virtual void paint(QPainter& /*painter*/) const {}

protected:
  Feature& feature();

  void updateState(std::unique_ptr<State>&& newState);

private:
  Feature& m_feature;
};

}}} // namespace mad::utils::gui
