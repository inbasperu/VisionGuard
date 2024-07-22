#ifndef PERFORMANCE_TRACKER_HPP
#define PERFORMANCE_TRACKER_HPP

#include <utils/performance_metrics.hpp>

class PerformanceTracker {
public:
    PerformanceTracker();
    void startTracking();
    void stopTracking();
    PerformanceMetrics getMetrics() const;

private:
    PerformanceMetrics metrics;
};

#endif // PERFORMANCE_TRACKER_HPP
