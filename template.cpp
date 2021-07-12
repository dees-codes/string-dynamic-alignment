/**
 * calculate the optimum alignment between two strings
 * @author Deepson Shrestha
 * @version 10/30/2019
 */
#include <climits>
#include <iomanip>
#include <iostream>
#include <string>
#include "matrix.h"
using namespace std;

const int INF {INT_MAX};

/**
 * find the maximum of three values
 * @param a one of the three values
 * @param b one of the three values
 * @param c one of the three values
 * @return the largest value
 */
int max3(int a, int b, int c);


/**
 * the recursive, memoized optimum function for computing the edit distance
 * between two strings
 * @param s the first string
 * @param i the index of the first string
 * @param t the second string
 * @param j the index of the second string
 * @param memo the memo table
 * @return the optimal alignment score of the two strings
 */
int opt(const string& s, size_t i, const string& t, size_t j,
        Matrix<int>& memo, int match, int mismatch, int gap);


/**
 * find and print the optimal alignment of the two strings
 * @param memo the memo table
 * @param s the first string
 * @param t the second string
 * @param gap the gap penalty
 */
void traceback(const Matrix<int>& memo, const string& s, const string& t,
               int gap);

/**
 * dump the memo table to standard output
 * @param memo the memo table
 * @param s the first string
 * @param t the second string
 */
void print_memo(const Matrix<int>& memo, const string& s, const string& t);


int main(int argc, char* argv[])
{
  if (argc != 6)
  {
    cerr << "Usage: " << argv[0] << " s1 s2 match mismatch gap" << endl;
    return 1;
  }

  // add a space to the beginning of each string to allow traceback
  string s {argv[1]};
  s = ' ' + s;
  string t {argv[2]};
  t = ' ' + t;

  int match {stoi(argv[3])};
  int mismatch {stoi(argv[4])};
  int gap {stoi(argv[5])};

  cout << "match: " << match << endl;
  cout << "mismatch: " << mismatch << endl;
  cout << "gap: " << gap << endl;


  Matrix<int> memo(s.size() + 1, t.size() + 1);
  for (size_t row {0}; row <= s.size(); row++)
  {
    for (size_t col {0}; col <= t.size(); col++)
    {
      memo.at(row, col) = INF;
    }
  }
  
  int score
  {opt(s, (s.size() - 1), t, (t.size() - 1), memo, match, mismatch, gap)};

  cout << "The optimal alignment score between " << s << " and " << t <<
    " is " << score << endl;

  cout << endl << "The completed memo table: " << endl << endl;

  print_memo(memo, s, t);

  traceback(memo, s, t, gap);
  return 0;
}

int opt(const string& s, size_t i, const string& t, size_t j,
        Matrix<int>& memo, int match, int mismatch, int gap)
{
  // Checking if the memo table cell (i,j) has been determined or not.
  if (memo.at(i, j) == INF)
  {
    // When i = 0 & j = 0, i.e when we reach the base case of having two empty
    // strings, set the value in the cell (0,0) to 0.
    if (i == 0 && j == 0)
    { 
      memo.at(0,0) = 0;	
    }

    // When i = 0, i.e when we reach the case of having the first string empty
    // then recursively find the value in the cell (0,j). 
    else if (i == 0)
    {
      memo.at(0,j) = opt(s, i, t, j - 1, memo, match, mismatch, gap) + gap; 
    }

    // When j = 0, i.e when we reach the case of having the second string empty
    // then recursively find the value in the cell (i, 0).
    else if (j == 0)
    {
      memo.at(i,0) = opt(s, i - 1, t, j, memo, match, mismatch, gap) + gap;
    }

    // When ith character of the string matches with jth character, recursively
    // assign the max value out of the (i-1,j)th cell value plus gap, (i,j-1)th
    // cell plus gap or (i-1,j-1)th cell plus match to the (i,j)th cell of the
    // memo table
    else if (s[i] == t[j])
    {
      memo.at(i,j) =
        max3(opt(s, i - 1, t, j, memo, match, mismatch, gap) + gap,
	     opt(s, i, t, j - 1, memo, match, mismatch, gap) + gap,
             opt(s, i - 1, t, j - 1, memo, match, mismatch, gap) + match);
    }

    // When ith character of the string does not match with the jth character,
    // recursively assign the max value out of the (i-1,j)th cell value plus gap,
    // (i,j-1)th cell plus gap or (i-1,j-1)th cell plus mismatch to the (i,j)th
    // cell of the  memo table
    else if (s[i] != t[j])
    {
      memo.at(i,j) =
	max3(opt(s, i - 1, t, j, memo, match, mismatch, gap) + gap,
             opt(s, i, t, j - 1, memo, match, mismatch, gap) + gap,
             opt(s, i - 1, t, j - 1, memo, match, mismatch, gap) + mismatch);
    }	
  }

  return memo.at(i,j);
}

void print_memo(const Matrix<int>& memo, const string& s, const string& t)
{
  int field_width {6};
  int left_label_width {6};
  int left_index_width {3};

  cout << right << setw(left_label_width) << ' ';
  for (size_t col {0}; col < t.size(); col++)
  {
    cout << setw(field_width) << t.at(col);
  }
  cout << endl;

  cout << setw(left_label_width) << ' ';
  for (size_t col {0}; col < t.size(); col++)
  {
    cout << setw(field_width) << col;
  }
  cout << endl;

  cout << setw(left_label_width) << '+';
  for (size_t col {0}; col < t.size(); col++)
  {
    cout << setw(field_width) << "---";
  }
  cout << endl;

  for (size_t row {0}; row < s.size(); row++)
  {
    cout << s.at(row) << setw(left_index_width) << row << " |";
    for (size_t col {0}; col < t.size(); col++)
    {
      if (memo.at(row, col) == INF)
      {
        cout << setw(field_width) << "inf";
      }
      else
      {
        cout << setw(field_width) << memo.at(row, col);
      }
    }
    cout << endl;
  }
}

int max3(int a, int b, int c)
{
  int result {a > b ? a : b};
  return result > c ? result : c;
}

void traceback(const Matrix<int>& memo, const string& s, const string& t,
               int gap)
{
  // creating new string variables that can be edited and changed
  string sf = s;
  string tf = t;

  // determining the max index position for the strings
  size_t i = sf.length() - 1;
  size_t j = tf.length() - 1;

  // Execute the loop until the (0,0)th memo cell is reached
  while (!(i == 0 && j == 0))
  {
    // Check if the (i,j)th cell was derived from (i-1,j)th cell
    if (i!= 0 && (memo.at(i-1,j) + gap) == memo.at(i,j))
    {
      tf = tf.insert(j+1,"_"); // insert _ after the tf[j] 
      i--;
    }

    // Check if the (i,j)th cell was derived from (i,j-1)th cell
    else if (j!=0 && (memo.at(i, j-1) + gap) == memo.at(i,j))
    {
      sf = sf.insert(i+1,"_"); // insert _ after the sf[i]
      j--;
    }

    // If (i,j)th cell was derived from (i-1, j-1)th cell, continue
    // moving diagonally through the memo table
    else
    {
      i--;
      j--;
    }

  } // end while 
  
  cout << endl << "The aligned strings: " << endl;
  cout << sf << endl;
  cout << tf << endl;
}
