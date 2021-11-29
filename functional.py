# example 
# input: 1*2
# output: let functional_param=${all_params[0]}*${all_params[1]}

import sys

functional = sys.argv[1]

# print(functional)

functional = ''.join("${all_params[" + str(int(c)-1) + "]}" if c.isdigit() else c for c in functional)

# print(functional)

print("let functional_param=" + functional)