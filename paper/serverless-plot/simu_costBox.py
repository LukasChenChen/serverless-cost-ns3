import numpy as np
import pandas as pd
import tools
from matplotlib.pylab import plt

exp_type = 'new_simulation'
print_values = True
is_normalized = False
font_size = 28

my_path_05 = 'Data/' + exp_type + '/my-result-0.5-10..csv'
my_path_10 = 'Data/' + exp_type + '/my-result-1-1.csv'
my_path_15 = 'Data/' + exp_type + '/my-result-1.5-10..csv'
my_05_df = tools.data_analysis_for_box(my_path_05)
my_10_df = tools.data_analysis_for_box(my_path_10)
my_15_df = tools.data_analysis_for_box(my_path_15)


lru_path_05 = 'Data/' + exp_type + '/lru-0.5-10..csv'
lru_path_10 = 'Data/' + exp_type + '/lru-1-1.csv'
lru_path_15 = 'Data/' + exp_type + '/lru-1.5-10..csv'
lru_05_df = tools.data_analysis_for_box(lru_path_05)
lru_10_df = tools.data_analysis_for_box(lru_path_10)
lru_15_df = tools.data_analysis_for_box(lru_path_15)


fc_path_05 = 'Data/' + exp_type + '/fc-result-0.5-10..csv'
fc_path_10 = 'Data/' + exp_type + '/fc-result-1-1.csv'
fc_path_15 = 'Data/' + exp_type + '/fc-result-1.5-10..csv'
fc_05_df = tools.data_analysis_for_box(fc_path_05)
fc_10_df = tools.data_analysis_for_box(fc_path_10)
fc_15_df = tools.data_analysis_for_box(fc_path_15)


b_05_avgcost = {'My': my_05_df,
                'LRU': lru_05_df,
                'FC': fc_05_df}
b_05_max = max([max(b_05_avgcost['My']), max(b_05_avgcost['LRU']), max(b_05_avgcost['FC'])])
b_05_min = min([min(b_05_avgcost['My']), min(b_05_avgcost['LRU']), min(b_05_avgcost['FC'])])


b_10_avgcost = {'My': my_10_df,
                'LRU': lru_10_df,
                'FC': fc_10_df}
b_10_max = max([max(b_10_avgcost['My']), max(b_10_avgcost['LRU']), max(b_10_avgcost['FC'])])
b_10_min = min([min(b_10_avgcost['My']), min(b_10_avgcost['LRU']), min(b_10_avgcost['FC'])])


b_15_avgcost = {'Alpha': my_15_df,
                'My': my_15_df,
                'LRU': lru_15_df,
                'FC': fc_15_df}
b_15_max = max([max(b_15_avgcost['My']), max(b_15_avgcost['LRU']), max(b_15_avgcost['FC'])])
b_15_min = min([min(b_15_avgcost['My']), min(b_15_avgcost['LRU']), min(b_15_avgcost['FC'])])


all = [b_05_min, b_05_max, b_10_max, b_10_min, b_15_min, b_15_max]
max_value = max(all)
min_value = min(all)
print("max value is %f", max_value)
print("min value is %f", min_value)

if is_normalized == False:
	max_value = 1
	min_value = 0

b_05_avgcost['My'] = (b_05_avgcost['My'] - min_value) / (max_value - min_value)
b_05_avgcost['LRU'] = (b_05_avgcost['LRU'] - min_value) / (max_value - min_value)
b_05_avgcost['FC'] = (b_05_avgcost['FC'] - min_value) / (max_value - min_value)
b_10_avgcost['My'] = (b_10_avgcost['My'] - min_value) / (max_value - min_value)
b_10_avgcost['LRU'] = (b_10_avgcost['LRU'] - min_value) / (max_value - min_value)
b_10_avgcost['FC'] = (b_10_avgcost['FC'] - min_value) / (max_value - min_value)
b_15_avgcost['My'] = (b_15_avgcost['My'] - min_value) / (max_value - min_value)
b_15_avgcost['LRU'] = (b_15_avgcost['LRU'] - min_value) / (max_value - min_value)
b_15_avgcost['FC'] = (b_15_avgcost['FC'] - min_value) / (max_value - min_value)



alp = ['⍺=0.001']


fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
my_05_plot = plt.boxplot(b_05_avgcost['My'], positions=np.array(np.arange(1))*2.0-1, 
					  widths=0.6, sym='')
lru_05_plot = plt.boxplot(b_05_avgcost['LRU'], positions=np.array(np.arange(1))*2.0, 
					  widths=0.6, sym='')
fc_05_plot = plt.boxplot(b_05_avgcost['FC'], positions=np.array(np.arange(1))*2.0+1, 
					  widths=0.6, sym='')

