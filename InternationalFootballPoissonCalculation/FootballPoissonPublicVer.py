import sys
import pandas
from scipy.stats import poisson

imported_data_frame = pandas.read_csv("results.csv")

imported_data_frame['date'] = pandas.to_datetime(imported_data_frame['date'], yearfirst=True, format='%Y-%m-%d')

England_away_games_df = imported_data_frame[(imported_data_frame['away_team'] == 'England') & (imported_data_frame['neutral'] ==False)]

sorted_by_date = England_away_games_df.sort_values(by=['date'], ascending=True)

England_away_wins = sorted_by_date[sorted_by_date['away_score'] > sorted_by_date['home_score']]

years_of_football = pandas.to_datetime(imported_data_frame['date'].max(), yearfirst=True, format='%y-%m-%d').year - pandas.to_datetime(imported_data_frame['date'].min(), yearfirst=True, format='%y-%m-%d').year

avg_Eng_away_wins_per_year = England_away_wins.size / years_of_football


print("\nAverage away game wins for England during period of November 1872 to March 2023 (not including neutral fields): ")
print(avg_Eng_away_wins_per_year)
print("\n")
print("Probability that England wins 20 away games in a given year (not including neutral fields): ")
print(poisson.pmf(20, avg_Eng_away_wins_per_year))
print("\n")


England_home_games_df = imported_data_frame[(imported_data_frame['home_team'] == 'England') & (imported_data_frame['neutral'] == False)]

home_sorted_by_date = England_home_games_df.sort_values(by=['date'], ascending=True)

England_home_wins = home_sorted_by_date[home_sorted_by_date['home_score'] > home_sorted_by_date['away_score']]

avg_Eng_home_wins_per_year = England_home_wins.size / years_of_football

print("Average home game wins for England during period of November 1872 to March 2023 (not including neutral fields): ")
print(avg_Eng_home_wins_per_year)
print("\n")
print("Probability that England wins 20 home games in a given year (not including neutral fields): ")
print(poisson.pmf(20, avg_Eng_home_wins_per_year))
print("\n")

England_all_games = imported_data_frame[(imported_data_frame['home_team'] == 'England') | (imported_data_frame['away_team'] == 'England')]

all_sorted_by_date = England_all_games.sort_values(by=['date'], ascending=True)

England_all_wins = all_sorted_by_date[ ( (all_sorted_by_date['home_team'] == 'England') & (all_sorted_by_date['home_score'] > all_sorted_by_date['away_score']) ) | ( (all_sorted_by_date['away_team'] == 'England') & (all_sorted_by_date['away_score'] > all_sorted_by_date['home_score']) ) ]

avg_total_wins_per_year = England_all_wins.size / years_of_football

print("Average total wins for England during period of November 1872 to March 2023: ")
print(avg_total_wins_per_year)
print("\n")
print("Probability that England wins 40 total games in a given year: ")
print(poisson.pmf(40, avg_total_wins_per_year))




