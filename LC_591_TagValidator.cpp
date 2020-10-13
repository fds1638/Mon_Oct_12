// done 12 Oct 2020
// solves Leet Code 591, Tag Validator
// https://leetcode.com/problems/tag-validator/
// difficulty: Hard

// The problem gives you a line of code, which is some text enclosed by
// some tags, and asks to give a T/F on whether the code given follows
// the list of tag and text rules given.

// My solution isn't the prettiest, but it is pretty fast (sometimes running
// in 0ms!), although it does use up a lot of memory.

class Solution {
public:
    bool isValid(string code) {
        // Use a stack to match up opening and closing tags.
        stack<string> opens;
        bool intag = false;     // am I in a tag?
        bool opentag = false;   // am I in an opening tag?
        bool incdata = false;   // am I in a CDATA block?
        string temp = "";       // temp string to hold text of tags
        if (code[0]!='<') {return false;}   // low-hanging fruit
        int ii=0;               // index for where we are in the code
        while (ii<code.size()){
            if (intag&&code[ii]=='<') {return false;}   // if already in a tag, can't open a tag
            if (code[ii]=='<' && !incdata) {            // are in a tag; in CDATA ignore <
                if ( code[ii+1]=='/') {                 // in a closing tag
                    intag = true; 
                    if ( opens.size()==0 ) {return false;}  // if no opening tag for potential match
                    if (code[ii+2]=='>') { return false;}   // if no text in the tag
                } else if ( code[ii+1]=='!') {              // in CDATA
                    if ( opens.size()==0 ) { return false; }    // if haven't had an opening tag yet
                    if ( code.substr(ii,9).compare("<![CDATA[")==0) { // expect these, process at once
                        temp = "";                              // reset temp
                        incdata = true;                         // flag for inside CDATA true
                        ii+=8;                                  // advance index
                    } else {
                        return false;                           // invalid <
                    }
                } else if ( ! incdata) {                        // in an opening tag
                    intag = true;
                    opentag = true;
                }
                if (! incdata) {temp += '<';}                   // ignore < within CDATA
            } else {
                // if have an illegal letter in the tag, return false
                if ( intag && (( (code[ii]<'A'||code[ii]>'Z')&&code[ii]!='/'&&code[ii]!='>' ) ||opentag&&code[ii]=='/') ) {
                    return false;
                } else if ( intag  ) {              // process text of the tag
                    temp += code[ii];               // add char to temp
                    // if the text of the tag is too long return false
                    if (code[ii]!='>'&&temp.find('/')==string::npos&&temp.size()>10) {return false;}
                    if ( code[ii]=='>' ) {          // if at the end of the tag
                        if (temp.find('/')==string::npos) { // if it's an opening tag
                            opens.push(temp);               // put it on the stack
                            opentag = false;                // change the flag opentag
                        } else {                            // if it's a closing tag
                            string closetag = temp.substr(0,1)+temp.substr(2);  // remove the "/" so can compare to ...
                            string opentag = opens.top();                       // ... opening tag on top of stack
                            if (closetag.compare(opentag)==0) {                 // if it matches ...
                                opens.pop();                                    // ... take it off the stack
                                // if have closed last tag on stack but haven't reached the end of the code, return false
                                if (opens.size()==0 && ii<code.size()-1 ) {return false;} 
                            } else {                                            // if closing tag does not match opening...
                                return false;                                   // return false
                            }
                        }
                        temp = "";                                              // reset temp folling closing of tag
                        intag = false;                                          // set intag flag to false
                    }
                } else if ( incdata ){                                          // inside CDATA 
                    temp += code[ii];                                           // append to temp string
                    if ( code[ii]==']' ) {                                      // if closing bracket...
                        if ( code.substr(ii,3).compare("]]>")==0) {             // check if it's part of closing ]]> sequence
                            temp = "";                                          // if so, CDATA is done so reset
                            incdata = false;
                            ii+=2;                                              // and advance index
                        }
                    }
                }
            } 
            ii++;
        }
        if (temp.size()>0) {return false; }                 // if have dangling text not part of a tag, return false
        if (opens.size()>0){return false; }                 // if haven't closed all tags on the stack, return false
        return true;
    }
};


