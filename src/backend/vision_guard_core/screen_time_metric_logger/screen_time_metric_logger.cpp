#include "screen_time_metric_logger.hpp"
#include <fstream>
#include <sstream>
#include <ctime>
#include <utils/slog.hpp>

ScreenTimeMetricLogger::ScreenTimeMetricLogger(const std::string &filePath)
    : dataFilePath(filePath), start_time(std::chrono::steady_clock::now()) {}

void ScreenTimeMetricLogger::logGazeData() {
    double gazeDuration = getAccumulatedGazeTime();
    nlohmann::json data = readDataFile();

    updateHourlyData(data, "gaze_time", gazeDuration);

    try {
        saveJsonData(data, dataFilePath);
    } catch (const std::exception &e) {
        slog::err << "Error saving data file: " << e.what() << slog::endl;
        throw;
    }
    slog::debug << "Hourly logs successfully updated: " << dataFilePath << slog::endl;
}

void ScreenTimeMetricLogger::createEmptyDataFile() {
    nlohmann::json emptyData = nlohmann::json::object();
    saveJsonData(emptyData, dataFilePath);
    slog::debug << "Created new data file with empty JSON object: " << dataFilePath << slog::endl;
}

void ScreenTimeMetricLogger::cleanOldData() {
    auto now = std::chrono::system_clock::now();
    auto oneWeekAgo = now - std::chrono::hours(24 * 7);
    nlohmann::json data = readDataFile();

    for (auto it = data.begin(); it != data.end();) {
        std::istringstream ss(it.key());
        std::tm tm = {};
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail()) {
            slog::err << "Failed to parse timestamp: " << it.key() << slog::endl;
            ++it;
            continue;
        }
        auto timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        if (timestamp < oneWeekAgo) {
            slog::debug << "Erasing timestamp: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << slog::endl;
            it = data.erase(it);
        } else {
            ++it;
        }
    }

    try {
        saveJsonData(data, dataFilePath);
    } catch (const std::exception &e) {
        slog::err << "Error saving data file: " << e.what() << slog::endl;
        throw;
    }

    slog::debug << "Successfully removed old data from: " << dataFilePath << slog::endl;
}

std::map<std::string, double> ScreenTimeMetricLogger::getDailyStats() {
    nlohmann::json data = readDataFile();
    std::map<std::string, double> dailyStats;

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&time);
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now_tm);
    std::string today(buffer);

    for (int i = 0; i < 24; ++i) {
        char hourBin[3];
        std::snprintf(hourBin, sizeof(hourBin), "%02d", i);
        dailyStats[hourBin] = 0;
    }

    for (auto &[key, value] : data.items()) {
        if (key.substr(0, 10) == today) {
            std::string hourBin = key.substr(11, 2);
            dailyStats[hourBin] += value["gaze_time"].get<double>();
        }
    }

    return dailyStats;
}

std::map<std::string, double> ScreenTimeMetricLogger::getWeeklyStats() {
    nlohmann::json data = readDataFile();
    std::map<std::string, double> weeklyStats;

    auto now = std::chrono::system_clock::now();
    auto oneWeekAgo = now - std::chrono::hours(24 * 7);

    for (auto &[key, value] : data.items()) {
        std::istringstream ss(key);
        std::tm tm = {};
        ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        if (ss.fail()) {
            slog::err << "Failed to parse timestamp: " << key << slog::endl;
            continue;
        }
        auto timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        if (timestamp >= oneWeekAgo) {
            std::string date = key.substr(2, 8);
            weeklyStats[date] += value["gaze_time"].get<double>();
        }
    }

    return weeklyStats;
}

nlohmann::json ScreenTimeMetricLogger::readDataFile() {
    nlohmann::json data;

    try {
        std::ifstream inFile(dataFilePath);
        if (inFile.is_open()) {
            if (inFile.peek() == std::ifstream::traits_type::eof()) {
                inFile.close();
                createEmptyDataFile();
                data = nlohmann::json::object();
            } else {
                inFile >> data;
                inFile.close();
                slog::debug << "Data file read successfully: " << dataFilePath << slog::endl;
            }
        } else {
            slog::err << "Unable to open file for reading: " << dataFilePath << slog::endl;
            createEmptyDataFile();
            data = nlohmann::json::object();
        }
    } catch (const std::exception &e) {
        slog::err << "Error reading data file: " << e.what() << slog::endl;
        throw;
    }

    return data;
}

void ScreenTimeMetricLogger::saveJsonData(const nlohmann::json &data, const std::string &filePath) {
    try {
        std::ofstream outFile(filePath);
        if (outFile.is_open()) {
            outFile << data.dump(4);
            outFile.close();
            slog::debug << "Data file saved successfully: " << filePath << slog::endl;
        } else {
            slog::err << "Unable to open file for writing: " << filePath << slog::endl;
            throw std::runtime_error("File open error while saving data file");
        }
    } catch (const std::exception &e) {
        slog::err << "Error saving data file: " << e.what() << slog::endl;
        throw;
    }
}

std::string ScreenTimeMetricLogger::getHourlyTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&time);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:00:00", now_tm);
    return std::string(buffer);
}

void ScreenTimeMetricLogger::updateHourlyData(nlohmann::json &data, const std::string &key, double value) {
    std::string hour = getHourlyTimestamp();

    if (data.contains(hour)) {
        data[hour][key] = data[hour][key].get<double>() + value;
    } else {
        data[hour][key] = value;
    }
}
