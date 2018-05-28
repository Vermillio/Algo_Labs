#pragma once

template<class T>
class KeyValuePair {
public:
	int key;
	T data;
	KeyValuePair() : key(-1) {};
	KeyValuePair(int _key, const T &_data) : key(_key), data(_data) {};

	bool operator==(const KeyValuePair<T> &x) {
		return data == x.data;
	}
	bool operator>(const KeyValuePair<T> &x) {
		return data > x.data;
	}
	bool operator<(const KeyValuePair<T> &x) {
		return data < x.data;
	}
	bool operator>=(const KeyValuePair<T> &x) {
		return data >= x.data;
	}
	bool operator<=(const KeyValuePair<T> &x) {
		return data <= x.data;
	}
};