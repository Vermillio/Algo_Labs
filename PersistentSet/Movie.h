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

class Movie {
	string title;
	int voted;
public:
	string getTitle() const { return title; }
	void setTitle(string &x) { title = x; }
	
	int getVoted() const { return voted; }
	void setVoted(int x) { voted = x; }

	bool operator<(const Movie &x) const {
		return voted < x.voted;
	}
	bool operator>(const Movie &x) const {
		return voted > x.voted;
	}
	bool operator==(const Movie &x) const {
		return voted == x.voted;
	}
	bool operator!=(const Movie &x) const {
		return voted != x.voted;
	}
	bool operator<=(const Movie &x) const {
		return voted <= x.voted;
	}
	bool operator>=(const Movie &x) const {
		return voted >= x.voted;
	}


	Movie() {};

	Movie(int t_voted) : voted(t_voted) {};

	Movie(string &t_title, int t_voted) : title(t_title), voted(t_voted) {};
};

inline string to_string(const Movie &x) {
	return 	"title: " + x.getTitle() + "voted: " + to_string(x.getVoted());
}

inline std::istream & operator >> (std::istream & c, Movie & x) {
	string title;
	cout << "title: ";
	cin >> title;
	x.setTitle(title);
	int voted;
	cout << "voted: ";
	cin >> voted;
	x.setVoted(voted);
	return c;
};

inline std::ostream & operator << (std::ostream & c, const Movie & x) {
	c << to_string(x);
	return c;
};

