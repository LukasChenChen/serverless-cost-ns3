import numpy as np
import pandas as pd
from matplotlib.pylab import plt
from ast import literal_eval




def data_analysis(data_path):
    df = pd.read_table(data_path, low_memory=False)

    alpha_df = df[df.iloc[:, 0].str.contains('Alpha', regex=False, case=False, na=False)]
    alpha_df = alpha_df.iloc[:, 0].str.split(',').tolist()
    alpha_df = np.array(alpha_df)
    alpha_df = pd.DataFrame(alpha_df[:, 1])
    alpha_df.columns = ['Alpha']

    avg_df = df[df.iloc[:, 0].str.contains('avgCost', regex=False, case=False, na=False)]

    column_name = avg_df.iloc[0, :]
    column_name = column_name.str.split(',').tolist()
    column_name = column_name[0]

    cost_df = df.iloc[avg_df.index + 1, :]
    cost_df = cost_df.iloc[:, 0].str.split(',').tolist()
    cost_df = pd.DataFrame(cost_df)
    cost_df.columns = column_name

    avg_cost_df = pd.concat([alpha_df, cost_df], axis=1, join='inner')
    for i in avg_cost_df.columns:
        avg_cost_df[i] = avg_cost_df[i].astype(float)

    unique_alpha = np.unique(avg_cost_df['Alpha'].values)

    coldFre = []
    avgCost = []
    for i in range(len(unique_alpha)):
        temp_avg_cost_df = avg_cost_df[avg_cost_df['Alpha'] == unique_alpha[i]]
        coldFre.append(temp_avg_cost_df['coldfreq'].mean())
        avgCost.append(temp_avg_cost_df['avgcost'].mean())

    result_df = {'Alpha': unique_alpha,
                 'coldFre': coldFre,
                 'avgCost': avgCost}

    result_df = pd.DataFrame(result_df)

    return result_df






def data_analysis_for_box(data_path):
    df = pd.read_table(data_path, low_memory=False, index_col=None)
    df = df[df[df.columns[0]].str.contains('avgcost') == False]
    df = df.iloc[13:-3, 0].str.split(',')
    df = pd.DataFrame(df)
    df = df.reset_index()
    df['singleCost'] = df[df.columns[1]]
    # print(df)
    new_df = pd.DataFrame(df['singleCost'])
    cost = [float(row['singleCost'][7]) for idx, row in new_df.iterrows()]
    # for idx, row in new_df.iterrows():
    #      if len(row['singleCost']) == 8:
    #         # print(float(row['singleCost'][7]))
    #         cost.append(float(row['singleCost'][7]))
    # print(cost)
    # print(cost)
    if  0 in np.array(cost):
        print("find 0")
    
    return np.array(cost)


def test_data_analysis_for_box(data_path):
    df = pd.read_table(data_path, low_memory=False, index_col=None)
    #temp_df = df.loc[df[df.columns[0]].between('Alpha,0.001', 'Alpha,0.005')]
    temp_df = df.loc[df[df.columns[0]].between('Alpha,0.040','Alpha,fake')]
    df = df.loc[temp_df.index[0]:temp_df.index[1]]
    df = df.reset_index()
    df = df.iloc[6:-13, 1]
    df = pd.DataFrame(df)
    df = df[df.columns[0]].str.split(',')
    df = df.reset_index()
    df['singleCost'] = df[df.columns[1]]
    # print(df)
    new_df = pd.DataFrame(df['singleCost'])
    cost = [float(row['singleCost'][6]) for idx, row in new_df.iterrows()]
    # for idx, row in new_df.iterrows():
    #      if len(row['singleCost']) == 8:
    #         # print(float(row['singleCost'][7]))
    #         cost.append(float(row['singleCost'][7]))
    # print(cost)
    # print(cost)
    return np.array(cost)


def data_merge(data):
    data_list = []
    for i in range(len(data)):
        data_list.append(data['singleCost'][i].values)        
    return np.array(data_list)



def define_box_properties(plot_name, color_code, label):
	for k, v in plot_name.items():
		plt.setp(plot_name.get(k), color=color_code)
		
	# use plot function to draw a small line to name the legend.
	plt.plot([], c=color_code, label=label)
	plt.legend()

#print box values
def print_box_values(plot, plt):
    #boxes = [box.get_ydata() for box in plot["boxes"]]
    medians = [median.get_ydata() for median in plot["medians"]]
    whiskers = [whiskers.get_ydata() for whiskers in plot["whiskers"]]
    
    for i, line in enumerate(plot['medians']):
        x, y = line.get_xydata()[1]
        plt.annotate("{:.9f}".format(medians[0][0]), xy=(x, y))
    
    for i, line in enumerate(plot['whiskers']):
        x, y = line.get_xydata()[1]
        
        plt.annotate("{:.9f}".format(whiskers[i][1]), xy=(x, y))
        

    print("whisker ", whiskers)
    

 

    
    

   