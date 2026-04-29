<h1>Python Environment Setup</h1>

<h2>Table of Contents</h2>

## Creating a Virtual Environment

```sh
conda create --name hocap-capture python=3.12
conda activate hocap-capture
```

## Installing Dependencies

- Install PyTorch with CUDA support:

```sh
python -m pip install torch==2.7.1 torchvision==0.22.1 --index-url https://download.pytorch.org/whl/cu118
```

- Install hocap-capture package:

```sh
python -m pip install -e .
```
