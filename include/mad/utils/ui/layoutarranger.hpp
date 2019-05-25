#pragma once

#include "widgetfactory.hpp"

#include <QWidget>

#include <memory>

class QGridLayout;
class QWidget;

namespace mad { namespace utils { namespace ui {

/**
 * @brief Works with a given or created layout and arranges items in it.
 */
class LayoutArranger
{
public:
  class Impl;

public:
  /**
   * Creates an arranger specialized for placing items in the \c QGridLayout layout.
   */
  explicit LayoutArranger(QGridLayout& gridLayout, QWidget* parent = nullptr);

  /**
   * @brief Creates a new arranger specialized for a specific layout type.
   *
   * This automatically creates the new layout and sets it into the widget. It's then used
   * for arranging items and the widget is used as a parent when creating new widgets through
   * factories.
   *
   * @param widget The widget for which to arrange items in the layout.
   * @tparam LayoutT The layout type to initialize for the widget. Note that supported layout
   *                 at any given moment are the ones for which a constructor exists.
   */
  template <typename LayoutT>
  static LayoutArranger create(QWidget& widget);

  virtual ~LayoutArranger();

  /**
   * Places widget parts into the layout.
   */
  void place(std::vector<std::unique_ptr<QWidget>>&& parts) const;

  /**
   * Places a widget into the layout.
   */
  void place(std::unique_ptr<QWidget>&& widget) const;

  /**
   * Builds a widget and places its parts into the layout.
   *
   * @tparam WidgetFactoryT The type of the factory used to build the widget parts.
   */
  template <typename WidgetFactoryT>
  void place(const WidgetFactory<WidgetFactoryT>& widgetFactory) const;

private:
  std::unique_ptr<Impl> m_impl;
  QWidget* m_parent = nullptr;
};

template <typename LayoutT>
inline LayoutArranger LayoutArranger::create(QWidget& widget)
{
  return LayoutArranger(*(new LayoutT(&widget)), &widget);
}

inline void LayoutArranger::place(std::unique_ptr<QWidget>&& widget) const
{
  std::vector<std::unique_ptr<QWidget>> parts;
  parts.push_back(std::move(widget));
  place(std::move(parts));
}

template <typename WidgetFactoryT>
inline void LayoutArranger::place(const WidgetFactory<WidgetFactoryT>& widgetFactory) const
{
  place(widgetFactory.buildParts(m_parent).release());
}

}}} // namespace mad::utils::ui
