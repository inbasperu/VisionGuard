#include "break_notification_system.hpp"
#include <iostream>
#include <utils/slog.hpp>

BreakNotificationSystem::BreakNotificationSystem() {}

void BreakNotificationSystem::notifyBreak() {
  slog::info << "Time to take a break!" << slog::endl;
}
