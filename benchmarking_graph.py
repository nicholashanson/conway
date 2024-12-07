import numpy as np
import matplotlib.pyplot as plt
import csv

data = []

with open('benchmarking_data.csv', newline='') as csvfile:
    reader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in reader:
        for item in row:
            data.append( int( item.rstrip(',') ) )

average_time = sum( data ) / len( data )

x = np.array([ x for x in range(1, 201) ])

plt.plot( x, data )
plt.plot( x, [ average_time for _ in range( 0, 200 ) ], label = 'average' )

plt.xlabel( 'nth iteration' )
plt.ylabel( 'execution time (microseconds)' )

plt.legend()

plt.savefig( 'performance.png' )