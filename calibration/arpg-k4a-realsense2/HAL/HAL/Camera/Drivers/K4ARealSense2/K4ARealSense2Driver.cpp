#include "K4ARealSense2Driver.h"

namespace hal {

K4ARealSense2Driver::K4ARealSense2Driver(
    int k4a_width, int k4a_height, int k4a_frame_rate, bool k4a_capture_color,
    bool k4a_capture_depth, bool k4a_capture_ir, std::string k4a_depth_mode,
    const std::vector<std::string>& k4a_ids, int rs2_width, int rs2_height,
    int rs2_frame_rate, bool rs2_capture_color, bool rs2_capture_depth,
    bool rs2_capture_ir0, bool rs2_capture_ir1,
    const std::vector<std::string>& rs2_ids)
    : k4a_ids_(k4a_ids),
      k4a_width_(k4a_width),
      k4a_height_(k4a_height),
      k4a_capture_color_(k4a_capture_color),
      k4a_capture_depth_(k4a_capture_depth),
      k4a_capture_ir_(k4a_capture_ir),
      k4a_frame_rate_(k4a_frame_rate),
      k4a_depth_mode_(k4a_depth_mode),
      rs2_ids_(rs2_ids),
      rs2_width_(rs2_width),
      rs2_height_(rs2_height),
      rs2_capture_color_(rs2_capture_color),
      rs2_capture_depth_(rs2_capture_depth),
      rs2_capture_ir0_(rs2_capture_ir0),
      rs2_capture_ir1_(rs2_capture_ir1),
      rs2_frame_rate_(rs2_frame_rate) {
  Initialize();
}

K4ARealSense2Driver::~K4ARealSense2Driver() {}

bool K4ARealSense2Driver::Capture(CameraMsg& images) {
  if (!driver_k4a_->Capture(images)) return false;
  if (!driver_rs2_->Capture(images)) return false;
  return true;
}

std::shared_ptr<CameraDriverInterface> K4ARealSense2Driver::GetInputDevice() {
  return nullptr;
}

size_t K4ARealSense2Driver::NumChannels() const { return channel_count_; }

size_t K4ARealSense2Driver::Width(size_t index) const {
  return (driver_map_[index] == 0) ? driver_k4a_->Width(channel_map_[index])
                                   : driver_rs2_->Width(channel_map_[index]);
}

size_t K4ARealSense2Driver::Height(size_t index) const {
  return (driver_map_[index] == 0) ? driver_k4a_->Height(channel_map_[index])
                                   : driver_rs2_->Height(channel_map_[index]);
}

double K4ARealSense2Driver::MaxExposure(int channel) const {
  return driver_rs2_->MaxExposure(channel_map_[channel]);
}

double K4ARealSense2Driver::MinExposure(int channel) const {
  return driver_rs2_->MinExposure(channel_map_[channel]);
}

double K4ARealSense2Driver::MaxGain(int channel) const {
  return driver_rs2_->MaxGain(channel_map_[channel]);
}

double K4ARealSense2Driver::MinGain(int channel) const {
  return driver_rs2_->MinGain(channel_map_[channel]);
}

double K4ARealSense2Driver::Exposure(int channel) {
  return driver_rs2_->Exposure(channel_map_[channel]);
}

void K4ARealSense2Driver::SetExposure(double exposure, int channel) {
  driver_rs2_->SetExposure(exposure, channel_map_[channel]);
}

double K4ARealSense2Driver::Gain(int channel) {
  return driver_rs2_->Gain(channel_map_[channel]);
}

void K4ARealSense2Driver::SetGain(double gain, int channel) {
  driver_rs2_->SetGain(gain, channel_map_[channel]);
}

double K4ARealSense2Driver::ProportionalGain(int channel) const {
  return driver_rs2_->ProportionalGain(channel_map_[channel]);
}

double K4ARealSense2Driver::IntegralGain(int channel) const {
  return driver_rs2_->IntegralGain(channel_map_[channel]);
}

double K4ARealSense2Driver::DerivativeGain(int channel) const {
  return driver_rs2_->DerivativeGain(channel_map_[channel]);
}

double K4ARealSense2Driver::Emitter(int device) const {
  return driver_rs2_->Emitter(device);
}

void K4ARealSense2Driver::SetEmitter(int device, double emitter) const {
  return driver_rs2_->SetEmitter(device, emitter);
}

size_t K4ARealSense2Driver::NumRS2Devices() const {
  return driver_rs2_->NumDevices();
}

void K4ARealSense2Driver::Initialize() {
  CreateSelectedDevices();
  SetChannelCount();
  CreateMapping();
}

// TODO(ywchao): K4ARealSense2Driver::CreateAllDevices()
void K4ARealSense2Driver::CreateSelectedDevices() {
  driver_k4a_ = std::make_shared<K4ADriver>(
      k4a_width_, k4a_height_, k4a_frame_rate_, k4a_capture_color_,
      k4a_capture_depth_, k4a_capture_ir_, k4a_depth_mode_, k4a_ids_);
  driver_rs2_ = std::make_shared<RealSense2Driver>(
      rs2_width_, rs2_height_, rs2_frame_rate_, rs2_capture_color_,
      rs2_capture_depth_, rs2_capture_ir0_, rs2_capture_ir1_, rs2_ids_);
}

void K4ARealSense2Driver::SetChannelCount() {
  channel_count_ = driver_k4a_->NumChannels() + driver_rs2_->NumChannels();
}

void K4ARealSense2Driver::CreateMapping() {
  size_t index = 0;
  driver_map_.resize(channel_count_);
  channel_map_.resize(channel_count_);

  for (size_t j = 0; j < driver_k4a_->NumChannels(); ++j) {
    driver_map_[index] = 0;
    channel_map_[index] = j;
    ++index;
  }

  for (size_t j = 0; j < driver_rs2_->NumChannels(); ++j) {
    driver_map_[index] = 1;
    channel_map_[index] = j;
    ++index;
  }
}

}  // namespace hal