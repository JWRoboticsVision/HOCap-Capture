#include <HAL/Devices/DeviceFactory.h>
#include "K4ADriver.h"

namespace hal {

class K4AFactory : public DeviceFactory<CameraDriverInterface> {
 public:
  K4AFactory(const std::string& name)
      : DeviceFactory<CameraDriverInterface>(name) {
    Params() = {
        {"idN", "0", "Camera serial number, where N is zero-base index"},
        {"size", "1280x720", "Capture resolution"},
        {"fps", "30", "Capture frame rate"},
        {"rgb", "true", "Capture RGB image"},
        {"depth", "true", "Capture Depth image"},
        {"ir", "false", "Capture IR image"},
        {"mode", "nfov_unbinned", "Capture depth mode"},
    };
  }

  std::shared_ptr<CameraDriverInterface> GetDevice(const Uri& uri) {
    ImageDim dims          = uri.properties.Get("size", ImageDim(1280, 720));
    bool capture_color     = uri.properties.Get("rgb", true);
    bool capture_depth     = uri.properties.Get("depth", true);
    bool capture_ir        = uri.properties.Get("ir", false);
    int frame_rate         = uri.properties.Get("fps", 30);
    std::string depth_mode = uri.properties.Get("mode", std::string("nfov_unbinned"));

    std::vector<std::string> ids;

    while (true) {
      std::stringstream stream;
      stream << "id" << ids.size();
      const std::string key = stream.str();
      if (!uri.properties.Contains(key)) break;
      ids.push_back(uri.properties.Get<std::string>(key, ""));
    }

    std::shared_ptr<K4ADriver> driver =
        std::make_shared<K4ADriver>(dims.x, dims.y, frame_rate, capture_color,
                                    capture_depth, capture_ir, depth_mode, ids);

    return driver;
  }
};

static K4AFactory g_K4AFactory("k4a");

}  // namespace hal