KDD CUP 2013 - Track 1
===================

# License
Copyright 2013 Cheng-Xia Chang, Wei-Cheng Chang, Wei-Sheng Chin, Kuan-Hao Huang, Yu-Chin Juan, Tzu-Ming Kuo, Chun-Liang Li, Chih-Jen Lin, Hsuan-Tien Lin, Shan-Wei Lin, Shou-De Lin, Ting-Wei Lin, Young-San Lin, Yu-Chen Lu, Yu-Chuan Su, Cheng-Hao Tsai, Hsiao-Yu Tung, Jui-Pin Wang, Cheng-Kuang Wei, Felix Wu, Chun-Pai Yang, Tu-Chun Yin, Tong Yu, and Yong Zhuang

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

# Environment
a. The hardware / OS platform you used

Intel(R) Xeon(R) CPU X5690  @ 3.47GHz/ Ubuntu 10.04.4

b. Any necessary 3rd-party software (+ installation steps)

1) c++ source code require c++11 support

2) Java 1.7.0_25
Package: Jforest (included in this package)

3) Python 2.7.5 and 3.3

python -> 2.7.5 and python3 -> 3.3

Python depencecy:

	(1) pandas-0.10.1

	(2) sklearn 0.14-git

	(3) jellyfish 0.1.2

	(4) nltk 2.0

# How to train models
1) Type "make" to compile codes. 

2) Set paths in SETTINGS.json

3) Put all raw data into the directory "TRAIN_DATA_DIR_PATH".

4) Run train.py

Because the size of random forest model is very large, we choose not to 
save it. Therefore, the training and prediction process of random forest 
are both in predict.py. 

# How to make predictions on a new test set
1) Set paths in SETTINGS.json

2) Run predict.py
