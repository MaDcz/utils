#pragma once

#include "singlepartwidgetfactory.hpp"

#include <QDialogButtonBox>
#include <QString>

#include <initializer_list>
#include <vector>

namespace mad { namespace utils { namespace ui {

/**
 * @brief A widget factory that creates a @c QDialogButtonBox allowing for buttons configuration.
 */
class DialogButtonBoxFactory : public SinglePartWidgetFactory
{
public:
  DialogButtonBoxFactory() {}

  explicit DialogButtonBoxFactory(std::initializer_list<QDialogButtonBox::StandardButton> buttons);

  /**
   * @brief Adds a standard button to the consequently built button boxes.
   */
  void addButton(QDialogButtonBox::StandardButton button, const QString& text = {});

  std::unique_ptr<QWidget> build(QWidget* parent = nullptr) const override;

private:
  struct StandardButtonInfo
  {
    QDialogButtonBox::StandardButton button;
    QString text;

    explicit StandardButtonInfo(QDialogButtonBox::StandardButton button, const QString& text = {});
  };

  std::vector<StandardButtonInfo> m_buttonsInfo;
};

}}} // namespace mad::utils::ui
