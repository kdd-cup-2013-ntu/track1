#!/bin/bash

fea=$1
csv=$2
model=$3

# add query id to the feature file
python add_qid.py $csv $fea train

# jforests converts its input data sets to binary format
rm -rf jforests-*
rm -rf *.bin
java -jar jforests.jar --cmd=generate-bin --ranking --folder . --file train > /dev/null

# train 10 models with different random seeds(0~9 set in property files in properties/) and save to models/
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking0.properties --train-file train.bin --validation-file train.bin --output-model models/model0 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking1.properties --train-file train.bin --validation-file train.bin --output-model models/model1 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking2.properties --train-file train.bin --validation-file train.bin --output-model models/model2 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking3.properties --train-file train.bin --validation-file train.bin --output-model models/model3 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking4.properties --train-file train.bin --validation-file train.bin --output-model models/model4 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking5.properties --train-file train.bin --validation-file train.bin --output-model models/model5 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking6.properties --train-file train.bin --validation-file train.bin --output-model models/model6 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking7.properties --train-file train.bin --validation-file train.bin --output-model models/model7 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking8.properties --train-file train.bin --validation-file train.bin --output-model models/model8 > /dev/null
java -jar jforests.jar --cmd=train --ranking --config-file properties/ranking9.properties --train-file train.bin --validation-file train.bin --output-model models/model9 > /dev/null

# concatenate 10 models to one file
python mergeModel.py $model

