// done 12 Oct 2020
// solves Leet Code problem 980, Distinct Subsequences II
// https://leetcode.com/problems/distinct-subsequences-ii/
// difficulty: Hard

// Given a sequence of lower case characters, how many subsequences
// can be created. If all the characters were distinct, it would be
// 2^n - 1, but with repeated chars I used a dynamic programming table.

class Solution {
public:
    int distinctSubseqII(string S) {
        // This is relatively slow and expensive in space, but gets the job done.
        // Set up dynamic programming table dp, square in size S.size().
        // Lower left half of table is not used.
        // Set up vector prev, which for each character tells you at which index
        // it previously appeared, or gives -1 if never appeared before.
        int retval = 0; // return value
        int p = 1000000007;
        int dp[S.size()][S.size()];
        int prev[S.size()];
        map<char,int> letters;
        for (int rr=0; rr<S.size(); rr++) {
            pair<map<char,int>::iterator, bool> ret = letters.insert({S[rr],rr});
            if ( ret.second ) {
                prev[rr]=-1;
            } else {
                prev[rr]=ret.first->second;
                ret.first->second = rr;
            }
            
            for (int cc=0; cc<S.size(); cc++) {
                dp[rr][cc]=0;
            }
        }
        
        // Top row of dp table is number of different subsequences of length 1 that
        // can be found with the first cc characters. So if a character is a repeat,
        // it doesn't add a new one-char subsequence.
        dp[0][0]=1;
        for (int cc=1; cc<S.size(); cc++) {
            if (prev[cc]==-1) { 
                dp[0][cc]=dp[0][cc-1]+1; }
            else { 
                dp[0][cc]=dp[0][cc-1]; }
        }
        // Add the last column to the return value retval.
        retval+=dp[0][S.size()-1]%p; 
        retval = retval%p;
        
        // Row rr=0 represents subsequences of length 1, calculated above.
        // Row rr=1 represents subsequences of length 2, etc.
        // For the rest of the rows, each character adds new subsequences
        // by being appended to subsequences of length one less, but only
        // going back to the previous appearance of that same char, inclusive.
        // As dp works across the rows, accumulate by adding to the number to
        // its left so that can easily get accumulations between any two columns.
        for (int rr=1; rr<S.size(); rr++) {
            for (int cc=rr; cc<S.size(); cc++) {
                int curval = 0;     // value for this entry
                if ( prev[cc]<=0) { // first time seeing this char, append to all subseq in prev row
                    curval += dp[rr-1][cc-1]%p;
                } else { // append this char only to those subseq since prev appearance 
                    curval += dp[rr-1][cc-1]%p-dp[rr-1][prev[cc]-1]%p;
                }
                dp[rr][cc]=curval%p+dp[rr][cc-1]%p;
                retval += curval%p; // add to retval
                retval = retval%p;
            }
        }
        
        return retval;
    }
};


