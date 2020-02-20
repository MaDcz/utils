#include "ui/multiactionpushbutton.hpp"

#include <QAction>
#include <QHBoxLayout>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QStyleOptionButton>

#include <cassert>

namespace mad { namespace utils { namespace ui {

namespace
{

static const QString s_defaultActionButtonDefaultTest("<default>");

class DefaultActionButton : public QPushButton
{
public:
  DefaultActionButton(QWidget* parent = nullptr);

protected:
  void paintEvent(QPaintEvent* paintEvent) override;
};

DefaultActionButton::DefaultActionButton(QWidget* parent)
  : QPushButton(parent)
{
}

void DefaultActionButton::paintEvent(QPaintEvent* paintEvent)
{
  QStyleOptionButton option;
  initStyleOption(&option);

  if (!icon().isNull())
    option.icon = icon();
  else if (!text().isEmpty())
    option.text = text();
  else
    option.text = s_defaultActionButtonDefaultTest;

  QPainter painter(this);
  style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
}

} // namespace

MultiActionPushButton::MultiActionPushButton(QWidget* parent)
  : QWidget(parent)
{
  auto layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  m_defaultActionBtn = new DefaultActionButton();
  m_defaultActionBtn->setDisabled(true);
  m_defaultActionBtn->setContentsMargins(0, 0, 0, 0);
  connect(m_defaultActionBtn, &QAbstractButton::pressed, this, &MultiActionPushButton::triggerDefaultAction);
  layout->addWidget(m_defaultActionBtn);

  m_actionsMenuBtn = new QPushButton();
  m_actionsMenuBtn->setDisabled(true);
  m_actionsMenuBtn->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_actionsMenuBtn);

  m_actionsMenu = new QMenu(this);
  m_actionsMenuBtn->setMenu(m_actionsMenu);
}

void MultiActionPushButton::addAction(QAction& action, AddActionFlags flags)
{
  if (m_actionsMenu->actions().contains(&action))
    return;

  m_actionsMenu->addAction(&action);
  m_actionsMenuBtn->setEnabled(true);

  if (flags.isSet(AddActionFlag::SetActionAsDefault))
  {
    if (m_defaultAction)
      disconnect(m_defaultAction, &QAction::changed, this, &MultiActionPushButton::defaultActionChanged);

    m_defaultAction = &action;
    defaultActionChanged();
    connect(m_defaultAction, &QAction::changed, this, &MultiActionPushButton::defaultActionChanged);
  }
}

void MultiActionPushButton::defaultActionChanged()
{
  assert(m_defaultAction);

  m_defaultActionBtn->setEnabled(m_defaultAction->isEnabled());
  m_defaultActionBtn->setIcon(m_defaultAction->icon());
  m_defaultActionBtn->setText(m_defaultAction->iconText());
  m_defaultActionBtn->setToolTip(m_defaultAction->toolTip());
}

void MultiActionPushButton::triggerDefaultAction()
{
  assert(m_defaultAction);
  assert(m_defaultAction->isEnabled());

  m_defaultAction->trigger();
}

}}} // namespace mad::utils::ui
