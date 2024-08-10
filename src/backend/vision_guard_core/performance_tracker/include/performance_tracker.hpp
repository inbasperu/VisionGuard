#ifndef PERFORMANCE_TRACKER_HPP
#define PERFORMANCE_TRACKER_HPP

#include <monitors/presenter.h>
#include <utils/performance_metrics.hpp>

class PerformanceTracker {
public:
  PerformanceTracker();
  void startTracking();
  void updateTracking(cv::Mat &frame);
  void logMeanMetrics() const;

private:
  PerformanceMetrics metrics;
  Presenter resource_presenter;
  std::chrono::steady_clock::time_point start_time;
};

#endif // PERFORMANCE_TRACKER_HPP
