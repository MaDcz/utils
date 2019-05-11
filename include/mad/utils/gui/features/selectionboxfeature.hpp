#pragma once

#include "feature.hpp"

#include <QRect>

namespace mad { namespace utils { namespace gui {

class SelectionBoxFeature : public Feature
{
private:
  class State;
  class IdleState;
  class DragState;

public:
  SelectionBoxFeature();

  const QRect& selectionRectangle() const;

private:
  QRect m_selectionRect;
};

}}} // namespace mad::utils::gui
