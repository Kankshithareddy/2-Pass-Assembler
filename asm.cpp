/*****************************************************************************
TITLE: Assembler CPP Program																																
AUTHOR:   	PANNALA KANKSHITHA REDDY
ROLL NUMBER:	2201CS51

This .cpp file, asm.cpp, is part of the Allocate_Numeric_Location_ment of CS210 at the 
department of Computer Science and Engineering, IIT Patna .
In this code,the input file is test.txt
and output files are labelAndCount.txt,pass_2.txt,error.txt. 
*****************************************************************************/




#include<bits/stdc++.h>
using namespace std;
struct display {
    string one;
   string two;
    string three;
    string four;
};

multimap<string,string> memory;
unordered_map<string,string> labelMap;
string dec_to_bin(int num) {
    // Use bitset to convert integer to binary string
    if (num < 0) {
        // For negative numbers, ensure it's represented using 24 bits
        return bitset<24>(256 + num).to_string().substr(8);
    } else {
        return bitset<24>(num).to_string();
    }
}
char flip(char c) {
    // Flip the character '0' to '1' and '1' to '0'
    return (c == '0') ? '1' : '0';
}

string binary_to_hex(const string& binary) {
    string hex = "";
    // Convert each group of 4 bits to its corresponding hexadecimal digit
    for (size_t i = 0; i < binary.length(); i += 4) {
        string group = binary.substr(i, 4);
        int decimal = stoi(group, nullptr, 2);
        hex += "0123456789ABCDEF"[decimal];
    }
    return hex;
}
string twocomp(const string& num_str) {
    int num = stoi(num_str);
    if (num < 0) {
        string s = dec_to_bin(-num); 
        string twos;
        for (char c : s)  twos += flip(c);  //ones compliment
        // Add one to perform two's complement
        for (int i = twos.size() - 1; i >= 0; i--) {
            if (twos[i] == '0') {
                twos[i] = '1';
                break;
            } else {
                twos[i] = '0';
            }
        }

        return binary_to_hex(twos); //conversion to hexadecimal
    } else {
        // For positive numbers, convert directly to binary and then to hexadecimal
        return binary_to_hex(dec_to_bin(num));
    }
}



string hexadecimal(int num) {
    string hex = "";
    if (num == 0)
        return "000000"; // Return 6 zeros for 0.
    int dec = abs(num);
    while (dec != 0) {
        int rem = dec % 16;
        if (rem < 10)
            hex = to_string(rem) + hex;
        else
            hex = char('a' + rem - 10) + hex;
        dec /= 16;
    }
    // Pad with leading zeros if necessary.
    while (hex.length() < 6) {
        hex = "0" + hex;
    }
    return hex.substr(0, 6); // Take only the first 6 digits.
}

int checking_for_digit(string &str) {
    for (int x = 0; x < str.length(); x++) {
        if (x == 0 && str[x] == '-') continue;     
        if (!isdigit(str[x]))  return 0;                 
    }
    return 1;}
void labels_to_value(vector<display>& fun) {
    int present_add= 0;
    unordered_map<string, int> Address_of_label;
     for (int x = 0; x < fun.size(); x++) {  //finding address of labels
        if (!fun[x].one.empty())   Address_of_label[fun[x].one] = x;
    }

    // Replace labels with values
    present_add= 0;
    for (int x = 0; x < fun.size(); x++) {
        if (!fun[x].three.empty() && labelMap.find(fun[x].three) != labelMap.end()) {
            fun[x].three = labelMap[fun[x].three];
        }

        if (fun[x].two == "call" || fun[x].two == "br" || fun[x].two == "brlz" || fun[x].two == "brz") {
            if (!checking_for_digit(fun[x].three)) {
                int target_address = x;
                int label_address = Address_of_label[fun[x].three];
                fun[x].three = to_string(label_address-target_address);
            }
        }

        present_add++;
    }
}

