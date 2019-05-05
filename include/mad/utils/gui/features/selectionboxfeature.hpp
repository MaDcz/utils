#pragma once

#include "feature.hpp"

namespace mad { namespace utils { namespace gui {

class SelectionBoxFeature : public Feature
{
private:
  class State;
  class IdleState;
  class DragState;

public:
  SelectionBoxFeature();
};

}}} // namespace mad::utils::gui
