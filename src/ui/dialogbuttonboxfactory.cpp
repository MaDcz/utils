#include "ui/dialogbuttonboxfactory.hpp"

#include <QDialog>
#include <QPushButton>

namespace mad { namespace utils { namespace ui {

DialogButtonBoxFactory::DialogButtonBoxFactory(
    std::initializer_list<QDialogButtonBox::StandardButton> buttons)
{
  for (auto button : buttons)
    m_buttonsInfo.emplace_back(button);
}

void DialogButtonBoxFactory::addButton(QDialogButtonBox::StandardButton button, const QString& text)
{
  m_buttonsInfo.emplace_back(button, text);
}

std::unique_ptr<QWidget> DialogButtonBoxFactory::build(QWidget* parent) const
{
  auto buttonBox = std::make_unique<QDialogButtonBox>();
  auto dialog = qobject_cast<QDialog*>(parent);

  for (const auto& buttonInfo : m_buttonsInfo)
  {
    auto button = buttonBox->addButton(buttonInfo.button);
    if (!buttonInfo.text.isEmpty())
      button->setText(buttonInfo.text);

    if (dialog)
    {
      switch (buttonBox->buttonRole(button))
      {
      case QDialogButtonBox::AcceptRole:
        dialog->connect(buttonBox.get(), &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        break;

      case QDialogButtonBox::RejectRole:
        dialog->connect(buttonBox.get(), &QDialogButtonBox::rejected, dialog, &QDialog::reject);
        break;

      default:
        break;
      }
    }
  }

  return std::move(buttonBox);
}

DialogButtonBoxFactory::StandardButtonInfo::StandardButtonInfo(
    QDialogButtonBox::StandardButton button, const QString& text)
  : button(button),
    text(text)
{
}

}}} // namespace mad::utils::ui
