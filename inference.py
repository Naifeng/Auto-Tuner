import warnings
warnings.filterwarnings('ignore',category=FutureWarning)

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3' 
os.environ["CUDA_VISIBLE_DEVICES"] = '1'

import tensorflow as tf
import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler
import sys
import csv
import time

tf.set_random_seed(1234)

# read from the command line
FILENAME = sys.argv[1] 
input_dim = sys.argv[2]
md5_hash = sys.argv[3]

data = np.array(pd.read_csv(FILENAME, header=None))

test_data = data[:] # should be [:]
# print(test_data)
# test_feature = np.array(test_data[:, 1:])

test_x = np.array(test_data[:, :])

input_dim_array = np.full((test_x.shape[0], 1), input_dim)

# append input dim to test_x
test_x = np.append(input_dim_array.astype(float), test_x, 1)

# read mean and var for the preprocessing scalar from a .csv file
f = open('model_config/' + md5_hash + '/preprocessing.csv')

reader = csv.reader(f, delimiter=",")
data_read = [row for row in reader]

f.close()

# preprocessing
scaler = StandardScaler()
# setup scaler according to preprocessing.csv
scaler.mean_ = np.array(data_read[0][:-1]).astype(np.float) # [:-1] is used to exclude the last empty element in each row
scaler.scale_ = np.array(data_read[1][:-1]).astype(np.float)
# print(scaler.mean_)
# print(scaler.scale_)

test_xs = scaler.transform(test_x)
# print(test_xs)

x = tf.placeholder(tf.float32, [None, test_x.shape[1]], name='input')

L1 = tf.layers.dense(x, 5, tf.nn.relu)
L2 = tf.layers.dense(L1, 5, tf.nn.relu)

prediction = tf.layers.dense(L2,1)

saver = tf.train.Saver()

with tf.Session() as sess:

    saver.restore(sess, tf.train.latest_checkpoint('model_config/' + md5_hash))
    
    # inference_start = time.clock()
    prd = sess.run(prediction, feed_dict={x: test_xs})
    # inference_end = time.clock()
    
    f = open('model_results/predictions.txt', 'w') 
    for i in range(test_data.shape[0]):
        f.writelines(str(prd[i][0]) + "\n")
    f.close()

# evaluation starts here

# checking the test data
# for i in range(test_data.shape[0]):
#    print(test_data[:, [1]][i][0]) # [1] corresponds to the 'input size'

# # variant-selection
# # selected_info = []
# # write variant-selection results to a .csv file
# f = open('model_results/vs_re_halide_fft.csv', 'a')
# # TODO: change to automatically read in a vector
# # input_sizes = [1024, 2048, 4096, 8192, 16384, 32768] # for Halide blur
# input_sizes = [16, 32, 64, 128] # for Halide fft
# for j in range(len(input_sizes)):
#     variant_runtime = sys.maxsize
#     for i in range(test_data.shape[0]):
#         # access testing data
#         input_size = test_data[:, [1]][i][0] # [1] corresponds to the 'input size'
#         if input_size == input_sizes[j]:
#             if prd[i][0] < variant_runtime:
#                 variant_runtime = prd[i][0]
#                 truth = test_data[:, [0]][i][0]
#                 variant_schedule = test_x[i]
#     f.write(str(input_sizes[j])+',')
#     f.write(str(variant_runtime)+',')
#     f.write(str(truth)+',')
#     for i in range(len(variant_schedule[1:-1])):
#         f.write(str(variant_schedule[1:-1][i])+',')
#     # f.write('gpu-Quadro') # TODO: read it from filename *_out.csv
#     f.write('cpu-I5') # TODO: read it from filename *_out.csv
#     f.write('\n')
# f.close()

# TODO: show example selections
# input-1, []
# ...
# then ask the user which input do you want to tune
# because NN+C can generalize, it might work for any unforeseen input sizes

# inference_start = time.clock()
# prd = sess.run(prediction, feed_dict={x: test_xs})
# inference_end = time.clock()

# variant-selection using the test data

# should show best variant for each input size - maybe use the code above?
variant_runtime = sys.maxsize
for i in range(test_data.shape[0]):
    if prd[i][0] < variant_runtime:
        variant_runtime = prd[i][0]
        variant_schedule = test_x[i]
f.close()

print('The selected variant for input dimension(s)', input_dim, 'is', variant_schedule[1:]) # -1 excludes the last one, i.e. if there is cons, use -1
# print('Predicted runtime is', variant_runtime)
# print('Inference time:', (inference_end - inference_start) / test_data.shape[0])

