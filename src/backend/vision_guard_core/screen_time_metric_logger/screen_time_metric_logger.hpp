#ifndef SCREEN_TIME_METRIC_LOGGER_HPP
#define SCREEN_TIME_METRIC_LOGGER_HPP

#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <chrono>

class ScreenTimeMetricLogger {
public:
    ScreenTimeMetricLogger(const std::string &filePath);

    void logGazeData();
    void createEmptyDataFile();
    void cleanOldData();

    std::map<std::string, double> getDailyStats();
    std::map<std::string, double> getWeeklyStats();

    nlohmann::json readDataFile();
    void saveJsonData(const nlohmann::json &data, const std::string &filePath);

    std::string getHourlyTimestamp() const;
    void updateHourlyData(nlohmann::json &data, const std::string &key, double value);

private:
    std::string dataFilePath;
    std::chrono::steady_clock::time_point start_time;
};

#endif // SCREEN_TIME_METRIC_LOGGER_HPP
