#include <HAL/Devices/DeviceFactory.h>
#include "K4ARealSense2Driver.h"

namespace hal {

class K4ARealSense2Factory : public DeviceFactory<CameraDriverInterface> {
 public:
  K4ARealSense2Factory(const std::string& name)
      : DeviceFactory<CameraDriverInterface>(name) {
    Params() = {
        {"k4a_idN", "0", "K4A Camera serial number, where N is zero-base index"},
        {"k4a_size", "1280x720", "K4A capture resolution"},
        {"k4a_fps", "30", "K4A capture frame rate"},
        {"k4a_rgb", "true", "K4A capture RGB image"},
        {"k4a_depth", "true", "K4A capture Depth image"},
        {"k4a_ir", "false", "K4A capture IR image"},
        {"k4a_mode", "nfov_unbinned", "K4A capture depth mode"},
        {"rs2_idN", "0", "RealSense2 Camera serial number, where N is zero-base index"},
        {"rs2_size", "640x480", "RealSense2 capture resolution"},
        {"rs2_fps", "30", "RealSense2 capture frame rate"},
        {"rs2_rgb", "true", "RealSense2 capture RGB image"},
        {"rs2_depth", "true", "RealSense2 capture Depth image"},
        {"rs2_ir0", "false", "RealSense2 capture first IR image"},
        {"rs2_ir1", "false", "RealSense2 capture second IR image"},
        {"rs2_emitter", "0.462", "RealSense2 Laser emitter strength, 0 to disable"},
        {"rs2_exposure", "0", "RealSense2 RGB exposure value, 0 for auto-exposure"},
        {"rs2_gain", "64", "RealSense2 RGB camera gain"},
    };
  }

  std::shared_ptr<CameraDriverInterface> GetDevice(const Uri& uri) {
    ImageDim k4a_dims          = uri.properties.Get("k4a_size", ImageDim(1280, 720));
    bool k4a_capture_color     = uri.properties.Get("k4a_rgb", true);
    bool k4a_capture_depth     = uri.properties.Get("k4a_depth", true);
    bool k4a_capture_ir        = uri.properties.Get("k4a_ir", false);
    int k4a_frame_rate         = uri.properties.Get("k4a_fps", 30);
    std::string k4a_depth_mode = uri.properties.Get("k4a_mode", std::string("nfov_unbinned"));

    ImageDim rs2_dims          = uri.properties.Get("rs2_size", ImageDim(640, 480));
    bool rs2_capture_color     = uri.properties.Get("rs2_rgb", true);
    bool rs2_capture_depth     = uri.properties.Get("rs2_depth", true);
    bool rs2_capture_ir0       = uri.properties.Get("rs2_ir0", false);
    bool rs2_capture_ir1       = uri.properties.Get("rs2_ir1", false);
    double rs2_exposure        = uri.properties.Get("rs2_exposure", 0.0);
    double rs2_gain            = uri.properties.Get("rs2_gain", 64);
    double rs2_emitter         = uri.properties.Get("rs2_emitter", 0.462);
    int rs2_frame_rate         = uri.properties.Get("rs2_fps", 30);

    std::vector<std::string> k4a_ids;
    std::vector<std::string> rs2_ids;

    while (true) {
      std::stringstream stream;
      stream << "k4a_id" << k4a_ids.size();
      const std::string key = stream.str();
      if (!uri.properties.Contains(key)) break;
      k4a_ids.push_back(uri.properties.Get<std::string>(key, ""));
    }

    while (true) {
      std::stringstream stream;
      stream << "rs2_id" << rs2_ids.size();
      const std::string key = stream.str();
      if (!uri.properties.Contains(key)) break;
      rs2_ids.push_back(uri.properties.Get<std::string>(key, ""));
    }

    std::shared_ptr<K4ARealSense2Driver> driver =
        std::make_shared<K4ARealSense2Driver>(
            k4a_dims.x, k4a_dims.y, k4a_frame_rate, k4a_capture_color,
            k4a_capture_depth, k4a_capture_ir, k4a_depth_mode, k4a_ids,
            rs2_dims.x, rs2_dims.y, rs2_frame_rate, rs2_capture_color,
            rs2_capture_depth, rs2_capture_ir0, rs2_capture_ir1, rs2_ids);

    if (rs2_capture_color) {
      const int channel = rs2_capture_ir0 + rs2_capture_ir1;
      // driver->SetExposure(rs2_exposure, channel);
      // driver->SetGain(rs2_gain, channel);
    }

    for (size_t i = 0; i < driver->NumRS2Devices(); ++i) {
      driver->SetEmitter(i, rs2_emitter);
    }

    return driver;
  }
};

static K4ARealSense2Factory g_RealSenseFactory("k4arealsense2");

}  // namespace hal
