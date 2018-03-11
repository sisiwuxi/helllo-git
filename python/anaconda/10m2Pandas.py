# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

"""Creating a Series by passing a list of values,
letting pandas create a default integer index"""
s = pd.Series([1,3,5,np.nan,6,8])
print s

"""Creating a DataFrame by passing a numpy array, 
with a datetime index and labeled columns:"""
dates = pd.date_range('20130101', periods=6)
print dates

df = pd.DataFrame(np.random.randn(6,4), index=dates, columns=list('ABCD'))
print df

"""Creating a DataFrame by passing a dict of objects 
that can be converted to series-like."""
df2 = pd.DataFrame({ 'A' : 1.,
                    'B' : pd.Timestamp('20130102'),
                    'C' : pd.Series(1,index=list(range(4)),dtype='float32'),
                    'D' : np.array([3] * 4,dtype='int32'),
                    'E' : pd.Categorical(["test","train","test","train"]),
                    'F' : 'foo' })
print df2
"""Having specific dtypes"""
print df2.dtypes

"""See the top & bottom rows of the frame"""
print df.head()
print df.tail(3)

"""Display the index, columns, and the underlying numpy data"""
print df.index
print df.columns
print df.values

"""Describe shows a quick statistic summary of your data"""
print df.describe()

"""Transposing your data"""
print df.T

"""Sorting by an axis"""
print df.sort_index(axis=1, ascending=False)

"""Sorting by values"""
print df.sort_values(by='B')

"""Selecting a single column, which yields a Series, equivalent to df.A"""
print df['A']

"""Selecting via [], which slices the rows."""
print df[0:3]
print df['20130102':'20130104']

"""For getting a cross section using a label"""
print df.loc[dates[0]]

"""Selecting on a multi-axis by label"""
print df.loc[:,['A','B']]

"""Showing label slicing, both endpoints are included"""
print df.loc['20130102':'20130104',['A','B']]

"""Reduction in the dimensions of the returned object"""
print df.loc['20130102',['A','B']]

"""For getting a scalar value"""
print df.loc[dates[0],'A']

"""For getting fast access to a scalar (equiv to the prior method)"""
print df.at[dates[0],'A']

"""Select via the position of the passed integers"""
print df.iloc[3]

"""By integer slices, acting similar to numpy/python"""
print df.iloc[3:5,0:2]

"""By lists of integer position locations, similar to the numpy/python style"""
print df.iloc[[1,2,4],[0,2]]

"""For slicing rows explicitly"""
print df.iloc[1:3,:]

"""For slicing columns explicitly"""
print df.iloc[:,1:3]

"""For getting a value explicitly"""
print df.iloc[1,1]

"""For getting fast access to a scalar (equiv to the prior method)"""
print df.iat[1,1]

"""Using a single column’s values to select data."""
print df[df.A > 0]

"""A where operation for getting."""
print df[df > 0]

"""Using the isin() method for filtering:"""
df2 = df.copy()
df2['E'] = ['one', 'one','two','three','four','three']
print df2
print df2[df2['E'].isin(['two','four'])]

"""Setting a new column automatically aligns the data by the indexes"""
s1 = pd.Series([1,2,3,4,5,6], index=pd.date_range('20130102', periods=6))
print s1
df['F'] = s1

"""Setting values by label"""
df.at[dates[0],'A'] = 0

"""Setting values by position"""
df.iat[0,1] = 0

"""Setting by assigning with a numpy array"""
df.loc[:,'D'] = np.array([5] * len(df))

"""The result of the prior setting operations"""
print df

"""A where operation with setting."""
df2 = df.copy()
df2[df2 > 0] = -df2
print df2

"""Reindexing allows you to change/add/delete the index on a specified axis. 
This returns a copy of the data."""
df1 = df.reindex(index=dates[0:4], columns=list(df.columns) + ['E'])
df1.loc[dates[0]:dates[1],'E'] = 1
print df1

