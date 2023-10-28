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

coldFre = {'beta': ['β=0.5', 'β=1.0', 'β=1.5'],
           'My': [my_05_df['coldFre'].mean(), my_10_df['coldFre'].mean(), my_15_df['coldFre'].mean()],
           'LRU': [lru_05_df['coldFre'].mean(), lru_10_df['coldFre'].mean(), lru_15_df['coldFre'].mean()],
           'HIST': [hist_05_df['coldFre'].mean(), hist_10_df['coldFre'].mean(), hist_15_df['coldFre'].mean()],
           'FC': [fc_05_df['coldFre'].mean(), fc_10_df['coldFre'].mean(), fc_15_df['coldFre'].mean()]}
coldFre = pd.DataFrame(coldFre)


beta = ['β=0.5', 'β=1.0', 'β=1.5']
color = ['#E18727', '#21854F', '#BD3C29', '#0172B6']

fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(beta))
width = 0.08
ax.bar(x-3*width/2, coldFre.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, coldFre.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, coldFre.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, coldFre.iloc[:, 4], width, label='FC', color=color[3])
# ax.set_xlabel('', fontsize=12)
ax.set_ylabel('Cold-start Frequency', fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(beta, fontsize=font_size)
# for bars in ax.containers:
#     ax.bar_label(bars, fmt='%.2f', fontsize=8)
ax.legend(bbox_to_anchor=(0.30, 1.00), loc=2, ncol=4, fontsize=2*font_size/3)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.yticks(fontsize=font_size)
plt.savefig('figures/' + exp_type + '_coldFre.png', bbox_inches='tight', dpi=500)
# plt.show()





fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
x = np.arange(len(beta))
width = 0.08
ax.bar(x-3*width/2, coldFre.iloc[:, 1], width, label='pCache', color=color[0])
ax.bar(x-width/2, coldFre.iloc[:, 2], width, label='LRU', color=color[1])
# ax.bar(x+width/2, coldFre.iloc[:, 3], width, label='HIST', color=color[2])
ax.bar(x+width/2, coldFre.iloc[:, 4], width, label='FC', color=color[3])
# ax.set_xlabel('', fontsize=12)
ax.set_ylabel('Cold-start Frequency', fontsize=font_size)
ax.set_xticks(x)
ax.set_xticklabels(beta, fontsize=font_size)
for bars in ax.containers:
    ax.bar_label(bars, fmt='%.2f', fontsize=font_size)
ax.legend(bbox_to_anchor=(0.30, 1.00), loc=2, ncol=4, fontsize=2*font_size/3)
ax.grid(axis='y', linestyle='--', linewidth=0.4)
plt.yticks(fontsize=font_size)
plt.savefig('figures/' + exp_type + '_coldFre_with_num.png', bbox_inches='tight', dpi=500)
# plt.show()

