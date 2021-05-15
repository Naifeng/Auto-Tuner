import pandas as pd
import sys

candidate_csv = 'candidate_set.csv'
profile_csv = sys.argv[1]
# print(type(profile_csv))

candidate_df = pd.read_csv(candidate_csv, header=None)
candidate_df[len(candidate_df.columns)] = 900 # hard-coded
candidate_df.insert(0, 'input_size', 30) # hard-coded
candidate_df.insert(0, 'runtime', 800) # hard-coded
# will look like
# [800 30 p1 p2 900]

profile_df = pd.read_csv(profile_csv, header=None)
# column names need to match
profile_df.columns = ['runtime', 'input_size', 0, 1, 2] 
# print(candidate_df.dtypes)
# print(profile_df.dtypes)

out_df = pd.concat([profile_df, candidate_df], axis=0)
out_df.to_csv('out.csv', index=False, header=None)