if print_values == True:
	# print the box values
	tools.print_box_values(my_05_plot, plt)
	tools.print_box_values(lru_05_plot, plt)
	tools.print_box_values(fc_05_plot, plt)					  

# setting colors for each groups
tools.define_box_properties(my_05_plot, '#E18727', 'pCache')
tools.define_box_properties(lru_05_plot, '#21854F', 'LRU')
tools.define_box_properties(fc_05_plot, '#0172B6', 'FC')
# set the x label values
# plt.xticks(np.arange(0, len(alp) * 2, 2), alp, fontsize=16)
#plt.xticks(visible=False)
plt.xticks([-1, 0, 1], ['pCache', 'LRU', 'FC'], fontsize=font_size)  
# set the limit for x axis
plt.xlim(-2, len(alp)*2)
# set the limit for y axis
if is_normalized == True:
    plt.ylim(-0.2, 1.2)
else:
	plt.ylim(0, 140)

plt.yticks(fontsize=font_size)
plt.ylabel("System Cost",fontsize=font_size)
# set the title
# plt.title('')
plt.legend(fontsize=font_size, loc = "upper left")
plt.legend().remove()
plt.savefig('figures/' + exp_type + '_costBox_β0.5.png', bbox_inches='tight', dpi=500)
# plt.show()




fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
my_10_plot = plt.boxplot(b_10_avgcost['My'], positions=np.array(np.arange(1))*2.0-1, 
					  widths=0.6, sym='')
lru_10_plot = plt.boxplot(b_10_avgcost['LRU'], positions=np.array(np.arange(1))*2.0, 
					  widths=0.6, sym='')
fc_10_plot = plt.boxplot(b_10_avgcost['FC'], positions=np.array(np.arange(1))*2.0+1, 
					  widths=0.6, sym='')

if print_values == True:
	# print the box values
	tools.print_box_values(my_10_plot, plt)
	tools.print_box_values(lru_10_plot, plt)
	tools.print_box_values(fc_10_plot, plt)


# setting colors for each groups
tools.define_box_properties(my_10_plot, '#E18727', 'pCache')
tools.define_box_properties(lru_10_plot, '#21854F', 'LRU')
tools.define_box_properties(fc_10_plot, '#0172B6', 'FC')
# set the x label values
# plt.xticks(np.arange(0, len(alp) * 2, 2), alp, fontsize=16)
#plt.xticks(visible=False)
plt.xticks([-1, 0, 1], ['pCache', 'LRU', 'FC'], fontsize=font_size)  
# set the limit for x axis
plt.xlim(-2, len(alp)*2)
# set the limit for y axis
if is_normalized == True:
    plt.ylim(-0.2, 1.2)
else:
	plt.ylim(0, 140)

plt.yticks(fontsize=font_size)
plt.ylabel("System Cost",fontsize=font_size)
# set the title
# plt.title('')
plt.legend(fontsize=font_size, loc = "upper left")
plt.legend().remove()
plt.savefig('figures/' + exp_type + '_costBox_β1.0.png', bbox_inches='tight', dpi=500)





fig, ax = plt.subplots(figsize=(10, 6), dpi=500)
my_15_plot = plt.boxplot(b_15_avgcost['My'], positions=np.array(np.arange(1))*2.0-1, 
					  widths=0.6, sym='')
lru_15_plot = plt.boxplot(b_15_avgcost['LRU'], positions=np.array(np.arange(1))*2.0, 
					  widths=0.6, sym='')
fc_15_plot = plt.boxplot(b_15_avgcost['FC'], positions=np.array(np.arange(1))*2.0+1, 
					  widths=0.6, sym='')

if print_values == True:
	# print the box values
	tools.print_box_values(my_15_plot, plt)
	tools.print_box_values(lru_15_plot, plt)
	tools.print_box_values(fc_15_plot, plt)


# setting colors for each groups
tools.define_box_properties(my_15_plot, '#E18727', 'pCache')
tools.define_box_properties(lru_15_plot, '#21854F', 'LRU')
tools.define_box_properties(fc_15_plot, '#0172B6', 'FC')
# set the x label values
# plt.xticks(np.arange(0, len(alp) * 2, 2), alp, fontsize=16)
#plt.xticks(visible=False)
plt.xticks([-1, 0, 1], ['pCache', 'LRU', 'FC'], fontsize=font_size)  
# set the limit for x axis
plt.xlim(-2, len(alp)*2)
if is_normalized == True:
# set the limit for y axis
    plt.ylim(-0.2, 1.2)
else:
	plt.ylim(0, 140)
plt.yticks(fontsize=font_size)
plt.ylabel("System Cost",fontsize=font_size)
# set the title
# plt.title('')
plt.legend(fontsize=font_size, loc = "upper left")
plt.legend().remove()
plt.savefig('figures/' + exp_type + '_costBox_β1.5.png', bbox_inches='tight', dpi=500)