void error(vector<display> &fun,int stop){
    int Errors_num=0;
    int warning_num=0;
    string k;
    vector<string> error;
     multimap<string, string>::iterator iter;
   for (iter = memory.begin(); iter != memory.end(); ++iter) {
        if (iter != memory.end() && next(iter) != memory.end() && iter->first == next(iter)->first) {
            string k = iter->first + " label repeated";
            error.push_back(k);
        }
    }


    for(int x=0;x<fun.size();x++){
        if(fun[x].two=="ldc"||fun[x].two=="data"||fun[x].two=="adc"||fun[x].two=="ldl"||fun[x].two=="stl"||fun[x].two=="ldnl"||fun[x].two=="stnl"||fun[x].two=="adj"||fun[x].two=="call"||fun[x].two=="brz"||fun[x].two=="brlz"||fun[x].two=="br"){
            if(!checking_for_digit(fun[x].three)){
                k.clear();
                k.append(hexadecimal(x)).append("  not a number");
                error.push_back(k);
               Errors_num++;
            }
        }
        else if(fun[x].two=="add"||fun[x].two=="a2sp"||fun[x].two=="sub"||fun[x].two=="shl"||fun[x].two=="shr"||fun[x].two=="sp2a"||fun[x].two=="return"||fun[x].two=="HALT"){
            if (!fun[x].three.empty()) {
                k.clear();
                k.append(hexadecimal(x)).append("  incorrect syntax");
                    error.push_back(k);
                    Errors_num++;
            }
        }
        else{
             k.clear();
            k.append(hexadecimal(x)).append("  warning incorrect nemonic");
             error.push_back(k);
                warning_num++;
        }
    }
if (Errors_num > 0 || warning_num>0) {
    ofstream pb("error.txt");
    if (pb.is_open()) {
        for (const string &st : error) {
            pb << st << endl;
        }
        pb.close();
    }
    else{
        cout <<"Error:failed to open"<<endl;
    }
    cout<<"No.of errors: "<<Errors_num<<endl;
    cout<<"No.of warnings: "<<warning_num<<endl;
}
else {
    cout<<"Successfully Compiled"<<endl;
}
}

void pass_2(vector<display> &fun){
	vector<string> print2;
	string st;
	for(int x=0;x<fun.size();x++){
		if (fun[x].two == "ldc") {
        st.clear(); 
        st.append(hexadecimal(x)).append(" ldc ").append(twocomp(fun[x].three)).append("00 ");
        print2.push_back(st);
    } 
        if(fun[x].two=="adc"){
            st.clear(); 
            st.append(hexadecimal(x)).append(" adc ").append(twocomp(fun[x].three)).append("01 ");
			    print2.push_back(st);
        }
         if(fun[x].two=="ldl"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" ldl ").append(twocomp(fun[x].three)).append("02 ");
			    print2.push_back(st);
			}
		if(fun[x].two=="stl"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" stl ").append(twocomp(fun[x].three)).append("03 ");
			    print2.push_back(st);
			}
		if(fun[x].two=="ldnl"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" ldnl ").append(twocomp(fun[x].three)).append("04 ");
			    print2.push_back(st);
			}
        if(fun[x].two=="stnl"){
            st.clear(); 
            st.append(hexadecimal(x)).append(" stnl ").append(twocomp(fun[x].three)).append("05 ");
			    print2.push_back(st);
			}
		if(fun[x].two=="add"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" add ").append(" 00000006 ");
			    print2.push_back(st);
			}
        if(fun[x].two=="sub"){
            st.clear(); 
            st.append(hexadecimal(x)).append(" sub ").append(" 00000007 ");
			    print2.push_back(st);
			}
		if(fun[x].two=="shl"){
		     st.clear(); 
            st.append(hexadecimal(x)).append(" shl ").append(" 00000008 ");
			    print2.push_back(st);
			}
        if(fun[x].two=="shr"){
             st.clear(); 
            st.append(hexadecimal(x)).append(" shr ").append(" 00000009 ");
			    print2.push_back(st);
			}
		if(fun[x].two=="adj"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" adj ").append(twocomp(fun[x].three)).append("0a ");
			    print2.push_back(st);
		}
		if(fun[x].two=="a2sp"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" a2sp ").append(" 0000000b ");
			    print2.push_back(st);
		}
		if(fun[x].two=="sp2a"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" sp2a ").append(" 0000000c ");
			    print2.push_back(st);
			}
         if(fun[x].two=="call"){
             st.clear(); 
            st.append(hexadecimal(x)).append(" call ").append(twocomp(fun[x].three)).append("0d ");
			    print2.push_back(st);
			}
		if(fun[x].two=="return"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" return ").append(" 0000000e ");
			    print2.push_back(st);
			}
		if(fun[x].two=="brz"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" brz ").append(twocomp(fun[x].three)).append("0f ");
			    print2.push_back(st);
			}
		if(fun[x].two=="brlz"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" brlz ").append(twocomp(fun[x].three)).append("10 ");
			    print2.push_back(st);
			}
		else if(fun[x].two=="br"){
		    st.clear(); 
            st.append(hexadecimal(x)).append(" br ").append(twocomp(fun[x].three)).append("11 ");
			    print2.push_back(st);
			}
	    if(fun[x].two=="HALT"){
	        st.clear(); 
            st.append(hexadecimal(x)).append(" HALT ").append(" 00000013 ");
			    print2.push_back(st);
			}
	}
    ofstream pc("pass_2.txt");
    if (pc.is_open()) {
        for (const string &st : print2) {
            pc << st << endl;
        }
        pc.close();
    }
    else{
        cout << "failed to open pass_2"<<endl;
    }

	//emulator(fun,stop);
}

     void writeLabelAndCountToFile(const vector<pair<string, int>>& label_addresses) {
    // Vector to store formatted strings
    vector<string> print1;

    // Populate print1 with formatted strings
    for (const auto& pair : label_addresses) {
        string format1 = pair.first + " " + hexadecimal(pair.second);
        print1.push_back(format1);
    }

    // Writing to file
    ofstream pa("labelAndCount.txt");
    if (pa.is_open()) {
        for (const string& str : print1) {
            pa << str << endl;
        }
        cout << "Output written to labelAndCount.txt successfully." << std::endl;
    } else {
       cerr << "Error: Unable to open file for writing." << std::endl;
    }
}

