#pragma once

#include "feature.hpp"

#include <QObject>
#include <QPoint>
#include <QPointer>
#include <QWidgetAction>

#include <memory>
#include <optional>

class QIntValidator;
class QLineEdit;
class QWidgetAction;

namespace mad { namespace utils { namespace gui {

class ZoomFeature : public Feature
{
public:
  class Signals;
  class Action;

  struct Settings
  {
    unsigned int min = 10;
    unsigned int max = 200;
    unsigned int step = 10;
  };

private:
  class State;

public:
  ZoomFeature();

  ~ZoomFeature() override;

  const Signals& sigs() const;

  unsigned int zoom() const;

  void setZoom(unsigned int zoom, const std::optional<QPoint>& pos = {});

  void increaseZoom(int stepsCount = 1, const std::optional<QPoint>& pos = {});

  void decreaseZoom(int stepsCount = 1, const std::optional<QPoint>& pos = {});

  unsigned int minimalZoom() const;

  void setMinimalZoom(unsigned int minZoom);

  unsigned int maximalZoom() const;

  void setMaximalZoom(unsigned int maxZoom);

  unsigned int zoomStep() const;

  void setZoomStep(unsigned int zoomStep);

  const Settings& settings() const;

  void setSettings(const Settings& settings);

  QAction* action() override;

private:
  unsigned int m_zoom = 100;
  Settings m_settings;

  mutable std::unique_ptr<Signals> m_sigs;
  std::unique_ptr<QWidgetAction> m_action;
};

class ZoomFeature::Signals : public QObject
{
  Q_OBJECT

signals:
  void zoomChanged(unsigned int newZoom, const std::optional<QPoint>& pos,
      unsigned int oldZoom) const;

  void settingsChanged(const Settings& newSettings, const Settings& oldSettings) const;
};

class ZoomFeature::Action : public QWidgetAction
{
  Q_OBJECT

public:
  Action(ZoomFeature& zoomFeature, QObject* parent = nullptr);

  ~Action() override;

protected:
  QWidget* createWidget(QWidget* parent) override;

private slots:
  void zoomPlusClicked();

  void zoomMinusClicked();

  void zoomChanged(unsigned int newZoom);

  void zoomSettingsChanged(const Settings& newSettings);

  void zoomEditingFinished();

private:
  ZoomFeature& m_zoomFeature;
  std::vector<QPointer<QLineEdit>> m_zoomEdits;
  QIntValidator* m_zoomEditsValidator = nullptr;
};

}}} // namespace mad::utils::gui
