#include "gui/dialog.hpp"

#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <assert.h>

namespace mad { namespace utils { namespace gui {

Dialog::Dialog(QWidget* parent)
  : QDialog(parent)
{
  this->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

Dialog::Dialog(std::unique_ptr<QWidget>&& mainWidget, QWidget* parent)
  : Dialog(parent)
{
  assert(mainWidget);

  if (mainWidget->sizePolicy().expandingDirections())
    // TODO Limit the resizing ability to respective direciton?
    setSizeGripEnabled(true);

  auto layout = new QVBoxLayout(this);

  layout->addWidget(mainWidget.release());
  layout->addSpacing(20);

  auto buttonBox = new QDialogButtonBox();
  buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  buttonBox->setOrientation(Qt::Horizontal);
  layout->addWidget(buttonBox);

  resize(500, 350);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

}}} // namespace mad::utils::gui
