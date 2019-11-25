#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool hasCorrectSyntax(string dance)
{
	int beatStart = 0;
	string beat = "";
	for (int i = 0; i != dance.size(); i++)
	{
		if (tolower(dance[i]) != 'l' && tolower(dance[i]) != 'r' && tolower(dance[i]) != 'd' && tolower(dance[i]) != 'u' &&
			dance[i] != '/' && !isdigit(dance[i])) //Checks that only the allowed characters are present
			return false;

		if (dance[i] == '/') //Finds end of each beat
		{
			beat = dance.substr(beatStart, i-beatStart); //Substring is the instructions of the beat
			beatStart = i + 1; //Adjusts the start of the beat for the next run through the loop
			if (beat.size() > 3 || //Beat must be 1, 2, or 3 characters long
				(beat.size() >= 1 && !isalpha(beat[beat.size() - 1])) || //Last character must be a letter
				(beat.size() >= 2 && !isdigit(beat[beat.size() - 2])) || // Second and third to last characters must be digits
				(beat.size() == 3 && !isdigit(beat[beat.size() - 3]))) 
				return false;
		}
	}
	beat = dance.substr(beatStart, dance.size()-beatStart);
	if (beat.size() > 0) //Checks if there is anything after the last beat
		return false;
	return true;
}


int convertDance(string dance, string& instructions, int& badBeat)
{
	if (!hasCorrectSyntax(dance)) //Return 1 if there is wrong syntax
		return 1;

	string convertedDance = ""; //Variables to be used
	int beatStart = 0;
	string beat = "";
	int freeze = 0;
	int beatNumber = 0;

	for (int i = 0; i < dance.size(); i++)
	{
		if (dance[i] == '/') //Finds end of each beat
		{
			beat = dance.substr(beatStart, i - beatStart); //Substring is the instructions of the beat
			beatStart = i + 1; //Adjusts the start of the beat for the next run through the loop
			switch (beat.size())
			{
			case 3:	//When beat is 2 or 3 characters long, a freeze is indicated; cases are handled almost identically
			case 2:
					if (beat.size() == 2) //Freeze length is calculated differently for different sizes
						freeze = beat[0] - '0';
					else
						freeze = 10 * (beat[0] - '0') + (beat[1] - '0');
					
					if (freeze == 0 || freeze == 1)
					{
						beatNumber++; //beatNumber keeps track of beat being examined
						badBeat = beatNumber;
						return 3;
					}
					
					for (int j = 0; j < freeze; j++) //Loop goes through the length of the freeze
					{
						beatNumber++; 
						if (i == dance.size()) //This occurs when the freeze is cut short
						{
							badBeat = beatNumber;
							return 2;
						}
						if (dance[i] != '/') //This occurs when a beat not consisting of only slash occurs during a freeze
						{
							badBeat = beatNumber;
							return 4;
						}
						convertedDance += toupper(beat[beat.size() - 1]);
						i++; //i is incremented so that the original dance can continue to be examined
					}
					beatStart = beatStart + freeze - 1;
					i--; //i is incremented 1 too far after going through the freeze
					break;
				case 1: //When beat is 0 or 1 characters long, it is handled almost identically
				case 0:
					beatNumber++;
					if (beat.size() == 0)
						convertedDance += ".";
					else
						convertedDance += tolower(beat[0]);
					break;
			}
		}
	}
	instructions = convertedDance; //We only want instructions to be altered if the dance is convertible, thus the use of a separate string variable
	return 0;
}

int main()
{
	string newDance = "";
	int bb = -999;
	string dance = "/03D////U/d/2L//2r//03u///";
	getline(cin, dance);
	int a = convertDance(dance, newDance, bb);
	cout << newDance << endl << a << endl << bb << endl;
}