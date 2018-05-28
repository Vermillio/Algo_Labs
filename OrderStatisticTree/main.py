import pandas as pd

data=pd.read_csv("movie_metadata_new.csv")

data_new=data [ ['movie_title', 'movie_imdb_link', 'num_voted_users', 'imdb_score', 'duration', 'gross', 'content_rating', 'budget', 'title_year'] ]


for idx, row in data_new.iterrows():
    for char in row['movie_title']:
        if char in ",":
            data_new.loc[idx, 'movie_title']=data_new.loc[idx, 'movie_title'].replace(char, '')

data_new.to_csv("movie_metadata_new.csv")
