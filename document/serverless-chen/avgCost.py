#%%
import pandas as pd
from matplotlib.pylab import plt
import numpy as np

#%%
beta_x5 = {'My': [11.4767, 14.1671, 16.7752, 20.0938, 22.0988],
           'LRU': [24.4080, 27.4249, 30.4671, 33.4973, 36.5126],
           'HIST': [24.4275, 27.4093, 30.4159, 33.4138, 36.3907],
           'FC': [27.2753, 30.2253, 33.1811, 36.1386, 39.1225]}
beta_x5 = pd.DataFrame(beta_x5)
beta_x5_max = beta_x5.to_numpy().max()
# beta_x5 = beta_x5 / beta_x5.to_numpy().max()


beta_1 = {'My': [14.9543, 18.0694, 21.2551, 24.2041, 28.8257],
          'LRU': [24.8430, 28.5589, 32.2764, 35.9904, 39.7119],
          'HIST': [25.1993, 28.8717, 32.5457, 36.2166, 39.8978],
          'FC': [29.3441, 32.8328, 36.3336, 39.8255, 43.3277]}
beta_1 = pd.DataFrame(beta_1)
beta_1_max = beta_1.to_numpy().max()
# beta_1 = beta_1 / beta_1.to_numpy().max()


beta_15 = {'My': [9.9423, 13.4877, 17.7322, 21.6508, 25.5548],
           'LRU': [13.2845, 17.3623, 21.4386, 25.5172, 29.5938],
           'HIST': [14.1461, 18.2305, 22.3133, 26.3984, 30.4819],
           'FC': [21.5942, 25.5793, 29.5640, 33.5500, 37.5349]}
beta_15 = pd.DataFrame(beta_15)
beta_15_max = beta_15.to_numpy().max()
# beta_15 = beta_15 / beta_15.to_numpy().max()

all_max = [beta_x5_max, beta_1_max, beta_15_max]
max = max(all_max)

beta_x5 = beta_x5 / max
beta_1 = beta_1 / max
beta_15 = beta_15 / max


alp = ['⍺=0.01', '⍺=0.02', '⍺=0.03', '⍺=0.04', '⍺=0.05']

#%%

# β = 0.5
fig, ax = plt.subplots(dpi=500)
x = np.arange(len(alp))
color = ['tab:blue', 'tab:green', 'tab:red', 'tab:orange']
width = 0.130
ax.bar(x-3*width/2, beta_x5.iloc[:, 0], width, label='My', color=color[0])
ax.bar(x-width/2, beta_x5.iloc[:, 1], width, label='LRU', color=color[1])
ax.bar(x+width/2, beta_x5.iloc[:, 2], width, label='HIST', color=color[2])
ax.bar(x+3*width/2, beta_x5.iloc[:, 3], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
ax.set_xlabel('β=0.5', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=12)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=12)
ax.legend(bbox_to_anchor=(0.10, 1.00), loc=2, ncol=4, fontsize=12)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/avgCost_β0.5.png', bbox_inches='tight', dpi=500)
plt.show()


# %%
# β = 1.0
fig, ax = plt.subplots(dpi=500)
x = np.arange(len(alp))
color = ['tab:blue', 'tab:green', 'tab:red', 'tab:orange']
width = 0.130
ax.bar(x-3*width/2, beta_1.iloc[:, 0], width, label='My', color=color[0])
ax.bar(x-width/2, beta_1.iloc[:, 1], width, label='LRU', color=color[1])
ax.bar(x+width/2, beta_1.iloc[:, 2], width, label='HIST', color=color[2])
ax.bar(x+3*width/2, beta_1.iloc[:, 3], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
ax.set_xlabel('β=1.0', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=12)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=12)
ax.legend(bbox_to_anchor=(0.10, 1.00), loc=2, ncol=4, fontsize=12)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/avgCost_β1.0.png', bbox_inches='tight', dpi=500)
plt.show()


#%%
# β = 1.5
fig, ax = plt.subplots(dpi=500)
x = np.arange(len(alp))
color = ['tab:blue', 'tab:green', 'tab:red', 'tab:orange']
width = 0.130
ax.bar(x-3*width/2, beta_15.iloc[:, 0], width, label='My', color=color[0])
ax.bar(x-width/2, beta_15.iloc[:, 1], width, label='LRU', color=color[1])
ax.bar(x+width/2, beta_15.iloc[:, 2], width, label='HIST', color=color[2])
ax.bar(x+3*width/2, beta_15.iloc[:, 3], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
ax.set_xlabel('β=1.5', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=12)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=12)
ax.legend(bbox_to_anchor=(0.10, 1.00), loc=2, ncol=4, fontsize=12)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/avgCost_β1.5.png', bbox_inches='tight', dpi=500)
plt.show()

# %%
