#ifndef SCREEN_TIME_METRIC_LOGGER_HPP
#define SCREEN_TIME_METRIC_LOGGER_HPP

#include <chrono>
#include <map>
#include <nlohmann/json.hpp>
#include <string>

class ScreenTimeMetricLogger {
public:
  // Constructor
  ScreenTimeMetricLogger(const std::string &filePath);

  // Log data methods
  void logGazeData(const double gazeDuration);

  // Getters
  std::map<std::string, double> getDailyStats();
  std::map<std::string, double> getWeeklyStats();

private:
  // Utility methods
  void cleanOldData();
  void createEmptyDataFile();
  std::string getHourlyTimestamp() const;
  nlohmann::json readDataFile();
  void saveJsonData(const nlohmann::json &data, const std::string &filePath);
  void updateHourlyData(nlohmann::json &data, const std::string &key,
                        double value);
  // Data members
  std::string dataFilePath;
};

#endif // SCREEN_TIME_METRIC_LOGGER_HPP
