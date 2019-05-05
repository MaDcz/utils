#pragma once

#include "../events/event.hpp"

#include <memory>

class QPainter;

namespace mad { namespace utils { namespace gui {

class Feature
{
protected:
  class State : public virtual EventProcessor
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

public:
  explicit Feature(std::unique_ptr<State>&& initialState);

  virtual ~Feature() {}

  State& currentState();

private:
  void updateState(std::unique_ptr<State>&& newState);

private:
  std::unique_ptr<State> m_currentState;
};

}}} // namespace mad::utils::gui
