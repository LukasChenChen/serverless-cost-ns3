import numpy as np
import pandas as pd
import tools
from matplotlib.pylab import plt

exp_type = 'simulation'
font_size = 24

my_path_05 = 'Data/' + exp_type + '/my-result-0.5-10..csv'
my_path_10 = 'Data/' + exp_type + '/my-result-1-1.csv'
my_path_15 = 'Data/' + exp_type + '/my-result-1.5-10..csv'

my_05_df = tools.data_analysis(my_path_05)
my_10_df = tools.data_analysis(my_path_10)
my_15_df = tools.data_analysis(my_path_15)


lru_path_05 = 'Data/' + exp_type + '/lru-0.5-10..csv'
lru_path_10 = 'Data/' + exp_type + '/lru-1-1.csv'
lru_path_15 = 'Data/' + exp_type + '/lru-1.5-10..csv'

lru_05_df = tools.data_analysis(lru_path_05)
lru_10_df = tools.data_analysis(lru_path_10)
lru_15_df = tools.data_analysis(lru_path_15)



hist_path_05 = 'Data/' + exp_type + '/hist-result-0.5-10..csv'
hist_path_10 = 'Data/' + exp_type + '/hist-result-1-1.csv'
hist_path_15 = 'Data/' + exp_type + '/hist-result-1.5-10..csv'

hist_05_df = tools.data_analysis(hist_path_05)
hist_10_df = tools.data_analysis(hist_path_10)
hist_15_df = tools.data_analysis(hist_path_15)



fc_path_05 = 'Data/' + exp_type + '/fc-result-0.5-10..csv'
fc_path_10 = 'Data/' + exp_type + '/fc-result-1-1.csv'
fc_path_15 = 'Data/' + exp_type + '/fc-result-1.5-10..csv'

fc_05_df = tools.data_analysis(fc_path_05)
fc_10_df = tools.data_analysis(fc_path_10)
fc_15_df = tools.data_analysis(fc_path_15)

b_05_avgcost = {'Alpha': my_05_df['Alpha'].tolist(),
                'My': my_05_df['avgCost'].tolist(),
                'LRU': lru_05_df['avgCost'].tolist(),
                'HIST': hist_05_df['avgCost'].tolist(),
                'FC': fc_05_df['avgCost'].tolist()}
b_05_avgcost = pd.DataFrame(b_05_avgcost)
b_05_avgcost_max = b_05_avgcost.to_numpy().max()


b_10_avgcost = {'Alpha': my_10_df['Alpha'].tolist(),
                'My': my_10_df['avgCost'].tolist(),
                'LRU': lru_10_df['avgCost'].tolist(),
                'HIST': hist_10_df['avgCost'].tolist(),
                'FC': fc_10_df['avgCost'].tolist()}
b_10_avgcost = pd.DataFrame(b_10_avgcost)
b_10_avgcost_max = b_10_avgcost.to_numpy().max()


b_15_avgcost = {'Alpha': my_15_df['Alpha'].tolist(),
                'My': my_15_df['avgCost'].tolist(),
                'LRU': lru_15_df['avgCost'].tolist(),
                'HIST': hist_15_df['avgCost'].tolist(),
                'FC': fc_15_df['avgCost'].tolist()}
b_15_avgcost = pd.DataFrame(b_15_avgcost)
b_15_avgcost_max = b_15_avgcost.to_numpy().max()

all_b_max = [b_05_avgcost_max, b_10_avgcost_max, b_15_avgcost_max]
max = max(all_b_max)

b_05_avgcost = b_05_avgcost / max
b_10_avgcost = b_10_avgcost / max
b_15_avgcost = b_15_avgcost / max


alp = ['⍺=0.001', '⍺=0.002', '⍺=0.005', '⍺=0.010', '⍺=0.015']
color = ['#E18727', '#21854F', '#BD3C29', '#0172B6']

