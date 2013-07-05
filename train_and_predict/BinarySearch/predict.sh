#!/bin/bash

fea=$1
csv=$2
model=$3
predictions=$4

# add query id to the feature file
python add_qid.py $csv $fea valid

# jforests converts its input data sets to binary format
rm -rf jforests-discrete-valid
rm -rf valid.bin
java -jar jforests.jar --cmd=generate-bin --ranking --folder . --file valid > /dev/null

# split concatenated file to original 10 model files
python splitModel.py $model
# predict with 10 lambdaMART models and save to predictions/
java -jar jforests.jar --cmd=predict --ranking --model-file models/model0 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions0.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model1 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions1.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model2 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions2.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model3 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions3.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model4 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions4.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model5 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions5.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model6 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions6.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model7 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions7.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model8 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions8.txt
java -jar jforests.jar --cmd=predict --ranking --model-file models/model9 --tree-type RegressionTree --test-file valid.bin --output-file predictions/predictions9.txt

# average the confidence scores from 10 models
python avg.py $predictions
