#pragma once

#include "widgetfactory.hpp"

namespace mad { namespace utils { namespace ui {

/**
 * An implementation of the \c WidgetFactory that eases implementation of factories that
 * builds widgets consisting only of a signle part.
 */
class SinglePartWidgetFactory : public WidgetFactory<SinglePartWidgetFactory>
{
public:
  enum Part { TheOnlyOnePart = 1 };

  /**
   * Takes advantage of the \c WidgetFactory::build() method and returns the built widget
   * in a parts collection.
   */
  WidgetPartsCollection<SinglePartWidgetFactory> buildParts(QWidget* parent = nullptr) const override;
};

inline WidgetPartsCollection<SinglePartWidgetFactory> SinglePartWidgetFactory::buildParts(QWidget* parent) const
{
  WidgetPartsCollection<SinglePartWidgetFactory> parts;
  parts.add(TheOnlyOnePart, build(parent));
  return parts;
}

}}} // namespace mad::utils::ui
