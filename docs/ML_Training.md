# Machine Learning Model Training Performance

# Model Trained

- Resnet-50 

## Training Results 

<ins>Resnet Training Performance Results</ins>

|Test|Average Time/Epoch |Average GPU %| Average RAM (MB)| Total Training Time | Test Accuary | Power Draw (W) | 
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
|MaxN (P)| 2:12 | 94.03 | 9539.33 | 22:08 | 41.84%| 51.03 |
|MaxN (S)|2:11 | 92.86 | 7965.45 |22:19 |42.66% |50.36 |
|50W|  |    |  |    |  
|30W|  |   |   |  |
|15W|  |  |  |   |

- (P) denotes that the power mode was adjusted to hit maximum GPU frequency
- (S) denotest the use of the standard power mode configuration
- Note: When conducting this training run the standard mode did automatically increase the GPU frequency to the max to accomidate the large computational load

