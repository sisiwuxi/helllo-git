import pandas as pd
#%mathplotlib inline

df = pd.read_csv('1.csv')
print(df.head(3))
#df.columns = {'Index','Name','Gender','Math','English','Chinese','Physical','Chemical','Biology'}
#print(df.head(5))
# draw histogram for math
df.math.plot(kind='kde')
# draw histogram for all
#df.plot(kind='kde')
