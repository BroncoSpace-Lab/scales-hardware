# Machine Learning Model Inference Performance 

This document will go over the performance of the Jetson AGX Orin 64 Dev Kit while inferecing over a variety of different common models. This is supposed to provide a reference baseline for developers to gather and understanding of how their models may performa on the SCALES system.

## Models Tested
- Resnet-152
    - Image classification
    - [Model Card](https://huggingface.co/microsoft/resnet-152)
    - Parameters: 60.3M

- Depth Anything V2
    - Depth Estimation 
    - [Model Card](https://huggingface.co/depth-anything/Depth-Anything-V2-Base-hf)
    - Parameters: 97.5M
- YOLO V8
    - Image Classification
    - [Model Card](https://huggingface.co/Ultralytics/YOLOv8)
    - Parameters: 3.2M
- Phi-3.5
    - Language Model
    - [Model Card](https://huggingface.co/microsoft/Phi-3.5-mini-instruct)
    - Parameters: 3.82B
## Procedure 
Models were choosen from the hugging face model hub and then completed inference tests on the jetson. The hugging face transformer pipeline api as well as data sets were used to crate the inference enviroment and data. 

The data was collected using the [tegrastats data parser](https://github.com/ssaraff98/tegrastats_parser) which is able to read all the metrics from the jetson and save all the data into a csv file while also plotting some of the key metrics. 

If interested in other metrics Tegrastats records all of the following metrics:

- Time
- Used RAM
- Total Ram
- Number of Free RAM Blocks
- Size of Free RAM Blocks
- Used SWAP
- Total SWAP
- Cached SWAP
- CPU Frequency
- CPU 0-11 Load %
- Used GR3D %
- GR3D Frequency
- CPU Temperature
- tboard Temperature
- Temperature 
- tdiode Temperature
- tj Temperature


Each of the models were tested at all 4 of the Jetson power modes to show performance in resource constrained enviroments

## Resnet Results
The resnet model was tested using the [CIFAR100](https://huggingface.co/datasets/uoft-cs/cifar100) dataset.The team used the test split to inference 10,000 images.

<ins>Resnet Test Results</ins>

|Test|Average FPS|Average GPU %| Average RAM (MB)| Total Inference Time (s)| Power Draw (W) |
| ----------- | ----------- | ----------- | ----------- | ----------- |----------- |
|MaxN| 21.01 | 57.16 | 10128.28 | 475.99  ||
|50W| 15.27 | 52.39  |10161.1   |  655.08  |12.18|  
|30W| 16.91 | 61.29  | 4255.58  | 591.32  |12.72|
|15W| 10.58 | 68.48  | 4075.85  | 945.32   ||

### Raw Data

- [MaxN Raw Data](data/resnet_test.csv)
- [50W Raw Data](data/resnet_50W.csv)
- [30W Raw Data](data/resnet_30W.csv)
- [15W Raw Data](data/resnet_15W.csv)

### Graphs
#### MAXN Data
![MAXN Used GR3D %](Images/resnet_maxn_gr3d.png)
![MAXN Used RAM](Images/resnet_maxn_used_ram.png)

#### 50W Data
![50W Used GR3D %](Images/restnet_50W_gr3d.png)
![50W Used RAM](Images/resnet_50W_used_ram.png)

#### 30W Data
![30W Used GR3D %](Images/resnet_30W_gr3d.png)
![30W Used RAM](Images/resnet_30W_used_ram.png)

#### 15W Data
![15W Used GR3D %](Images/resnet_15W_gr3d.png)
![15W Used RAM](Images/resenet_15W_used_ram.png)


## Depth Anything Results
The resnet model was tested using the [CIFAR100](https://huggingface.co/datasets/uoft-cs/cifar100) dataset.The team used the test split to inference 1,000 images.

<ins>Depth Anything Test Results</ins>

|Test|Average FPS|Average GPU %| Average RAM (MB)| Total Inference Time (s)|Power Draw (W)| 
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
|MaxN| 5.25 | 45.98 | 4453.77 | 190.54  |21.25|
|50W| 3.71 | 90.62   | 4967.38 | 269.25 |27.78|  
|30W| 1.53 | 93.67  | 4665.56 |  655.45 |15.13|
|15W| 0.83 | 95.95 | 4903.13 |1198.83   |11.91|

### Raw Data

- [MaxN Raw Data](data/depth_maxn.csv)
- [50W Raw Data](data/depth_50W.csv)
- [30W Raw Data](data/depth_30W.csv)
- [15W Raw Data](data/depth_15W.csv)

### Graphs
#### MAXN Data
![MAXN Used GR3D %](Images/depth_maxn_gr3d.png)
![MAXN Used RAM](Images/depth_maxn_used_ram.png)

#### 50W Data
![50W Used GR3D %](Images/depth_50W_gr3d.png)
![50W Used RAM](Images/depth_50W_used_ram.png)

#### 30W Data
![30W Used GR3D %](Images/depth_30W_gr3d.png)
![30W Used RAM](Images/depth_30W_used_ram.png)

#### 15W Data
![15W Used GR3D %](Images/depth_15W_gr3d.png)
![15W Used RAM](Images/depth_15W_used_ram.png)

## Phi-3 Results

<ins>Phi-3 Test Results</ins>

|Test|Tokens/s|Average GPU %| Average RAM (MB)| Total Generation Time (s)|
| ----------- | ----------- | ----------- | ----------- | ----------- |
|MaxN| 3.72 |43.5  | 9857.42 | 129.47 |
|50W| 3.62 | 53.41  | 11094.16|132.94 |  
|30W|  3.83 | 57.91 | 11771.74 | 125.59  |
|15W|   |  |     | |


## Notes

- The jetson power modes are designed to optimize for power draw which can limit the upper end of performance. For example when running the Phi-3 model in MaxN mode the GPU was running at the minium frequency of 300 Mhz and gets the 3.72 tokens/second. If the user is to manually edit the power modes to set the GPU frequency to the max of 1.3 GHz the jetson is able to produce ~8 tokens/second of performance. Now the downside is that you jump from ~20W of power draw to ~50W but this is an operational trade off between uptime and speed. 

- From the previous note we have noticed that this performance is only acheived on some models. This might be because of implemenation methodolies but for example we saw large performance increases with the phi-3 model but for the resnet and depth anything there was no impactful change in performance even when the GPU frequency was manually increased. 

- There may be other settings for each power mode that can continue to improve performance of models. The metrics data primarly use the default power states of of the Jetson

- There is a weird discrpancy in the NVIDIA documentation about the AGX Orin Power modes. So all the testing for this file has been done on a 32GB version of the AGX orin and the documentation says that there should be a 40W mode but there is only a 50W mode which is the documentation for the 64gb version of the board. additionally this board is having the max gpu frequency that is only supposed to be on the 64Gb version. So worried about the consistecny of the results on a 64gb board. 

Things to do still 

- Flash and test everything on a 64gb Jetson AGX Orin
- Delete Collumns with zeros at the beginning and end of the csv data files
- Redo the average calcs for gr3d % usage with zero rows removed
- Go back and reparse all the txt data so that the csv gets the power draw metrics as well
- edit the tegrastats average calcualtor to add the power draws and give total power draw and an average
- Redo resnet 15W run
- Redo Resnet maxn run