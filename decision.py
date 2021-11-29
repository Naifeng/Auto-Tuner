import csv
import sys

input_dim = []
for i in range(1, len(sys.argv)):
    input_dim.append(int(sys.argv[i]))

with open('best_candidates.csv', newline='') as f:
	reader = csv.reader(f)
	data = list(reader)

variant_runtime = sys.maxsize
for j in range(len(data)):
	row = data[j]
	# print(j, row)
	if float(row[1]) < variant_runtime:
		# each row in the csv file: [input_size, pred_runtime, [schedule], platform]
		variant_runtime = float(row[1])
		variant_schedule = row[len(input_dim)+1:-1] 
		# print(type(variant_schedule[0]))
		variant_schedule = [int(float(k)) for k in variant_schedule]
		variant_platform = row[-1]

print("\nThe predicted best variant for input size", input_dim, "is")
print(variant_platform, "-", variant_schedule)
print(" ")

# print(data)

