#pragma once

#include <HAL/Camera/AutoExposureInterface.h>
#include <HAL/Camera/Drivers/K4A/K4ADriver.h>
#include <HAL/Camera/Drivers/RealSense2/RealSense2Driver.h>
#include <librealsense2/rs.hpp>

namespace hal {

class K4ARealSense2Driver : public AutoExposureInterface {
 public:
  K4ARealSense2Driver(int k4a_width, int k4a_height, int k4a_frame_rate,
                      bool k4a_capture_color, bool k4a_capture_depth,
                      bool k4a_capture_ir, std::string k4a_depth_mode,
                      const std::vector<std::string>& k4a_id, int rs2_width,
                      int rs2_height, int rs2_frame_rate,
                      bool rs2_capture_color, bool rs2_capture_depth,
                      bool rs2_capture_ir0, bool rs2_capture_ir1,
                      const std::vector<std::string>& rs2_id);

  virtual ~K4ARealSense2Driver();

  bool Capture(CameraMsg& images) override;

  std::shared_ptr<CameraDriverInterface> GetInputDevice() override;

  size_t NumChannels() const override;

  size_t Width(size_t index = 0) const override;

  size_t Height(size_t index = 0) const override;

  double MaxExposure(int channel = 0) const override;

  double MinExposure(int channel = 0) const override;

  double MaxGain(int channel = 0) const override;

  double MinGain(int channel = 0) const override;

  double Exposure(int channel = 0) override;

  void SetExposure(double exposure, int channel = 0) override;

  double Gain(int channel = 0) override;

  void SetGain(double gain, int channel = 0) override;

  double ProportionalGain(int channel = 0) const override;

  double IntegralGain(int channel = 0) const override;

  double DerivativeGain(int channel = 0) const override;

  double Emitter(int device) const;

  void SetEmitter(int device, double emitter) const;

  size_t NumRS2Devices() const;

 private:
  void Initialize();

  void CreateSelectedDevices();

  void SetChannelCount();

  void CreateMapping();

 protected:
  std::shared_ptr<K4ADriver> driver_k4a_;

  std::shared_ptr<RealSense2Driver> driver_rs2_;

  std::vector<int> driver_map_;

  std::vector<int> channel_map_;

  int channel_count_;

  std::vector<std::string> k4a_ids_;

  int k4a_width_;

  int k4a_height_;

  bool k4a_capture_color_;

  bool k4a_capture_depth_;

  bool k4a_capture_ir_;

  int k4a_frame_rate_;

  std::string k4a_depth_mode_;

  std::vector<std::string> rs2_ids_;

  int rs2_width_;

  int rs2_height_;

  bool rs2_capture_color_;

  bool rs2_capture_depth_;

  bool rs2_capture_ir0_;

  bool rs2_capture_ir1_;

  int rs2_frame_rate_;
};

}  // namespace hal