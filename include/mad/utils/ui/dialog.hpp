#pragma once

#include <QDialog>

#include <memory>

class QWidget;

namespace mad { namespace utils { namespace ui {

class Dialog : public QDialog
{
public:
  Dialog(QWidget* parent = nullptr);

  Dialog(std::unique_ptr<QWidget>&& mainWidget, QWidget* parent = nullptr);
};

}}} // namespace mad::utils::ui
