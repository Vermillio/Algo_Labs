#pragma once



template<class KeyClass, class ValClass>
class KeyValuePair {

protected:
	KeyClass key;
	ValClass val;
public:

	KeyValuePair(const KeyClass & t_key, const ValClass & t_val) : key(t_key), val(t_val) {};


	KeyClass getKey() const { return key; }
	ValClass getValue() const { return value; }
	void setKey(const KeyClass & t_key) { key = t_key; }
	void setValue(const ValClass & t_val) { val = t_val; }

};