# Project cost_model
## 1. code structure
### 1.1 class
1. ChipBasic : store parameters of chip wormhole
2. PhysicalLayer : DNN layer and its allocated cores
3. NeuralNetwork : the whole network. Calculate the time of each step.


## TODO:
1. allocate diffrent cores to a layer in forward and backward process
2. data transfer across chips

