#pragma once

#include "feature.hpp"

#include <QObject>
#include <QPoint>

#include <memory>
#include <optional>

namespace mad { namespace utils { namespace gui {

class ZoomFeature : public Feature
{
public:
  class Signals;

private:
  class State;

public:
  ZoomFeature();

  const Signals& sigs() const;

  unsigned int zoom() const;

  void setZoom(unsigned int zoom, const std::optional<QPoint>& pos = {});

  unsigned int zoomStep() const;

  void setZoomStep(unsigned int zoomStep);

  unsigned int minimalZoom() const;

  void setMinimalZoom(unsigned int minZoom);

  unsigned int maximalZoom() const;

  void setMaximalZoom(unsigned int maxZoom);

private:
  unsigned int m_zoom = 100;
  unsigned int m_zoomStep = 10;
  unsigned int m_minZoom = 10;
  unsigned int m_maxZoom = 200;

  mutable std::unique_ptr<Signals> m_sigs;
};

class ZoomFeature::Signals : public QObject
{
  Q_OBJECT

signals:
  void zoomChanged(unsigned int oldZoom, unsigned int newZoom,
      const std::optional<QPoint>& pos) const;
};

}}} // namespace mad::utils::gui
