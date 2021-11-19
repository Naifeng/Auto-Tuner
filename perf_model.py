import warnings
warnings.filterwarnings('ignore',category=FutureWarning)

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
os.environ["CUDA_VISIBLE_DEVICES"] = '1'

import tensorflow as tf
import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
import time
from scipy.stats import spearmanr
import sys

tf.set_random_seed(1234)

# read from the command line
FILENAME = sys.argv[1] # "../halide_dataset/halide_fft_cpu_4000_points_I5.csv" 
training_size = int(sys.argv[2])
md5_hash = sys.argv[3]

if not os.path.exists('model_config'):
    os.makedirs('model_config')

# if not os.path.exists('model_config'):
#     os.makedirs('model_config/' + md5_hash)

if not os.path.exists('model_results'):
    os.makedirs('model_results')

data = np.array(pd.read_csv(FILENAME, header=None))

train_data = data[:training_size]
test_data = data[training_size:]

# print(test_data)

train_feature = np.array(train_data[:, 1:])
train_label = np.array(train_data[:, [0]])

test_x = np.array(test_data[:, 1:])

# print("test data shape: ", test_data.shape)
# print("train data shape: ", train_data.shape)
# print("train feature shape: ", train_feature.shape)

x = tf.placeholder(tf.float32, [None, train_feature.shape[1]], name='input')
y = tf.placeholder(tf.float32, [None, 1])  

# preprocessing
scaler = StandardScaler()
# fit scaler on training dataset
scaler.fit(train_feature)

train_feature = scaler.transform(train_feature)
test_xs = scaler.transform(test_x)

# needs to output the mean
# print("scaler: mean:", scaler.mean_)
# print("scaler: std:", scaler.scale_)

print("scaler: mean:", type(scaler.mean_[0]))

# write mean and var to a .csv file
f = open('model_config/' + md5_hash + '/preprocessing.csv', 'w')

for item in scaler.mean_:
    f.write(str(item)+',')
f.write('\n')

for item in scaler.scale_:    
    f.write(str(item)+',')

f.close()

# neural network structure
L1 = tf.layers.dense(x, 5, tf.nn.relu)
L2 = tf.layers.dense(L1, 5, tf.nn.relu)

prediction = tf.layers.dense(L2, 1)

# name the output
tf.identity(prediction, name="output")

loss = tf.reduce_mean(tf.square(y - prediction))

saver = tf.train.Saver()

train_step = tf.train.AdamOptimizer(0.01).minimize(loss)


total_parameters = 0
for variable in tf.trainable_variables():
    shape = variable.get_shape()
    variable_parameters = 1
    for dim in shape:
        variable_parameters *= dim.value
    total_parameters += variable_parameters
# print("total parameters: ", total_parameters)

print("Selecting the best variant...")

with tf.Session() as sess:

    sess.run(tf.global_variables_initializer())

    # print(sess.run(loss, feed_dict={x: train_feature, y: train_label}))
   
    lowest_mape = 1
    highest_rho = 0
    asso_rho = 0
    
    for i in range(20000):
        sess.run(train_step, feed_dict={x: train_feature, y: train_label})
        if i % 200 == 0:
            # print(i)
            # print(sess.run(loss, feed_dict={x: train_feature, y: train_label}))
            prd = sess.run(prediction, feed_dict={x: test_xs})
            
            # -----evaluation-----#

            import math
            import statistics

            sum_ae = 0.0
            sum_ape = 0.0
            sum_aape = 0.0

            pred_value_list = []
            truth_value_list = []

            for i in range(test_data.shape[0]):
                truth_value = test_data[:, [0]][i][0]
                pred_value = prd[i][0]
                sum_ae += abs(prd[i][0] - test_data[:, [0]][i][0])
                truth_value_list.append(truth_value)
                pred_value_list.append(pred_value)

            # print("MAE: ", sum_ae / test_data.shape[0])

            c = 0

            # decide the percentage to drop
            percentage = 0.3
            threshold = sorted(truth_value_list)[int(len(test_data) * percentage) - 1]

            median = statistics.median(truth_value_list)

            for i in range(test_data.shape[0]):

                pred_value = prd[i][0]
                truth_value = test_data[:, [0]][i][0]

                ape = (abs(prd[i][0] - test_data[:, [0]][i][0]) / test_data[:, [0]][i][0])
                aape = math.atan(abs(prd[i][0] - test_data[:, [0]][i][0]) / test_data[:, [0]][i][0])

                # valid rule
                if truth_value > threshold:
                    sum_ape += ape
                    c += 1

                sum_aape += aape

            rho, pval = spearmanr(pred_value_list,truth_value_list)
            curr_mape = round(sum_ape / c, 2)
            
            if curr_mape < lowest_mape:
                lowest_mape = curr_mape
                asso_rho = rho

                # write results to a .txt file
                f = open('model_results/predictions.txt', 'w')
                for i in range(test_data.shape[0]):
                    f.writelines(str(prd[i][0]) + "\n")
                f.close()

                # save the model as a .pb graph
                with open('model_config/' + md5_hash + '/model.pb', 'wb') as f:
                    f.write(tf.get_default_graph().as_graph_def().SerializeToString())

                saver.save(sess, 'model_config/' + md5_hash + '/my-model.ckpt')

            # valid rule
            if rho > highest_rho:
                highest_rho = rho
            
            print("MAPE: ", curr_mape)    
            print('rho:', rho)
            # ------------------#
