// #include "utils/default_flags.hpp"
// #include "vision_guard.hpp"
// #include "vision_guard_demo.hpp"
// #include <gflags/gflags.h>
// #include <iostream>
// #include <opencv2/opencv.hpp>
// #include <openvino/openvino.hpp>
// #include <vector>

// bool ParseAndCheckCommandLine(int argc, char *argv[]) {
//   // Parsing and validating input arguments
//   gflags::ParseCommandLineNonHelpFlags(&argc, &argv, true);
//   if (FLAGS_h) {
//     showUsage();
//     showAvailableDevices();
//     return false;
//   }

//   if (FLAGS_i.empty())
//     throw std::logic_error("Parameter -i is not set");
//   if (FLAGS_m.empty())
//     throw std::logic_error("Parameter -m is not set");
//   if (FLAGS_m_fd.empty())
//     throw std::logic_error("Parameter -m_fd is not set");
//   if (FLAGS_m_hp.empty())
//     throw std::logic_error("Parameter -m_hp is not set");
//   if (FLAGS_m_lm.empty())
//     throw std::logic_error("Parameter -m_lm is not set");
//   if (FLAGS_m_es.empty())
//     throw std::logic_error("Parameter -m_es is not set");

//   return true;
// }

// int main(int argc, char *argv[]) {
//   if (!ParseAndCheckCommandLine(argc, argv)) {
//     return 1;
//   }

//   try {
//     PerformanceMetrics metrics;
//     VisionGuard visionGuard(FLAGS_m, FLAGS_m_fd, FLAGS_m_hp, FLAGS_m_lm,
//                             FLAGS_m_es, FLAGS_d);
//     bool flipImage = false;
//     int delay = 1;
//     std::string windowName = "Gaze estimation demo";

//     // Open the default camera
//     std::unique_ptr<ImagesCapture> cap = openImagesCapture(
//         FLAGS_i, FLAGS_loop, read_type::efficient, 0,
//         std::numeric_limits<size_t>::max(), stringToSize(FLAGS_res));

//     auto startTime = std::chrono::steady_clock::now();
//     cv::Mat frame = cap->read();

//     LazyVideoWriter videoWriter{FLAGS_o, cap->fps(), FLAGS_limit};
//     cv::Size graphSize{frame.cols / 4, 60};
//     Presenter presenter(FLAGS_u, frame.rows - graphSize.height - 10, graphSize);

//     // Calibrate screen
//     cv::Size imageSize(1920, 1080);
//     visionGuard.showCalibrationWindow(imageSize);

//     do {
//       if (flipImage) {
//         cv::flip(frame, frame, 1);
//       }

//       visionGuard.processFrame(frame);

//       presenter.drawGraphs(frame);
//       metrics.update(startTime, frame, {10, 22}, cv::FONT_HERSHEY_COMPLEX,
//                      0.65);
//       videoWriter.write(frame);

//       if (!FLAGS_no_show) {
//         cv::imshow(windowName, frame);

//         // Controls the information being displayed while demo runs
//         int key = cv::waitKey(delay);
//         visionGuard.toggle(key);

//         // Press 'Esc' to quit, 'f' to flip the video horizontally
//         if (key == 27)
//           break;
//         if (key == 'f')
//           flipImage = !flipImage;
//         else
//           presenter.handleKey(key);
//       }

//       frame = cap->read();
//     } while (frame.data);

//     slog::info << "Metrics report:" << slog::endl;
//     metrics.logTotal();
//     slog::info << presenter.reportMeans() << slog::endl;
//   } catch (const std::exception &error) {
//     slog::err << error.what() << slog::endl;
//     return 1;
//   } catch (...) {
//     slog::err << "Unknown/internal exception happened." << slog::endl;
//     return 1;
//   }
//   return 0;
// }
