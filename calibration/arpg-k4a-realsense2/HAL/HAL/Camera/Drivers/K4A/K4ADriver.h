#pragma once

#include <HAL/Camera/CameraDriverInterface.h>
#include <k4a/k4a.h>

namespace hal {

class K4ADriver : public CameraDriverInterface {
 public:
  K4ADriver(int width, int height, int frame_rate, bool capture_color,
            bool capture_depth, bool capture_ir, std::string depth_mode,
            const std::vector<std::string>& ids);

  virtual ~K4ADriver();

  bool Capture(CameraMsg& images) override;

  std::shared_ptr<CameraDriverInterface> GetInputDevice() override;

  size_t NumChannels() const override;

  size_t Width(size_t index = 0) const override;

  size_t Height(size_t index = 0) const override;

 private:
  void CreateSelectedDevices();

  void SetChannelCount();

  // TODO(ywchao): move to an abstract device class.
  void SetDeviceConfig();

  // TODO(ywchao): move to an abstract device class.
  void StartCameras();

  // TODO(ywchao): move to an abstract device class---use Width() & Height().
  void SetDimensions();

  // TODO(ywchao): move to an abstract device class.
  std::pair<int, int> GetDepthDimensions();

  // TODO(ywchao): move to an abstract device class.
  void SetSerialNumbers();

 protected:
  // TODO(ywchao): use an abstract device class.
  std::vector<k4a_device_t> devices_;

  std::vector<std::string> ids_;

  int channel_count_;

  int width_;

  int height_;

  bool capture_color_;

  bool capture_depth_;

  bool capture_ir_;

  int frame_rate_;

  std::string depth_mode_;

  // TODO(ywchao): move to an abstract device class.
  k4a_device_configuration_t config_;

  // TODO(ywchao): move to an abstract device class.
  k4a_capture_t capture_ = NULL;

  // TODO(ywchao): move to an abstract device class---use Width() & Height().
  std::vector<std::pair<int, int>> dimensions_;

  // TODO(ywchao): move to an abstract device class.
  std::vector<uint64_t> serial_numbers_;
};

}  // namespace hal