#include "Decipher.h"

Decipher::Decipher(std::string message, std::string key):
	m_message{ message }, m_key{ key }
{
}

void Decipher::setMessage(std::string message)
{
	m_message = message;
}

void Decipher::setKey(std::string key)
{
	m_key = key;
}

std::string Decipher::getMessage()
{
	return m_message;
}

std::string Decipher::getKey()
{
	return m_key;
}

void Decipher::decryptMessage(std::string encryptedMessage)
{
	std::string decryptedMessage;
	int numberOfLines = encryptedMessage.length() / m_key.length();
	std::vector<std::vector<char>> decryptMatrix(numberOfLines, std::vector<char>(m_key.length()));

	std::string order;
	order = getKeysIndexes();

	while (order.length() != 0)
	{
		
		std::string messagePart = encryptedMessage.substr(0, decryptMatrix.size());
		std::vector<char> messagePartV = stringToCharVector(messagePart);
		char c = order[0];
		int position = c - '0';
		
			for (int index = 0; index < messagePart.length(); index++)
			{
				decryptMatrix[index][position] = messagePartV[index];
			}
		
		order.erase(0, 1);
		
		encryptedMessage.erase(0, decryptMatrix.size());
	}

	for (int line = 0; line < decryptMatrix.size(); line++)
	{
		for (int column = 0; column < decryptMatrix[line].size(); column++)
		{
			decryptedMessage += decryptMatrix[line][column];
		}
	}
	setMessage(decryptedMessage);
	
}

std::string Decipher::getKeysIndexes()
{
	int currentIndex = 0;
	std::string orderedKey = m_key;
	std::string indexes = m_key;
	std::sort(orderedKey.begin(), orderedKey.end());
	for (char c : orderedKey)
	{
		std::string indexFormat;
		indexFormat = std::to_string(currentIndex);
		std::size_t found = m_key.find(c);
		indexes.replace(found, 1, indexFormat);
		currentIndex++;
	}

	return indexes;
}

std::vector<char> Decipher::stringToCharVector(std::string word)
{
	std::vector<char> wordVector;
	for (char c : word)
	{
		wordVector.push_back(c);
	}

	return wordVector;
}


