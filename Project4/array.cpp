#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int tally(const string a[], int n, string target)
{
	if (n < 0)
		return -1;

	int count = 0;
	for (int i = 0; i < n; i++) //Goes through the interesting items and increments count upon finding the target string
		if (a[i] == target)
			count++;
	return count;
}

int findFirst(const string a[], int n, string target)
{
	for (int i = 0; i < n; i++) //Goes through interesting items and returns (ending the loop) once finding the target string
		if (a[i] == target)
			return i;
	return -1;
}

bool findFirstSequence(const string a[], int n, string target, int& begin, int& end)
{
	int b = -1, e = -1; //b and e stand for begin and end
	for (int i = 0; i < n; i++)
	{
		if (a[i] == target && b == -1) //Upon finding the first occurence of the target string, begin and end are set to that position
		{
			b = i;
			e = i;
			if (i + 1 >= n) //Handles the case if its a sequence of 1 at the end of the array
			{
				begin = b;
				end = e;
				return true;
			}
			continue; //Don't want the other if statements to be considered upon finding the first occurence of the target string
		}
		if (a[i] == target && a[i - 1] == target) //Handles when the sequence is longer than 1
		{
			e++;
			if (i + 1 >= n || a[i + 1] != target) //Finds the end of a sequence longer than 1
			{
				begin = b;
				end = e;
				return true;
			}
		}
		if (i - 1 >= 0 && a[i] != target && a[i - 1] == target) //Handles the case if its a sequence of 1 anywhere but the end
		{
			begin = b;
			end = e;
			return true;
		}
	}
	return false;
}

int positionOfMin(const string a[], int n)
{
	if (n <= 0)
		return -1;

	int pos = 0; //Holds the position of the minimum entry encountered as the array is gone through
	for (int i = 0; i < n; i++)
	{
		if (a[i] < a[pos]) //Makes pos holds the position of the minimum entry encountered (< ensures that it is the position of the first encounter)
			pos = i;
	}
	return pos;
}

int moveToEnd(string a[], int n, int pos)
{
	if (n < 0 || pos < 0 || pos >= n)
		return -1;

	string temp = a[pos]; //Holds the entry at pos for safe keeping
	for (int i = pos; i < n - 1; i++) //Shifts every entry after pos one to the left
	{
		a[i] = a[i + 1];
	}
	a[n - 1] = temp; //Sets the last entry to the original entry that was at pos
	return pos;
}

int moveToBeginning(string a[], int n, int pos)
{
	if (n < 0 || pos < 0 || pos >= n)
		return -1;

	string temp = a[pos]; //Holds the entry at pos for safe keeping
	for (int i = pos; i > 0; i--) // Shifts every entry before pos one to the right
	{
		a[i] = a[i - 1];
	}
	a[0] = temp; //Sets the first entry to the original entry that was at pos
	return pos;
}

int disagree(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0)
		return -1;
	
	int n;
	if (n1 < n2) //Gets the size of the smallest array
		n = n1;
	else
		n = n2;

	for (int i = 0; i < n; i++) //Compares entries in each array until the last entry of the smallest array,
		if (a1[i] != a2[i])     //returning and breaking the loop upon finding a disagreement
			return i;
	return n;
}

int removeDups(string a[], int n)
{
	if (n < 0)
		return -1;
	
	int i;
	int ncount = 0; //ncount keeps track of how many elements of the array have been examined
	bool adjust;
	if (a[n - 1] == "") //Due to replacing duplicates with "" and moving them to the end of the array, adjustment is necessary if the original
		adjust = true;  //final entry is "" (but only if there are duplicates)
	else
		adjust = false;

	for  (i = 0; i < n; i++)
	{
		if (ncount >= n) //If there are duplicates, ncount will cause the loop to break once every entry in the original array has been examined
			break;
		if (i + 1 < n && a[i] == a[i + 1]) //If the next entry is a duplicate, the duplicate is removed and all entries after it are moved one to the left
		{                                  //with the final entry being replaced by ""
			for (int j = i + 1; j < n - 1; j++)
			{
				a[j] = a[j + 1];
			}
			a[n - 1] = "";
			i--; //i is decrememnted so that it can be checked if there are more duplicates
		}        //Due to this decrement, i will end at the value of the number of unique items in the altered array,
		ncount++;//unless the final item is "" when there are duplicates, which is handled by adjust
	}
	if (i == ncount || !adjust) //if i == ncount, there were no duplicates, so no adjustment is required
		return i;               //if "" was not the last entry, no adjustment is required either
	else //if there were duplicates and "" was the last entry adjustment is necessary
		return i + 1;
}

