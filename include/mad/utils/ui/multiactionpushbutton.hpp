#pragma once

#include "../flags.hpp" // FIXME This is weird.

#include <QWidget>

class QAction;
class QMenu;
class QPushButton;

namespace mad { namespace utils { namespace ui {

class MultiActionPushButton : public QWidget
{
public:
  MultiActionPushButton(QWidget* parent = nullptr);

  using QWidget::addAction;

  enum class AddActionFlag
  {
    Default = 0,
    SetActionAsDefault = 0x1
  };

  using AddActionFlags = Flags<AddActionFlag>;

  void addAction(QAction& action, AddActionFlags flags = AddActionFlag::Default);

private slots:
  void triggerDefaultAction();

  void defaultActionChanged();

private:
  QMenu* m_actionsMenu = nullptr;
  QAction* m_defaultAction = nullptr;
  QPushButton* m_defaultActionBtn = nullptr;
  QPushButton* m_actionsMenuBtn = nullptr;
};

}}} // namespace mad::utils::ui
