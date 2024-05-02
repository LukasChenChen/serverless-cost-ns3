import pandas as pd
from matplotlib.pylab import plt
import numpy as np


colFre = {'My': [0.221803, 0.275836, 0.12121],
          'LRU': [0.558778, 0.498463, 0.190078],
          'HIST': [0.573154, 0.518785, 0.209314],
          'FC': [0.709795, 0.649958, 0.366287]}
colFre = pd.DataFrame(colFre)

beta = ['β=0.5', 'β=1.0', 'β=1.5']

fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(beta))
color = ['tab:blue', 'tab:green', 'tab:red', 'tab:orange']
width = 0.08
ax.bar(x-3*width/2, colFre.iloc[:, 0], width, label='My', color=color[0])
ax.bar(x-width/2, colFre.iloc[:, 1], width, label='LRU', color=color[1])
ax.bar(x+width/2, colFre.iloc[:, 2], width, label='HIST', color=color[2])
ax.bar(x+3*width/2, colFre.iloc[:, 3], width, label='FC', color=color[3])
# ax.set_xlabel('', fontsize=12)
ax.set_ylabel('Cold Frequency', fontsize=12)
ax.set_xticks(x)
ax.set_xticklabels(beta, fontsize=12)
ax.legend(bbox_to_anchor=(0.30, 1.00), loc=2, ncol=4, fontsize=12)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/coldFre.png', bbox_inches='tight', dpi=500)
plt.show(bbox_inches='tight')