bool subsequence(const string a1[], int n1, const string a2[], int n2)
{
	if (n1 < 0 || n2 < 0 || n2 > n1) //If the second array is larger than the first, it can't be a subsequence
		return false;

	int pos = 0; //pos is to check if entries appear in the same order
	bool found = true; //found keeps track if entries in a2 have been found in a1
	for (int i = 0; i < n2; i++) //Takes entries in a2 one by one to compare to entries in a1
	{
		found = false;
		for (int j = pos; j < n1; j++)
		{
			if (a2[i] == a1[j])
			{
				pos = j + 1; //pos is set to one after the found entry for the next time through the loop
				found = true;
				break;
			}
		}
		if (!found) //If the entry from a2 is not found, false is returned; otherwise the next entry is looked for
			return false;
	}
	return true; //If each entry is found or if the loop is never entered (n2=0), the loop will end and true will be returned
}

int mingle(const string a1[], int n1, const string a2[], int n2, string result[], int max)
{
	if (n1 < 0 || n2 < 0 || n1 + n2 > max)
		return -1;
	for (int i = 0; i < n1 - 1; i++) //Checks for nondecreasing order
		if (a1[i] > a1[i + 1])
			return -1;
	for (int i = 0; i < n2 - 1; i++) //Checks for nondecreasing order
		if (a2[i] > a2[i + 1])
			return -1;

	for (int i = 0; i < n1; i++) //Puts all entries from a1 and a2 into result
		result[i] = a1[i];
	for (int i = 0; i < n2; i++)
		result[i + n1] = a2[i];

	string temp;
	for (int j = 0; j < n1 + n2; j++) //Outer loop determines the range for the inner loop
		for (int i = 0; i < n1 + n2 - j - 1; i++) //Inner loop takes the largest entry within its range and puts it at the end of its range
			if (result[i] > result[i + 1])        //Range decreases so that the largest entry is put at the end, then the second largest
			{                                     //is put one before the end, etc.
				temp = result[i];
				result[i] = result[i + 1];
				result[i + 1] = temp;
			}
	return n1 + n2;
}

int divide(string a[], int n, string divider)
{
	if (n < 0)
		return -1;

	string temp;
	for (int j = 0; j < n; j++) //Same sorting from last function
		for (int i = 0; i < n - j - 1; i++)
			if (a[i] > a[i + 1])
			{
				temp = a[i];
				a[i] = a[i + 1];
				a[i + 1] = temp;
			}

	int pos = -999;
	for (int i = 0; i < n; i++) //Finds the position of the first entry not less than divider
		if (a[i] >= divider)
		{
			pos = i;
			break;
		}
	if (pos == -999)
		return n;
	else
		return pos;
}

int main()
{
	string h[7] = { "petyr", "jaime", "jon", "daenerys", "", "tyrion", "jon" };
	assert(tally(h, 7, "jon") == 2);
	assert(tally(h, 7, "") == 1);
	assert(tally(h, 7, "theon") == 0);
	assert(tally(h, 0, "jon") == 0);
	assert(findFirst(h, 7, "jon") == 2);
	assert(findFirst(h, 2, "jon") == -1);
	int bg;
	int en;
	assert(findFirstSequence(h, 7, "daenerys", bg, en) && bg == 3 && en == 3);

	string g[4] = { "petyr", "jaime", "daenerys", "tyrion" };
	assert(positionOfMin(g, 4) == 2);
	assert(disagree(h, 4, g, 4) == 2);
	assert(subsequence(h, 7, g, 4));
	assert(moveToEnd(g, 4, 1) == 1 && g[1] == "daenerys" && g[3] == "jaime");

	string f[4] = { "daenerys", "tyrion", "jon", "jaime" };
	assert(moveToBeginning(f, 4, 2) == 2 && f[0] == "jon" && f[2] == "tyrion");

	string e[5] = { "daenerys", "daenerys", "daenerys", "jon", "jon" };
	assert(removeDups(e, 5) == 2 && e[1] == "jon");

	string x[4] = { "cersei", "jaime", "jaime", "theon" };
	string y[4] = { "daenerys", "jaime", "jon", "tyrion" };
	string z[10];
	assert(mingle(x, 4, y, 4, z, 10) == 8 && z[5] == "jon");

	assert(divide(h, 7, "jon") == 3);

	cout << "All tests succeeded" << endl;
}