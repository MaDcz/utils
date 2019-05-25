#pragma once

#include <QDialog>

#include <memory>

class QWidget;

namespace mad { namespace utils { namespace gui {

class Dialog : public QDialog
{
public:
  explicit Dialog(QWidget* parent = nullptr);

  explicit Dialog(std::unique_ptr<QWidget>&& mainWidget, QWidget* parent = nullptr);
};

}}} // namespace mad::utils::gui