# β = 0.5
fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(alp))
width = 0.130
ax.bar(x-3*width/2, b_05_avgcost.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, b_05_avgcost.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, b_05_avgcost.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, b_05_avgcost.iloc[:, 4], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
# ax.set_xlabel('β=0.5', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=font_size)
plt.yticks(fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=font_size)
# for bars in ax.containers:
#     ax.bar_label(bars, fmt='%.2f', fontsize=8)
ax.legend(bbox_to_anchor=(0.05, 1.00), loc=2, ncol=4, fontsize=font_size)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/' + exp_type + '_avgCost_β0.5.png', bbox_inches='tight', dpi=500)
# plt.show()


# β = 1.0
fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(alp))
width = 0.130
ax.bar(x-3*width/2, b_10_avgcost.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, b_10_avgcost.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, b_10_avgcost.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, b_10_avgcost.iloc[:, 4], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
# ax.set_xlabel('β=1.0', fontsize=16)
ax.set_ylabel('Normalised Average Cost', fontsize=font_size)
plt.yticks(fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=font_size)
# for bars in ax.containers:
#     ax.bar_label(bars, fmt='%.2f', fontsize=8)
ax.legend(bbox_to_anchor=(0.05, 1.00), loc=2, ncol=4, fontsize=font_size)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/' + exp_type + '_avgCost_β1.0.png', bbox_inches='tight', dpi=500)
# plt.show()


# β = 1.5
fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(alp))
width = 0.130
ax.bar(x-3*width/2, b_15_avgcost.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, b_15_avgcost.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, b_15_avgcost.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, b_15_avgcost.iloc[:, 4], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
# ax.set_xlabel('β=1.5', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=font_size)
plt.yticks(fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=font_size)
# for bars in ax.containers:
#     ax.bar_label(bars, fmt='%.2f', fontsize=8)
ax.legend(bbox_to_anchor=(0.05, 1.00), loc=2, ncol=4, fontsize=font_size)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/' + exp_type + '_avgCost_β1.5.png', bbox_inches='tight', dpi=500)
# plt.show()



# ================================================

# β = 0.5
fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(alp))
width = 0.130
ax.bar(x-3*width/2, b_05_avgcost.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, b_05_avgcost.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, b_05_avgcost.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, b_05_avgcost.iloc[:, 4], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
# ax.set_xlabel('β=0.5', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=font_size)
for bars in ax.containers:
    ax.bar_label(bars, fmt='%.2f', fontsize=font_size)
ax.legend(bbox_to_anchor=(0.05, 1.00), loc=2, ncol=4, fontsize=font_size)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/' + exp_type + '_avgCost_β0.5_with_num.png', bbox_inches='tight', dpi=500)
# plt.show()


# β = 1.0
fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(alp))
width = 0.130
ax.bar(x-3*width/2, b_10_avgcost.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, b_10_avgcost.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, b_10_avgcost.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, b_10_avgcost.iloc[:, 4], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
# ax.set_xlabel('β=1.0', fontsize=16)
ax.set_ylabel('Normalised Average Cost', fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=font_size)
for bars in ax.containers:
    ax.bar_label(bars, fmt='%.2f', fontsize=font_size)
ax.legend(bbox_to_anchor=(0.05, 1.00), loc=2, ncol=4, fontsize=font_size)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/' + exp_type + '_avgCost_β1.0_with_num.png', bbox_inches='tight', dpi=500)
# plt.show()


# β = 1.5
fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(alp))
width = 0.130
ax.bar(x-3*width/2, b_15_avgcost.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, b_15_avgcost.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, b_15_avgcost.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, b_15_avgcost.iloc[:, 4], width, label='FC', color=color[3])
ax.set_ylim((0, 1))
# ax.set_xlabel('β=1.5', fontsize=12)
ax.set_ylabel('Normalised Average Cost', fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(alp, fontsize=font_size)
for bars in ax.containers:
    ax.bar_label(bars, fmt='%.2f', fontsize=font_size)
ax.legend(bbox_to_anchor=(0.05, 1.00), loc=2, ncol=4, fontsize=font_size)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.savefig('figures/' + exp_type + '_avgCost_β1.5_with_num.png', bbox_inches='tight', dpi=500)
# plt.show()
