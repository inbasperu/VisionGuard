```mermaid
---
title: VisionGuard Core Architecture
---
classDiagram
    class VisionGuardCore

    class GazeDetectionEngine
    class VectorCalibration
    class EyeGazeTimeTracker
    class BreakNotificationSystem
    class PerformanceTracker
    class ScreenTimeMetricLogger

    VisionGuardCore *-- GazeDetectionEngine
    VisionGuardCore *-- VectorCalibration
    VisionGuardCore *-- EyeGazeTimeTracker
    VisionGuardCore *-- BreakNotificationSystem
    VisionGuardCore *-- PerformanceTracker
    VisionGuardCore *-- ScreenTimeMetricLogger

```