"""To drop any rows that have missing data"""
df1.dropna(how='any')

"""Filling missing data"""
df1.fillna(value=5)

"""To get the boolean mask where values are"""
pd.isnull(df1)

"""Performing a descriptive statistic"""
print df.mean()

"""Same operation on the other axis"""
print df.mean(1)

"""Operating with objects that have different dimensionality and need alignment. 
In addition, pandas automatically broadcasts along the specified dimension."""
s = pd.Series([1,3,5,np.nan,6,8], index=dates).shift(2)
print s
print df.sub(s, axis='index')

"""Applying functions to the data"""
df.apply(np.cumsum)
df.apply(lambda x: x.max() - x.min())

"""See more at Histogramming and Discretization"""
s = pd.Series(np.random.randint(0, 7, size=10))
print s
print s.value_counts()

"""Series is equipped with a set of string processing methods in the str 
attribute that make it easy to operate on each element of the array, 
as in the code snippet below. Note that pattern-matching in str generally uses 
regular expressions by default (and in some cases always uses them). 
See more at Vectorized String Methods."""
s = pd.Series(['A', 'B', 'C', 'Aaba', 'Baca', np.nan, 'CABA', 'dog', 'cat'])
print s.str.lower()

"""Concatenating pandas objects together with concat():"""
df = pd.DataFrame(np.random.randn(10, 4))
print df
pieces = [df[:3], df[3:7], df[7:]]
print pd.concat(pieces)

"""SQL style merges. See the Database style joining"""
left = pd.DataFrame({'key': ['foo', 'foo'], 'lval': [1, 2]})
right = pd.DataFrame({'key': ['foo', 'foo'], 'rval': [4, 5]})
print left
print right
print pd.merge(left, right, on='key')

"""Another example that can be given is:"""
left = pd.DataFrame({'key': ['foo', 'bar'], 'lval': [1, 2]})
right = pd.DataFrame({'key': ['foo', 'bar'], 'rval': [4, 5]})
print left
print right
print pd.merge(left, right, on='key')

"""Append rows to a dataframe. See the Appending"""
df = pd.DataFrame(np.random.randn(8, 4), columns=['A','B','C','D'])
print df
s = df.iloc[3]
print df.append(s, ignore_index=True)

"""See the Grouping section"""
df = pd.DataFrame({'A' : ['foo', 'bar', 'foo', 'bar', 'foo', 'bar', 'foo', 'foo'],
                          'B' : ['one', 'one', 'two', 'three', 'two', 'two', 'one', 'three'],
                          'C' : np.random.randn(8),
                          'D' : np.random.randn(8)})
print df

"""Grouping and then applying a function sum to the resulting groups."""
print df.groupby('A').sum()

"""Grouping by multiple columns forms a hierarchical index, 
which we then apply the function."""
print df.groupby(['A','B']).sum()

"""See the sections on Hierarchical Indexing and Reshaping."""
tuples = list(zip(*[['bar', 'bar', 'baz', 'baz', 'foo', 'foo', 'qux', 'qux'],
                     ['one', 'two', 'one', 'two','one', 'two', 'one', 'two']]))
index = pd.MultiIndex.from_tuples(tuples, names=['first', 'second'])
df = pd.DataFrame(np.random.randn(8, 2), index=index, columns=['A', 'B'])
df2 = df[:4]
print df2

"""The stack() method “compresses” a level in the DataFrame’s columns."""
stacked = df2.stack()
print stacked

"""With a “stacked” DataFrame or Series (having a MultiIndex as the index), 
the inverse operation of stack() is unstack(),
which by default unstacks the last level:"""
print stacked.unstack()
print stacked.unstack(1)
print stacked.unstack(0)

"""See the section on Pivot Tables."""
df = pd.DataFrame({'A' : ['one', 'one', 'two', 'three'] * 3,
                   'B' : ['A', 'B', 'C'] * 4,
                   'C' : ['foo', 'foo', 'foo', 'bar', 'bar', 'bar'] * 2,
                   'D' : np.random.randn(12),
                   'E' : np.random.randn(12)})
