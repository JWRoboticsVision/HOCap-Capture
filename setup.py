from setuptools import setup, find_packages

# Minimum dependencies required prior to installation
INSTALL_REQUIRES = [
    "numpy<2",
    "opencv-python",
    "pillow",
    "scipy",
    "pyrealsense2",
]

# Installation operation
setup(
    name="hocap_capture",
    version="0.1.0",
    packages=find_packages(include=["hocap_capture", "hocap_capture.*"]),
    author="Jikai Wang",
    author_email="jikai.wang@utdallas.edu",
    maintainer="Jikai Wang",
    maintainer_email="jikai.wang@utdallas.edu",
    description="HOCap Capture Toolkit",
    keywords=["hocap", "data_collection", "robotics", "ros_noetic"],
    install_requires=INSTALL_REQUIRES,
    license="MIT",
    include_package_data=True,
    python_requires=">=3.9",
    classifiers=[
        "Intended Audience :: Developers",
        "Intended Audience :: Science/Research",
        "Natural Language :: English",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.9",
    ],
    zip_safe=False,
)
