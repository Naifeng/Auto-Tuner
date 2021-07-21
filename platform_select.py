import csv
import sys

# TODO: read from argument
input_sizes = [1024, 2048, 4096, 8192, 16384, 32768]

with open('vs_re.csv', newline='') as f:
    reader = csv.reader(f)
    data = list(reader)

for i in range(len(input_sizes)):
	variant_runtime = sys.maxsize
	for j in range(len(data)):
		row = data[j]
		# print(j, row)
		if float(row[0]) == input_sizes[i]:
			if float(row[1]) < variant_runtime:
				variant_runtime = float(row[1])
				variant_schedule = row[2:-1] # hard-coded
				variant_platform = row[-1]
				# in the csv file
				# [input_size, pred_runtime, [schedule], platform]
	print(input_sizes[i], ':', variant_platform, variant_schedule)

# print(data)