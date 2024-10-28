# Machine Learning Model Training Performance

# Model Trained

- Resnet-50 

Was set up to train over the CIFAR100 data set where there is 50,000 training images and 10,000 test images. These images are labeled with 100 different classes. Resnet is a classification model so this data set meets its needs perfectly. The models were only trained up to 10 epochs purely in the conservation of time since this testing is producing results on hardware performance and not model accucary. Users could extrapolate time and resource utilization to train a model to completion from the test results. These results are just to serve as a basline so developers have a commonly used reference model to base their expectations around in terms of onboard performance. 

## Training Results 

<ins>Resnet Training Performance Results</ins>

|Test|Average Time/Epoch |Average GPU %| Average RAM (MB)| Total Training Time | Test Accuary | Power Draw (W) | 
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
|MaxN (P)| 2:12 | 94.03 | 9539.33 | 22:08 | 41.84%| 51.03 |
|MaxN (S)|2:11 | 92.86 | 7965.45 |22:19 |42.66% |50.36 |
|50W| 2:44 | 94.56   | 8467.51 |  28:22  | 37.68% |34.23 | 
|30W| 5:05 | 95.97  |  7907.65 | 52:27 |39.17%|21.60|
|15W| 10:34 | 84.18 | 7868.99 |  1:46:40 |32.07%|13.62|

- (P) denotes that the power mode was adjusted to hit maximum GPU frequency
- (S) denotest the use of the standard power mode configuration
- Note: When conducting this training run the standard mode did automatically increase the GPU frequency to the max to accomidate the large computational load


# Notes

The GPU has a different max frequency at each power mode that effects performance.

Unlike inferencing when training the Jetson seems to default to using the max frequency allocation for each power mode to get the most performance possible for the very intenisve task

Frequencies: 
MaxN - 1.3 GHZ 
50W - 828 MHz
30W - 612 MHz
15W - 408 MHz
