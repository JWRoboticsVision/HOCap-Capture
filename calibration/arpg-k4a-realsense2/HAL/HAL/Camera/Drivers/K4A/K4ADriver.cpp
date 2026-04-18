#include "K4ADriver.h"
#include <HAL/Utils/TicToc.h>
#include <opencv2/opencv.hpp>

namespace hal {

K4ADriver::K4ADriver(int width, int height, int frame_rate, bool capture_color,
                     bool capture_depth, bool capture_ir,
                     std::string depth_mode,
                     const std::vector<std::string>& ids)
    : ids_(ids),
      width_(width),
      height_(height),
      capture_color_(capture_color),
      capture_depth_(capture_depth),
      capture_ir_(capture_ir),
      frame_rate_(frame_rate),
      depth_mode_(depth_mode) {
  CreateSelectedDevices();
  SetDeviceConfig();
  StartCameras();
  SetChannelCount();
  SetDimensions();
  SetSerialNumbers();
}

K4ADriver::~K4ADriver() {
  for (size_t i = 0; i < devices_.size(); ++i) {
    k4a_device_close(devices_[i]);
  }
}

bool K4ADriver::Capture(CameraMsg& images) {
  const double time = Tic();
  images.set_device_time(time);
  images.set_system_time(time);

  for (size_t i = 0; i < devices_.size(); ++i) {
    switch (k4a_device_get_capture(devices_[i], &capture_, 1000)) {
      case K4A_WAIT_RESULT_SUCCEEDED:
        break;
      case K4A_WAIT_RESULT_TIMEOUT:
        printf("Timed out waiting for a capture\n");
        return false;
      case K4A_WAIT_RESULT_FAILED:
        printf("Failed to read a capture\n");
        k4a_device_close(devices_[0]);
        return false;
    }

    const double time_i = Tic();

    if (capture_color_) {
      k4a_image_t frame = k4a_capture_get_color_image(capture_);
      int w = k4a_image_get_width_pixels(frame);
      int h = k4a_image_get_height_pixels(frame);
      ImageMsg* image = images.add_image();
      image->set_timestamp(time_i);
      image->set_serial_number(serial_numbers_[i]);
      image->set_width(w);
      image->set_height(h);
      image->set_type(PB_UNSIGNED_BYTE);
      image->set_format(PB_BGR);

      // https://stackoverflow.com/questions/57222190/how-to-convert-k4a-image-t-to-opencv-matrix-azure-kinect-sensor-sdk
      // https://github.com/arpg/HAL/blob/master/HAL/Camera/Drivers/Freenect2/Freenect2Driver.cpp
      cv::Mat bgra(h, w, CV_8UC4, (void*)k4a_image_get_buffer(frame),
                   cv::Mat::AUTO_STEP);
      std::vector<cv::Mat1b> channels_bgra;
      cv::split(bgra, channels_bgra);
      std::vector<cv::Mat1b> channels_bgr(channels_bgra.begin(),
                                          channels_bgra.begin() + 3);
      cv::Mat3b bgr;
      cv::merge(channels_bgr, bgr);
      image->set_data(bgr.ptr<unsigned char>(),
                      bgr.rows * bgr.cols * bgr.channels());
    }

    if (capture_depth_) {
      k4a_image_t frame = k4a_capture_get_depth_image(capture_);
      int w = k4a_image_get_width_pixels(frame);
      int h = k4a_image_get_height_pixels(frame);
      ImageMsg* image = images.add_image();
      image->set_timestamp(time_i);
      image->set_serial_number(serial_numbers_[i]);
      image->set_width(w);
      image->set_height(h);
      image->set_type(PB_SHORT);
      image->set_format(PB_LUMINANCE);

      uint8_t* buffer = k4a_image_get_buffer(frame);
      uint16_t* depth_buffer = reinterpret_cast<uint16_t*>(buffer);
      image->set_data(depth_buffer, k4a_image_get_stride_bytes(frame) * h);
    }

    if (capture_ir_) {
      k4a_image_t frame = k4a_capture_get_ir_image(capture_);
      int w = k4a_image_get_width_pixels(frame);
      int h = k4a_image_get_height_pixels(frame);
      ImageMsg* image = images.add_image();
      image->set_timestamp(time_i);
      image->set_serial_number(serial_numbers_[i]);
      image->set_width(w);
      image->set_height(h);
      image->set_type(PB_UNSIGNED_BYTE);
      image->set_format(PB_LUMINANCE);

      // https://github.com/microsoft/Azure-Kinect-Sensor-SDK/issues/490
      uint8_t* buffer = k4a_image_get_buffer(frame);
      uint16_t* ir_buffer = reinterpret_cast<uint16_t*>(buffer);
      std::vector<uint8_t> gray_img(w * h);
      int min_value = 0;
      int max_value = (depth_mode_ == "passive_ir") ? 100 : 1000;
      for (int i = 0; i < w * h; i++) {
        uint16_t v = ir_buffer[i];
        float gray_value = 0.0f;
        if (v <= min_value) {
          gray_value = 0.0f;
        } else if (v >= max_value) {
          gray_value = 1.0f;
        } else {
          gray_value = (float)(v - min_value) / (float)(max_value - min_value);
        }
        gray_img[i] = (uint8_t)(gray_value * 255);
      }
      cv::Mat ir(cv::Size(w, h), CV_8U, gray_img.data());
      image->set_data(ir.ptr<unsigned char>(),
                      ir.rows * ir.cols * ir.channels());
    }
  }

  return true;
}

std::shared_ptr<CameraDriverInterface> K4ADriver::GetInputDevice() {
  return nullptr;
}

size_t K4ADriver::NumChannels() const { return channel_count_; }

size_t K4ADriver::Width(size_t index) const { return dimensions_[index].first; }

size_t K4ADriver::Height(size_t index) const {
  return dimensions_[index].second;
}

// TODO(ywchao): K4ADriver::CreateAllDevices()
void K4ADriver::CreateSelectedDevices() {
  uint32_t device_count = k4a_device_get_installed_count();
  if (device_count == 0) {
    printf("No K4A devices found\n");
  }

  std::vector<bool> is_open(device_count, false);
  for (const std::string& id : ids_) {
    for (uint8_t i = 0; i < device_count; i++) {
      if (is_open[i]) continue;

      k4a_device_t device = NULL;
      if (K4A_RESULT_SUCCEEDED != k4a_device_open(i, &device)) {
        printf("Failed to open K4A device at index %d\n", i);
        continue;
      }

      std::string serial;
      size_t serial_size = 0;
      k4a_device_get_serialnum(device, NULL, &serial_size);
      serial.resize(serial_size);
      k4a_device_get_serialnum(device, &serial[0], &serial_size);
      serial.resize(serial_size - 1);

      if (serial == id) {
        devices_.push_back(device);
        is_open[i] = true;
        break;
      } else {
        k4a_device_close(device);
      }
    }
  }
}

void K4ADriver::SetChannelCount() {
  channel_count_ = 0;
  if (capture_color_) ++channel_count_;
  if (capture_depth_) ++channel_count_;
  if (capture_ir_) ++channel_count_;
  channel_count_ *= devices_.size();
}

void K4ADriver::SetDeviceConfig() {
  config_ = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;

  if (capture_color_) {
    config_.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;

    if (width_ == 1280 && height_ == 720) {
      config_.color_resolution = K4A_COLOR_RESOLUTION_720P;
    } else if (width_ == 1920 && height_ == 1080) {
      config_.color_resolution = K4A_COLOR_RESOLUTION_1080P;
    } else if (width_ == 2560 && height_ == 1440) {
      config_.color_resolution = K4A_COLOR_RESOLUTION_1440P;
    } else if (width_ == 2048 && height_ == 1536) {
      config_.color_resolution = K4A_COLOR_RESOLUTION_1536P;
    } else if (width_ == 3840 && height_ == 2160) {
      config_.color_resolution = K4A_COLOR_RESOLUTION_2160P;
    } else if (width_ == 4096 && height_ == 3072) {
      config_.color_resolution = K4A_COLOR_RESOLUTION_3072P;
    } else {
      printf("Invalid RGB camera resolution: %dx%d\n", width_, height_);
    }
  }

  if (capture_depth_ || capture_ir_) {
    if (depth_mode_ == "nfov_2x2binned") {
      config_.depth_mode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
    } else if (depth_mode_ == "nfov_unbinned") {
      config_.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
    } else if (depth_mode_ == "wfov_2x2binned") {
      config_.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
    } else if (depth_mode_ == "wfov_unbinned") {
      config_.depth_mode = K4A_DEPTH_MODE_WFOV_UNBINNED;
    } else if (depth_mode_ == "passive_ir") {
      config_.depth_mode = K4A_DEPTH_MODE_PASSIVE_IR;
    } else {
      printf("Invalid Depth camera mode: %s\n", depth_mode_.c_str());
    }
  }

  if (frame_rate_ == 5) {
    config_.camera_fps = K4A_FRAMES_PER_SECOND_5;
  } else if (frame_rate_ == 15) {
    config_.camera_fps = K4A_FRAMES_PER_SECOND_15;
  } else if (frame_rate_ == 30) {
    config_.camera_fps = K4A_FRAMES_PER_SECOND_30;
  } else {
    printf("Invalid camera fps: %d\n", frame_rate_);
  }

  if (capture_color_ && (capture_depth_ || capture_ir_)) {
    config_.synchronized_images_only = true;
  } else {
    config_.synchronized_images_only = false;
  }
}

void K4ADriver::StartCameras() {
  for (size_t i = 0; i < devices_.size(); ++i) {
    k4a_device_start_cameras(devices_[i], &config_);
  }
}

void K4ADriver::SetDimensions() {
  for (size_t i = 0; i < devices_.size(); ++i) {
    if (capture_color_) dimensions_.push_back(std::make_pair(width_, height_));
    if (capture_depth_) dimensions_.push_back(GetDepthDimensions());
    if (capture_ir_) dimensions_.push_back(GetDepthDimensions());
  }
}

std::pair<int, int> K4ADriver::GetDepthDimensions() {
  if (depth_mode_ == "nfov_2x2binned") {
    return {320, 288};
  } else if (depth_mode_ == "nfov_unbinned") {
    return {640, 576};
  } else if (depth_mode_ == "wfov_2x2binned") {
    return {512, 512};
  } else if (depth_mode_ == "wfov_unbinned" || depth_mode_ == "passive_ir") {
    return {1024, 1024};
  } else {
    printf("Invalid Depth camera mode: %s\n", depth_mode_.c_str());
    return {-1, -1};
  }
}

void K4ADriver::SetSerialNumbers() {
  for (const std::string& id : ids_) {
    serial_numbers_.push_back(std::strtoull(id.c_str(), nullptr, 10));
  }
}

}  // namespace hal