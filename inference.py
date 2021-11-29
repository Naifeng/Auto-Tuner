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

np.set_printoptions(suppress=True)

tf.set_random_seed(1234)

# read from the command line
FILENAME = sys.argv[1] 
md5_hash = sys.argv[2]
decision = sys.argv[3]
input_dim = []
for i in range(4, (len(sys.argv)-1)):
    input_dim.append(int(sys.argv[i]))
functional = sys.argv[len(sys.argv)-1]

print('decision', decision)

data = np.array(pd.read_csv(FILENAME, header=None))

test_data = data[:] # should be [:]
# print(test_data)
# test_feature = np.array(test_data[:, 1:])

test_x = np.array(test_data[:, :])

# (len(sys.argv)-1) - 3(argv[1-3]) - 1(functional) is the number of input dimensions in given input size
input_dim_array = np.full((test_x.shape[0], len(sys.argv) - 5), input_dim) 

# append input dim to test_x
test_x = np.append(input_dim_array.astype(float), test_x, 1)

# append functional to the last

# print(functional)

if functional != "1":
    # translate to string first
    functional = ''.join("test_x[i][" + str(int(c)-1) + "]" if c.isdigit() else c for c in functional)

    # functional needs to access both input_dim and candidate
    functional_array = np.zeros(shape=(len(test_x),1))

    # for each row in test_x
    # perform operation indicated by functional
    for i in range(len(test_x)):
        # use exec() to execute the string
        exec("functional_array[i] = " + functional)

else:
    functional_array = np.ones(shape=(len(test_x),1))

# append functional to test_x
test_x = np.append(test_x, functional_array.astype(float), 1)

# print(test_x)

# read mean and var for the preprocessing scalar from a .csv file
f = open('model_config/' + md5_hash + '/' + decision + '/preprocessing.csv')

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

    saver.restore(sess, tf.train.latest_checkpoint('model_config/' + md5_hash + '/' + decision))
    
    # inference_start = time.clock()
    prd = sess.run(prediction, feed_dict={x: test_xs})
    # inference_end = time.clock()
    
    f = open('model_results/inference_predictions.txt', 'w+') 
    for i in range(test_data.shape[0]):
        f.writelines(str(prd[i][0]) + "\n")
    f.close()

variant_runtime = sys.maxsize
for i in range(test_data.shape[0]):
    if prd[i][0] < variant_runtime:
        variant_runtime = prd[i][0]
        variant_schedule = test_x[i]
f.close()

print('The selected variant for input size', input_dim, 'is', variant_schedule[len(input_dim):-1]) # -1 excludes the last one, i.e. if there is cons, use -1
# print('Predicted runtime is', variant_runtime)
# print('Inference time:', (inference_end - inference_start) / test_data.shape[0])

# write to a file to make global decisions
f = open('best_candidates.csv', 'a')
for i in range(len(input_dim)):
    f.write(str(input_dim[i])+',')
f.write(str(variant_runtime)+',')
for i in range(len(variant_schedule[len(input_dim):-1])):
    f.write(str(variant_schedule[len(input_dim):-1][i])+',')
f.write(decision) 
f.write('\n')
f.close()