void pass_1(vector<display>& fun) {
    int stop;
  map<string, int> label_addresses;// Map to store addresses of labels
    int address = 0;

    for (int x = 0; x < fun.size(); x++) {  // Check if the current instruction has a label
        if (!fun[x].one.empty()) label_addresses[fun[x].one] = address;
        // Increment address
        if (fun[x].two == "data") memory.insert({fun[x].one, fun[x].three}); // increase address if mnemonic=data
        if (fun[x].two == "HALT")  stop=address; // stop processing if halt comes
        address++;
    }
vector<pair<string, int>> label_addresses_vec(label_addresses.begin(), label_addresses.end());
    // Output labels with their addresses
    writeLabelAndCountToFile(label_addresses_vec);
    error(fun, stop);
    pass_2(fun);

    //print1(vector)->address label
}

int main() {
    ifstream infile("test.txt");
    display output;
    vector<display> fun;
     
    
    if (!infile.is_open()) {
        cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }
    string line;
    int present_add= 0;
    while (getline(infile, line)) {
    std::istringstream ss(line);
    int label_present = 0;
    std::string symbol, nemonic, label, value;
    while (ss >> symbol && symbol[0] != ';') { // symbol not a comment
        if (symbol.back() == ':') {
            label = symbol.substr(0, symbol.size() - 1);
            label_present = 1;
        } else {
            // If symbol is not a label
            if (nemonic.empty()) {
                // If nemonic is not set yet, assign symbol to nemonic
                nemonic = symbol;
            } else {
                // Otherwise, assign symbol to value
                value = symbol;
            }
        }
    }

    // Check if the line contains label, value, and nemonic is "data"
    if (!label.empty() && !value.empty() && nemonic == "data") {
       labelMap.insert({label, value});
    }

    // Output the formatted instruction directly into the vector
    if (!nemonic.empty()) {
        fun.push_back({label, nemonic, value});
    }

    if (!symbol.empty() && symbol.front() != ';') {
        present_add++;
    }
}
     
    labels_to_value(fun);
    pass_1(fun);
    return 0;
}