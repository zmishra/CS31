#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<cstring>
#include<cctype>

using namespace std;

int format(int lineLength, istream& inf, ostream& outf)
{
	bool wordPrinted = false; //Used to determine if spaces need to be considered
	bool wordPortionPrinted = false; //Only important when a word portion is larger than the line length
	bool sentenceEnd = false;
	bool paragraphBreak = false; //booleans used because nothing can be printed preemptively
	bool wordFound = false; //Tracks if any word is found
	int returnCode = 0;
	int count = 0; //Keeps track of how many character have been or will potenntially be printed to a line
	char word[400] = "";
	char temp[2] = " "; //Used to append characters onto word
	char c;

	if (lineLength < 1)
		return 2;

	while (inf.get(c))
	{
		if (strlen(word) !=0 && (isspace(c) || (word[strlen(word) - 1] == '-' && !isspace(c)))) //Find out if there are word portions/words
		{
			if (isspace(c) && strcmp(word, "#P#") == 0)
			{
				if (wordPrinted && !paragraphBreak) //At the beginning of a line is the only time when wordPrinted is false for this statement
					paragraphBreak = true;
				strcpy(word, "");
				continue;
			}
			wordFound = true;
			if (paragraphBreak) //Deals with printing out a paragraph break
			{
				outf << '\n' << '\n';
				count = 0;
				wordPrinted = false;
				paragraphBreak = false;
			}

			if (strlen(word) > lineLength) //Finds out if the word portion is longer than the line length
			{
				returnCode = 1;
				if (wordPrinted || wordPortionPrinted) //If a word or word portion has been printed, go to the next line, unless the end of word has been reached
					outf << '\n';
				for (int i = 1; word[i-1] != '\0'; i++)
				{
					outf << word[i-1];
					if (i % lineLength == 0 && i != strlen(word)) //After printing out a line of characters, go to the next line
						outf << '\n';
				}
				if (word[strlen(word) - 1] == '-' && !isspace(c))
				{
					wordPrinted = false; //No spaces after a word portion ending with '-', so wordPrinted is false
					wordPortionPrinted = true;
				}
				else
				{
					wordPrinted = true;
					wordPortionPrinted = false; //Word portion printed needs to be reset for the next words
					sentenceEnd = (word[strlen(word) - 1] == '.' || word[strlen(word) - 1] == '?');
				}
				if (strlen(word) % lineLength == 0) //with a count depending on the remainder of the string length over the line length
					count = lineLength;
				else
					count = strlen(word) % lineLength;
				strcpy(word, "");
				if (!isspace(c)) //still need to process character
				{
					temp[0] = c;
					strcat(word, temp);
				}
				continue; //The word is done being processed
			}

			if (wordPrinted) //If a word has been printed, need to check if a space, spaces, or newline character is needed
			{
				if (sentenceEnd)
				{
					count += 2;
					if (count + strlen(word) > lineLength) //Checks if another word preceded by 2 spaces can be put on the line
					{
						outf << '\n';
						count = 0;
					}
					else
						outf << ' ' << ' ';
				}
				else
				{
					count++;
					if (count + strlen(word) > lineLength) //Checks if another word preceded by a space can be put on the line
					{
						outf << '\n';
						count = 0;
					}
					else
						outf << ' ';
				}
			}
			if (count + strlen(word) > lineLength) //Checks if the word or word portion can fit on the line
			{
				outf << '\n';
				count = 0;
			}
			count += strlen(word);
			outf << word;
			if (word[strlen(word) - 1] == '-' && !isspace(c))
			{
				wordPrinted = false; //No spaces after a word portion ending with '-', so wordPrinted is false
				wordPortionPrinted = true;
			}
			else
			{
				wordPrinted = true;
				wordPortionPrinted = false; //wordPortionPrinted needs to be reset for the next word
				sentenceEnd = (word[strlen(word) - 1] == '.' || word[strlen(word) - 1] == '?');
			}
			strcpy(word, "");
		}

		if (!isspace(c)) //Finds words
		{
			temp[0] = c;
			strcat(word, temp);
		}
	}
	if (strlen(word) != 0 && strcmp(word, "#P#") != 0) //On exiting the loop, if there is a last word, it needs to be checked
	{												   //Code is nearly the same as before, but only concerned with printing
		wordFound = true;
		if (paragraphBreak)							   //Paragraph breaks do not matter unless there is a word to follow 
		{
			outf << '\n' << '\n';
			count = 0;
			wordPrinted = false;
			paragraphBreak = false;
		}

		if (strlen(word) > lineLength)
		{
			if (wordPrinted || wordPortionPrinted) //If a word or word portion has been printed, go to the next line
				outf << '\n';
			for (int i = 1; word[i - 1] != '\0'; i++)
			{
				outf << word[i - 1];
				if (i % lineLength == 0 && i != strlen(word)) //After printing out a line of characters, go to the next line, unless the end of word has been reached
					outf << '\n';
			}
			outf << '\n';
			return 1;
		}

		if (wordPrinted) //If a word has been printed, need to check if a space, spaces, or newline character is needed
		{
			if (sentenceEnd)
			{
				count += 2;
				if (count + strlen(word) > lineLength) //Checks if another word preceded by 2 spaces can be put on the line
				{
					outf << '\n';
					count = 0;
				}
				else
					outf << ' ' << ' ';
			}
			else
			{
				count++;
				if (count + strlen(word) > lineLength) //Checks if another word preceded by a space can be put on the line
				{
					outf << '\n';
					count = 0;
				}
				else
					outf << ' ';
			}
		}
		if (count + strlen(word) > lineLength) //Checks if the word or word portion can fit on the line
		{
			outf << '\n';
		}
		outf << word << '\n';
	}
	else if (wordFound)
		outf << '\n';
	return returnCode;
}

int main()
{
	const int MAX_FILENAME_LENGTH = 100;
	for (;;)
	{
		ofstream outfile;
		
		cout << "Enter input file name (or q to quit): ";
		char filename[MAX_FILENAME_LENGTH] = "C:\\Users\\zubin\\Documents\\Visual Studio 2015\\Projects\\Project5\\test.txt";
		//cin.getline(filename, MAX_FILENAME_LENGTH);
		if (strcmp(filename, "q") == 0)
			break;
		ifstream infile(filename);
		if (!infile)
		{
			cerr << "Cannot open " << filename << "!" << endl;
			continue;
		}
		cout << "Enter maximum line length: ";
		int len;
		cin >> len;
		cin.ignore(10000, '\n');
		outfile.open("test1.txt");
		int returnCode = format(len, infile, outfile);
		cout << "Return code is " << returnCode << endl;
	}
}