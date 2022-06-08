#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Cipher
{
private:
	std::string m_message;
	std::string m_key;

public:
	Cipher()=default;
	Cipher(std::string message, std::string key);

	void setMessage(std::string message);
	void setKey(std::string key);

	std::string getMessage();
	std::string getKey();
	
	std::vector<std::vector<char>> createTraspositionMatrix();
	std::vector<char> stringToCharVector(std::string word);

	bool verifyMessagesSizeToFitKey();
	void adjustMessageSizeToFitTranspositionMatrix();

	void displayTranspositionMatrix(std::vector<std::vector<char>> matrix);
	std::string getEncryptedMessage();
	std::string getKeysIndexes();
	void decryptMessage(std::string encryptedMessage);

	~Cipher() = default;
};