print df

"""We can produce pivot tables from this data very easily:"""
print pd.pivot_table(df, values='D', index=['A', 'B'], columns=['C'])

"""pandas has simple, powerful, and efficient functionality for performing 
resampling operations during frequency conversion (e.g., converting secondly 
data into 5-minutely data). This is extremely common in, but not limited to, 
financial applications. See the Time Series section"""
rng = pd.date_range('1/1/2012', periods=100, freq='S')
ts = pd.Series(np.random.randint(0, 500, len(rng)), index=rng)
print ts.resample('5Min').sum()

"""Time zone representation"""
rng = pd.date_range('3/6/2012 00:00', periods=5, freq='D')
ts = pd.Series(np.random.randn(len(rng)), rng)
print ts

ts_utc = ts.tz_localize('UTC')
print ts_utc

"""Convert to another time zone"""
print ts_utc.tz_convert('US/Eastern')

"""Converting between time span representations"""
rng = pd.date_range('1/1/2012', periods=5, freq='M')
ts = pd.Series(np.random.randn(len(rng)), index=rng)
print ts
ps = ts.to_period()
print ps
print ps.to_timestamp()

"""Converting between period and timestamp enables some convenient arithmetic 
functions to be used. In the following example, we convert a quarterly frequency 
with year ending in November to 9am of the end of the month following the quarter 
end:"""
prng = pd.period_range('1990Q1', '2000Q4', freq='Q-NOV')
ts = pd.Series(np.random.randn(len(prng)), prng)
ts.index = (prng.asfreq('M', 'e') + 1).asfreq('H', 's') + 9
print ts.head()

"""Since version 0.15, pandas can include categorical data in a DataFrame. 
For full docs, see the categorical introductionand the API documentation."""
df = pd.DataFrame({"id":[1,2,3,4,5,6], "raw_grade":['a', 'b', 'b', 'a', 'a', 'e']})
"""Convert the raw grades to a categorical data type."""
df["grade"] = df["raw_grade"].astype("category")
print df["grade"]

"""Rename the categories to more meaningful names (assigning to Series.cat.categories 
is inplace!)"""
df["grade"].cat.categories = ["very good", "good", "very bad"]
"""Reorder the categories and simultaneously add the missing categories (methods 
under Series .cat return a newSeries per default)."""
df["grade"] = df["grade"].cat.set_categories(["very bad", "bad", "medium", "good", "very good"])
print df["grade"]

"""Sorting is per order in the categories, not lexical order."""
print df.sort_values(by="grade")

"""Grouping by a categorical column shows also empty categories."""
print df.groupby("grade").size()

"""Plotting docs."""
ts = pd.Series(np.random.randn(1000), index=pd.date_range('1/1/2000', periods=1000))
ts = ts.cumsum()
ts.plot()

"""On DataFrame, plot() is a convenience to plot all of the columns with labels:"""
df = pd.DataFrame(np.random.randn(1000, 4), index=ts.index,columns=['A', 'B', 'C', 'D'])
df = df.cumsum()
plt.figure(); df.plot(); plt.legend(loc='best')

"""Writing to a csv file"""
df.to_csv('foo.csv')
"""Reading from a csv file"""
pd.read_csv('foo.csv')

"""Writing to a HDF5 Store"""
df.to_hdf('foo.h5','df')
"""Reading from a HDF5 Store"""
pd.read_hdf('foo.h5','df')

"""Writing to an excel file"""
df.to_excel('foo.xlsx', sheet_name='Sheet1')
"""Reading from an excel file"""
pd.read_excel('foo.xlsx', 'Sheet1', index_col=None, na_values=['NA'])

"""If you are trying an operation and you see an exception like:"""
if pd.Series([False, True, False]):
    print("I was true")