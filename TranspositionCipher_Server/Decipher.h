#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Decipher
{
private:
	std::string m_message;
	std::string m_key;
public:
	Decipher() = default;
	Decipher(std::string message, std::string key);

	void setMessage(std::string message);
	void setKey(std::string key);

	std::string getMessage();
	std::string getKey();

	void decryptMessage(std::string encryptedMessage);
	std::string getKeysIndexes();
	std::vector<char> stringToCharVector(std::string word);


	~Decipher()=default;
};

