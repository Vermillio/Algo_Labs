#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cstdlib>
#include <iostream>

using std::vector;
using std::string;
using std::shared_ptr;
using std::to_string;
using std::cout;
using std::cin;

enum MovieFeatures {
	title,
	num_voted_users,
	imdb_score,
	duration,
	gross,
	budget,
	year
};

class Movie
{
protected:
	string title;
	string imdb_link;
	int num_voted_users;
	float imdb_score;
	int duration;
	int gross;
	string content_rating;
	int budget;
	int year;

	bool (Movie::*less)(const Movie &);
	bool (Movie::*equal)(const Movie &);

	bool less_title(const Movie &x) {
		return title.compare(x.title)<0;
	}
	bool less_num_voted_users(const Movie &x) {
		return num_voted_users < x.num_voted_users;
	}
	bool less_imdb_score(const Movie &x) {
		return imdb_score < x.imdb_score;
	}
	bool less_duration(const Movie &x) {
		return duration < x.duration;
	}
	bool less_gross(const Movie &x) {
		return gross < x.gross;
	}
	bool less_budget(const Movie &x) {
		return budget < x.budget;
	}
	bool less_year(const Movie &x) {
		return year < x.year;
	}

	bool eq_title(const Movie &x) {
		return title.compare(x.title) == 0;
	}
	bool eq_num_voted_users(const Movie &x) {
		return num_voted_users == x.num_voted_users;
	}
	bool eq_imdb_score(const Movie &x) {
		return imdb_score == x.imdb_score;
	}
	bool eq_duration(const Movie &x) {
		return duration == x.duration;
	}
	bool eq_gross(const Movie &x) {
		return gross == x.gross;
	}
	bool eq_budget(const Movie &x) {
		return budget == x.budget;
	}
	bool eq_year(const Movie &x) {
		return year == x.year;
	}

public:
	Movie() {};

	Movie(const string & _title, const string & _imdb_link, int _num_voted_users,
		float _imdb_score, int _duration, int _gross, const string &_rating, int _budget, int _year)
		: title(_title),
		imdb_link(_imdb_link),
		num_voted_users(_num_voted_users),
		imdb_score(_imdb_score),
		duration(_duration),
		gross(_gross),
		content_rating(_rating),
		budget(_budget),
		year(_year)
	{
		cmp_by_imdb_score();
	}

	~Movie() {};

	string getTitle() const { return title; }
	int getNumVotedUsers() const { return num_voted_users; }
	float getIMDBScore() const { return imdb_score; }
	int getDuration() const { return duration; }
	int getGross() const { return gross; }
	int getYear() const { return year; }
	int getBudget() const { return budget; }

	void setSearchValue(string &data) {
		if (less == &Movie::less_title) {
			title = data;
		}
		else if (less == &Movie::less_num_voted_users) {
			num_voted_users = atoi(data.c_str());
		}
		else if (less == &Movie::less_imdb_score) {
			imdb_score = atof(data.c_str());
		}
		else if (less == &Movie::less_duration) {
			duration = atoi(data.c_str());
		}
		else if (less == &Movie::less_gross) {
			gross = atoi(data.c_str());
		}
		else if (less == &Movie::less_budget) {
			budget = atoi(data.c_str());
		}
		else if (less == &Movie::less_year) {
			year = atoi(data.c_str());
		}

	}

	bool operator< (const Movie & x) {
		return (this->*less)(x);
	}
	bool operator> (const Movie & x) {
		return !((this->*equal)(x) || (this->*less)(x));
	}
	bool operator>= (const Movie & x) {
		return !(this->*less)(x);
	}
	bool operator<= (const Movie & x) {
		return (this->*equal)(x) || (this->*less)(x);
	}
	bool operator== (const Movie & x) {
		return (this->*equal)(x);
	}
	void cmp_by_title() {
		less = &Movie::less_title;
		equal = &Movie::eq_title;
	}
	void cmp_by_num_voted_users() {
		less = &Movie::less_num_voted_users;
		equal = &Movie::eq_num_voted_users;
	}
	void cmp_by_imdb_score() {
		less = &Movie::less_imdb_score;
		equal = &Movie::eq_imdb_score;
	}
	void cmp_by_duration() {
		less = &Movie::less_duration;
		equal = &Movie::eq_duration;
	}
	void cmp_by_gross() {
		less = &Movie::less_gross;
		equal = &Movie::eq_gross;
	}
	void cmp_by_budget() {
		less = &Movie::less_budget;
		equal = &Movie::eq_budget;
	}
	void cmp_by_year() {
		less = &Movie::less_year;
		equal = &Movie::eq_year;
	}

	void cmp_by(MovieFeatures feature) {
		if (feature == MovieFeatures::title)
			cmp_by_title();
		else if (feature == MovieFeatures::num_voted_users)
			cmp_by_num_voted_users();
		else if (feature == MovieFeatures::imdb_score)
			cmp_by_imdb_score();
		else if (feature == MovieFeatures::duration)
			cmp_by_duration();
		else if (feature == MovieFeatures::gross)
			cmp_by_gross();
		else if (feature == MovieFeatures::budget)
			cmp_by_budget();
		else if (feature == MovieFeatures::year)
			cmp_by_year();
		else if (feature == MovieFeatures::title)
			cmp_by_title();
	}

	string toString() {
		return "title: \"" + title
			+ "\" num_voted_users: " + to_string(num_voted_users)
			+ " imdb_link: \"" + imdb_link
			+ "\" imdb_score: " + to_string(imdb_score)
			+ " duration: " + to_string(duration)
			+ " content_rating: " + content_rating
			+ " gross: " + to_string(gross)
			+ " budget: " + to_string(budget)
			+ " year: " + to_string(year);
	}


	friend istream & operator>>(istream &c, Movie &m);
};

string to_string(Movie &m) {
	return m.toString();
}


istream & operator>>(istream &c, Movie &m) {
	cout << "title: ";
	cin >> m.title;
	cout << " num_voted_users: ";
	cin >> m.num_voted_users;
	cout << " imdb_link: ";
	cin >> m.imdb_link;
	cout << "\" imdb_score: ";
	cin >> m.imdb_score;
	cout << " duration: ";
	cin >> m.duration;
	cout << " content_rating: ";
	cin >> m.content_rating;
	cout << " gross: ";
	cin >> m.gross;
	cout << " budget: ";
	cin >> m.budget;
	cout << " year: ";
	cin >> m.year;
	return c;
}
