#include "performance_tracker.hpp"

PerformanceTracker::PerformanceTracker()
    : resource_presenter("", 650, cv::Size(320, 60)),
      metrics(std::chrono::seconds(1)),
      start_time(std::chrono::steady_clock::now()) {
  slog::debug << "Initialized Performance Tracker" << slog::endl;
}

void PerformanceTracker::startTracking() {
  start_time = std::chrono::steady_clock::now();
}

void PerformanceTracker::updateTracking(cv::Mat &frame) {
  resource_presenter.drawGraphs(frame);
  metrics.update(start_time, frame, {10, 22}, cv::FONT_HERSHEY_COMPLEX, 0.65);
}

void PerformanceTracker::logMeanMetrics() const {
  slog::info << "Metrics report:" << slog::endl;
  metrics.logTotal();
  slog::info << resource_presenter.reportMeans() << slog::endl;
}
