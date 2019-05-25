#include "ui/features/feature.hpp"

#include <assert.h>

namespace mad { namespace utils { namespace ui {

//
// Feature::State
//

Feature::State::State(Feature& feature)
  : m_feature(feature)
{
}

Feature& Feature::State::feature()
{
  return m_feature;
}

void Feature::State::updateState(std::unique_ptr<State>&& newState)
{
  m_feature.updateState(std::move(newState));
}

//
// Feature
//

Feature::Feature(std::unique_ptr<State>&& initialState)
  : m_currentState(std::move(initialState))
{
  assert(m_currentState);
}

QAction* Feature::action()
{
  return nullptr;
}

Feature::State& Feature::currentState()
{
  assert(m_currentState);

  return *m_currentState;
}

void Feature::updateState(std::unique_ptr<State>&& newState)
{
  assert(newState);

  m_currentState = std::move(newState);
}

}}} // namespace mad::utils::ui
