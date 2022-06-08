#include "Cipher.h"

Cipher::Cipher(std::string message, std::string key):
	m_message{message}, m_key{key}
{
}

void Cipher::setMessage(std::string message)
{
	m_message = message;
}

void Cipher::setKey(std::string key)
{
	m_key = key;
}

std::string Cipher::getMessage()
{
	return m_message;
}

std::string Cipher::getKey()
{
	return m_key;
}

std::vector<std::vector<char>> Cipher::createTraspositionMatrix()
{
	std::vector<std::vector<char>> transpositionMatrix;
	while (m_message.length() != 0)
	{
		std::string messagePart;
		messagePart = m_message.substr(0, m_key.length());
		std::vector<char> messagePartV;
		messagePartV = stringToCharVector(messagePart);
		transpositionMatrix.emplace_back(messagePartV);
		m_message.erase(0, m_key.length());
	}

	return transpositionMatrix;
}

std::vector<char> Cipher::stringToCharVector(std::string word)
{
	std::vector<char> wordVector;
	for (char c : word)
	{
		wordVector.push_back(c);
	}

	return wordVector;
}

bool Cipher::verifyMessagesSizeToFitKey()
{
	if (m_message.length() % m_key.length() == 0)
		return true;
	return false;
}

void Cipher::adjustMessageSizeToFitTranspositionMatrix()
{
	if (verifyMessagesSizeToFitKey() == false)
	{
		int difference;
		difference = m_key.length() - m_message.length() % m_key.length();
		while (difference != 0)
		{
			m_message += " ";
			difference--;
		}
	}
}

void Cipher::displayTranspositionMatrix(std::vector<std::vector<char>> matrix)
{
	for(int line=0; line<matrix.size(); line++)
		for (int column=0; column<matrix[line].size(); column++)
		{
			std::cout << matrix[line][column];
		}
}

void Cipher::decryptMessage(std::string encryptedMessage)
{
	std::string decryptedMessage;
	int numberOfLines = encryptedMessage.length() / m_key.length();
	std::vector<std::vector<char>> decryptMatrix(numberOfLines, std::vector<char>(m_key.length()));
	
	std::string order;
	order = getKeysIndexes();

	while (order.length()!=0)
	{
		std::string messagePart = encryptedMessage.substr(0, m_key.length()+1);
		std::vector<char> messagePartV = stringToCharVector(messagePart);
		char c = order[0];
		int position = c - '0';
		for (int index = 0; index < messagePart.length(); index++)
		{
			decryptMatrix[index][position] = messagePartV[index];
		}
		order.erase(0, 1);
		encryptedMessage.erase(0, m_key.length()+1);
	}
	

	for (int line = 0; line < decryptMatrix.size(); line++)
	{
		for (int column = 0; column < decryptMatrix[line].size(); column++)
		{
			decryptedMessage += decryptMatrix[line][column];
		}
		std::cout << std::endl;
	}
	std::cout << decryptedMessage;
}

std::string Cipher::getEncryptedMessage()
{
	adjustMessageSizeToFitTranspositionMatrix();
	std::string order;
	order = getKeysIndexes();

	std::vector<std::vector<char>> matrix = createTraspositionMatrix();
	
	std::string encriptedMessage;
	while (order.length()!=0)
	{
		char c = order[0];
		int position = c-'0';

		std::string partAtPosition;
		for (int index = 0; index < matrix.size(); index++)
		{
			partAtPosition += matrix[index][position];
		}
		encriptedMessage += partAtPosition;
		
		order.erase(0, 1);
	}

	return encriptedMessage;
}

std::string Cipher::getKeysIndexes()
{
	int currentIndex = 0;
	std::string orderedKey=m_key;
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
