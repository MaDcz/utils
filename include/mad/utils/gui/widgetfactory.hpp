#pragma once

#include "widgetpartscollection.hpp"

class QWidget;

namespace mad { namespace utils { namespace gui {

/**
 * Base for various widget factories.
 */
template <typename WidgetFactoryT>
class WidgetFactory
{
public:
  virtual ~WidgetFactory() {}

  /**
   * @brief Creates a widget consisting of parts accroding to the current factory settings.
   */
  virtual std::unique_ptr<QWidget> build(QWidget* parent = nullptr) const = 0;

  /**
   * @brief Creates and returns widget parts according to the current factory settings.
   *
   * This is useful for custom layouting.
   */
  virtual WidgetPartsCollection<WidgetFactoryT> buildParts(QWidget* parent = nullptr) const = 0;
};

}}} // namespace mad::utils::gui
