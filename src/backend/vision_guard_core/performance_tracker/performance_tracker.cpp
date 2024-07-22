#include "performance_tracker.hpp"

PerformanceTracker::PerformanceTracker() {}

void PerformanceTracker::startTracking() {
    metrics.start();
}

void PerformanceTracker::stopTracking() {
    metrics.stop();
}

PerformanceMetrics PerformanceTracker::getMetrics() const {
    return metrics;
}
