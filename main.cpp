#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <functional>

using namespace std;

bool is_positive(int a){
	return a > 0;
};
bool isnt_future(int y){
	return y<2024;
}
bool is_w_num(int a){
	return a>=0;
}
bool is_group_period(int g=1, int p=1){
	return (g>0 and g<19 and p>0 and p<8);
};

enum Type {
    metal,
    non_metal,
    halogen,
    noble_gases,
    etc
};
string type_to_string(Type t) {
    switch (t) {
        case metal: return "Metal";
        case non_metal: return "Non-Metal";
        case halogen: return "Halogen";
        case noble_gases: return "Noble Gas";
        default: return "Etc";
    }
}


struct Element{
	string name;
	string symbol;
	double A; // mess
	int Z; // atomic number
	double average_a=0;
	Type type;
	int find_at;
	int ex_remain;
	int iso_count;
	vector<double> iso_list;
	int group;
	int period;

	// methods
	void set_name(){
		cout << "enter your element name: ";
		getline(cin, name);
	};
	bool set_symbol(){
		string sym;
		cout << "Element symbol: ";
		cin >> sym;
		if(sym.empty() or sym[0]<65 or sym[0]>90 or sym.length()>2)
			return false;
		symbol = sym;
		return true;
	}
	void calculate_average_a(){
		average_a = 0;
		for(int i=0; i<iso_count; i++)
			average_a += iso_list[i];
		average_a = average_a/iso_count;
	}
	void modify_iso_count(){
		bool flag = false;
		for(int i=0; i<iso_list.size(); i++){
			if(iso_list[i]==A){
				flag = true;
				break;
			}
		}
		if(!flag)
			iso_list.push_back(A);
		iso_count = iso_list.size();
		calculate_average_a();
	}
	bool is_iso_exist(int m){
		for(int i=0; i<iso_list.size(); i++){
			if(iso_list[i]==m)
				return true;
		}
		return false;
	}
	void set_iso(){
		iso_list.clear();
		iso_list.push_back(A);
		int m = 0;
		do {
			cout << "\nEnter the Isotope A (enter 0 to stop): ";
			cin >> m;
			if(is_positive(m) and !is_iso_exist(m))
				iso_list.push_back(m);
		} while(m != 0);
		modify_iso_count();
	}
	bool set_location(){
		int g=0, p=0;
		cout << "Group: ";
		cin >> g;
		cout << "Period: ";
		cin >> p;

		// we could use the function in line 19 too.
		if(g>18 or g<1 or p>7 or p<1)
			return false;

		group = g;
		period = p;
		return true;
	}
	bool set_type(){
		int in=0;
		cout << "\n1. metal\n2. non metal\n3. halogen\n4. noble gases\n5.etc\n======\nEnter: ";
		cin >> in;
		if(in>5 or in<1)
			return false;
		switch(in){
			case 1:
				type = metal;
				break;
			case 2:
				type = non_metal;
				break;
			case 3:
				type = halogen;
				break;
			case 4:
				type = noble_gases;
				break;
			case 5:
				type = etc;
				break;
		}
		return true;
	}
};



// ترکیب شیمیایی
struct Compound{
	struct Involved{
		string symbol;
		int count;
	};

	string name;
	vector<Involved> involveds;
};

vector<Element> elements;
vector<Compound> compounds;

bool is_exist(int z){
	for(int i=0; i<elements.size(); i++){
		if(elements[i].Z==z)
			return true;
	}
	return false;
};
bool is_exist(string sym){
	for(int i=0; i<elements.size(); i++){
		if(elements[i].symbol==sym)
			return true;
	}
	return false;
}
bool is_compound_exist(string name){
	for(Compound c: compounds){
		if(c.name==name)
			return true;
	}
	return false;
}
int search_element(int z){
	for(int i=0; i<elements.size(); i++){
		if(elements[i].Z==z)
			return i;
	}
	return -1;
}
int search_element(string sym){
	for(int i=0; i<elements.size(); i++){
		if(elements[i].symbol==sym)
			return i;
	}
	return -1;
}
int search_compound(string name){
	for(int i=0; i<compounds.size(); i++){
		if(compounds[i].name==name)
			return i;
	}
	return -1;
}
void show_elements_short(){
	cout << "\nSYMBOL - IN LAB\n";
	for(int i=0; i<elements.size(); i++)
		cout << elements[i].symbol << " - " << elements[i].ex_remain << endl;
}

bool add_element(){
	Element newE;

	cout << "Z of Element: ";
	cin >> newE.Z;

	// check if element already exist
	if(is_exist(newE.Z)){
		return false;
	}

	cin.ignore();
	newE.set_name();

	// set symbol
	if(!newE.set_symbol()){
		cout << "SOMTHING WENT WRONG!";
		return false;
	}

	cout << "A of Element: ";
	cin >> newE.A;

	newE.set_type();

	cout << "Discover At: ";
	cin >> newE.find_at;
	if(!isnt_future(newE.ex_remain))
		return false;

	cout << "examples in Lab: ";
	cin >> newE.ex_remain;

	newE.set_iso();

	if(!newE.set_location()){
		cout << "SOMTHING WENT WRONG!";
		return false;
	}

	elements.push_back(newE);

	return true;
};
bool add_isotope(){
	int z;
	cout << "Enter The Atomic number of element you wanna change its isotopes: ";
	cin >> z;
	if(is_exist(z)){
		elements[search_element(z)].set_iso();
		return true;
	}
	return false;
};
bool add_chemical_compound() {
    Compound newC;

    cout << "Name: ";
    cin.ignore();
    getline(cin, newC.name);

    show_elements_short();

    string sym = "";
    bool repet = false;

    do {
        sym = "";
        cout << "symbol (enter empty to stop): ";
        getline(cin, sym);

        if (sym == "")
            break;
        else if (!is_exist(sym) || elements[search_element(sym)].ex_remain < 1) {
            cout << "THERE IS NO MORE " << elements[search_element(sym)].symbol << " IN LAB.\n";
            continue;
        }

        repet = false;
        for (int i = 0; i < newC.involveds.size(); i++) {
            if (newC.involveds[i].symbol == sym) {
                newC.involveds[i].count++;
                repet = true;
            }
        }
        if (!repet) {
            Compound::Involved inv;
            inv.symbol = sym;
            inv.count = 1;

            newC.involveds.push_back(inv);
        }
        elements[search_element(sym)].ex_remain--;
    } while (true);

    compounds.push_back(newC);

    return true;
}
void create_choices(){
	int c=0;
	cout << "\n1. Create Element\n2. Create Isotope\n3. Create Chemical Compound\nEnter: ";
	cin >> c;
	cin.ignore();
	switch(c){
		case 1:
			if(add_element())
				cout << "\nElement Successfully been added.\n";
			else
				cout << "\n\aSOMTHING WENT WRONG\n";
			break;
		case 2:
			if(add_isotope())
				cout << "\nIsotope Successfully been added.\n";
			else
				cout << "\n\aSOMTHING WENT WRONG\n";
			break;
		case 3:
			if(add_chemical_compound())
				cout << "\nCompound Successfully been added.\n";
			else
				cout << "\n\aSOMTHING WENT WRONG\n";
			break;
	}
}
bool remove_element(){
	int z=0;
	cout << "to remove, enter Element Z: ";
	cin >> z;

	if(is_exist(z)){
		elements.erase(elements.begin() + search_element(z));
		return true;
	}
	else
		cout << "there is no Element with given Z.\n";
	return false;
};
bool remove_isotope(){
	int a=0, z=0, i=0;
	cout << "Element z: ";
	cin >> a;

	if(is_exist(a)){
		i = search_element(a);
		cout << "\nIsotopes: ";
		for(int c=0; c<elements[i].iso_count; c++)
			cout << elements[i].iso_list[c] << "\t";
		cout << "\nTo Delete Enter z: ";
		cin >> z;

		elements[i].modify_iso_count();
		for(int c=0; c<elements[i].iso_count; c++){
			if(elements[i].iso_list[c]==z && elements[i].iso_count>1){
				elements[i].iso_list.erase(elements[i].iso_list.begin() + c);
				elements[i].modify_iso_count();
				return true;
			}else{
				cout << "\nEvery Element should have atleast one isotope.";
				break;
			}
		}
	}
	return false;
};
bool remove_chemical_compound(){
	string name="";
	cout << "\nName: ";
	cin.ignore();
	getline(cin , name);
	if(is_compound_exist(name))
		compounds.erase(compounds.begin()+search_compound(name));
	else
		return false;
	return true;
};
void remove_choices(){
	int c=0;
	cout << "\n1. remove Element\n2. remove Isotope\n3. remove Chemical Compound\nEnter: ";
	cin >> c;
	cin.ignore();
	switch(c){
		case 1:
			if(remove_element())
				cout << "\nElement Successfully been removed.\n";
			else
				cout << "\n\aSOMTHING WENT WRONG\n";
			break;
		case 2:
			if(remove_isotope())
				cout << "\nIsotope Successfully been removed.\n";
			else
				cout << "\n\aSOMTHING WENT WRONG\n";
			break;
		case 3:
			if(remove_chemical_compound())
				cout << "\nIsotope Successfully been removed.\n";
			else
				cout << "\n\aSOMTHING WENT WRONG\n";
			break;
	}
};

void bubbleSort(vector<Element>& vec, function<bool(const Element&, const Element&)> comparator) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (comparator(vec[j + 1], vec[j])) {
                swap(vec[j], vec[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}
void sort_by_atomic() {
    bubbleSort(elements, [](const Element& a, const Element& b) {
        return a.Z < b.Z;
    });
}
void show_element(int i){
	Element& e = elements[i];
	cout << "\tname: " << e.name
		 << "\tAtomic: " << e.Z
		 << "\tA: " << e.A
		 << "\taverage_a: " << e.average_a
		 << "\tsymbol: " << e.symbol
		 << "\tfind at: " << e.find_at
		 << "\ttype: " << type_to_string(e.type)
		 << "\texample remains: " << e.ex_remain
		 << "\tiso count: " << e.iso_count
		 << "\tisotopes: ";
	for(int z=0; z<e.iso_list.size(); z++){
		cout << e.iso_list[z];
		if(z<e.iso_list.size()-1)
			cout << " - ";
	}
	cout << "\tgroup: " << e.group << "\tperiod: " << e.period << endl;
}
void show_compound(Compound c){
	cout << c.name << " : ";
	for(int i=0; i<c.involveds.size(); i++)
		cout << c.involveds[i].symbol << c.involveds[i].count;
}
void print_all(){
	cout << "\n========ELEMENTS========\n";
	for(int i=0; i<elements.size(); i++){
		show_element(i);
		cout << endl;
	}
	cout << "\n========COMPOUNDS========\n";
	for(Compound c: compounds){
		show_compound(c);
		cout << endl;
	}
	cout << endl;
}
void sort_by_discovery(){
	bubbleSort(elements, [](const Element& a, const Element& b) {
			return a.find_at < b.find_at;
	});
	show_elements_short();
}
void sort_by_remainings(){
	bubbleSort(elements, [](const Element& a, const Element& b) {
			return a.ex_remain < b.ex_remain;
	});
	show_elements_short();
}
void print_stability(){
	double res=0;
	for(Element e: elements){
		for(int i: e.iso_list){
			cout << e.symbol << '-' << i << "->";
			res = i/e.A;
			if(res>1.5)
				cout << "NO STABLE :(\t";
			else
				cout << "STABLE :)\t";
		}
		cout << endl;
	}
}
void show_by_group(){
	int g;
	cout << "enter a valid group number(1-18): ";
	cin >> g;
	if(is_group_period(g,1)){
		for(Element e: elements){
			if(e.group==g){
				show_element(search_element(e.symbol));
				cout << endl;
			}
		}
	}
}
void show_by_period(){
	int p;
	cout << "enter a valid period number(1-7): ";
	cin >> p;
	if(is_group_period(1,p)){
		for(Element e: elements){
			if(e.period==p){
				show_element(search_element(e.symbol));
				cout << endl;
			}
		}
	}
}

bool update_element(int i){
	int choice=0;
	string line;
	cout << "1. Name\n "
		 << "2. Symbol\n"
		 << "3. A\n"
		 << "4. Type\n"
		 << "5. Discover Year\n"
		 << "6. Remainings\n"
		 << "7. Isotopes\n"
		 << "8. Location\n";
	cin >> choice;

	switch(choice){
		case 1:
			elements[i].set_name();
			break;
		case 2:
			elements[i].set_symbol();
			break;
		case 3:
			cout << "new A: ";
			cin >> choice;
			elements[i].A = choice;
			elements[i].calculate_average_a();
			break;
		case 4:
			if(!elements[i].set_type())
				return false;
			break;
		case 5:
			cout << "Discover at: ";
			cin >> choice;
			if(isnt_future(choice) and is_positive(choice))
				elements[i].find_at = choice;
			else
				return false;
			break;
		case 6:
			cout << "in lablatuary: ";
			cin >> choice;
			if(is_positive(choice))
				elements[i].ex_remain = choice;
			break;
		case 7:
			elements[i].set_iso();
			break;
		case 8:
			if(!elements[i].set_location())
				return false;
			break;
	}


	return true;
};

bool save(){
	ofstream elementFile("elements.txt", ofstream::out | ofstream::trunc), compoundFile("compounds.txt", ofstream::out | ofstream::trunc);
	if (!elementFile.is_open()) {
	    cout << "Error: Cannot open elements.txt for writing.\n";
	    return false;
	}
	if (!compoundFile.is_open()) {
	    cout << "Error: Cannot open compounds.txt for writing.\n";
	    return false;
	}
	for(Element e: elements){
		elementFile << e.name
					<< ',' << e.Z
					<< ',' << e.A
					<< ',' << e.average_a
					<< ',' << e.symbol
					<< ',' << e.type
					<< ',' << e.group
					<< ',' << e.period
					<< ',' << e.iso_count
					<< ',' << e.ex_remain
					<< ',' << e.find_at;
		for(int i=0; i<e.iso_list.size(); i++){
			elementFile << ',' << e.iso_list[i];
		}
		elementFile << endl;
	}
	for(Compound c: compounds){
		compoundFile << c.name;
		for(Compound::Involved in: c.involveds){
			compoundFile << ',' << in.symbol << ',' << in.count;
		}
		compoundFile << endl;
	}
	elementFile.close();
	compoundFile.close();
	return true;
};
bool load() {
    ifstream elementFile("elements.txt"), compoundFile("compounds.txt");

    if (!elementFile.is_open() || !compoundFile.is_open()) {
        cout << "Error opening file(s). Ensure files exist and are accessible.\n";
        return false;
    }

    elements.clear();
    compounds.clear();

    string line;

    while (getline(elementFile, line)) {
        stringstream ss(line);
        Element e;
        string token;
        vector<string> tokens;

        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 10) {
            cout << "Error: Element data corrupted.\n";
            return false;
        }

        e.name = tokens[0];
        e.Z = stoi(tokens[1]);
        e.A = stod(tokens[2]);
        e.average_a = stod(tokens[3]);
        e.symbol = tokens[4];
        e.type = static_cast<Type>(stoi(tokens[5]));
        e.group = stoi(tokens[6]);
        e.period = stoi(tokens[7]);
        e.iso_count = stoi(tokens[8]);
        e.ex_remain = stoi(tokens[9]);
        e.find_at = stoi(tokens[10]);

        for (size_t i = 11; i < tokens.size(); ++i) {
            e.iso_list.push_back(stod(tokens[i]));
        }

        elements.push_back(e);
    }

    while (getline(compoundFile, line)) {
        stringstream ss(line);
        Compound c;
        string token;

        getline(ss, token, ',');
        c.name = token;

        while (getline(ss, token, ',')) {
            Compound::Involved inv;
            inv.symbol = token;

            if (!getline(ss, token, ',')) {
                cout << "Error: Compound data corrupted.\n";
                return false;
            }
            inv.count = stoi(token);

            c.involveds.push_back(inv);
        }

        compounds.push_back(c);
    }

    elementFile.close();
    compoundFile.close();

    cout << "Data successfully been loaded.\n";
    return true;
}
int main(){
	int choice=0;
	int z=0;
	string name = "";

	while(choice!=14){
		z=0;
		name="";
		cout<< "\n*******************************\n"
			<< "1. Create Somthing\n"
			<< "2. Delete Somthing\n"
			<< "3. Sort by Atomic number\n"
			<< "4. Search an Elements\n"
			<< "5. Update an Element\n"
			<< "6. Print All\n"
			<< "7. Print by Discovery\n"
			<< "8. Print by remainings\n"
			<< "9. Print by Stablity\n"
			<< "10. Print By Group or Period\n"
			<< "11. Search for Compound\n"
			<< "12. Save\n"
			<< "13. Load\n"
			<< "14. Exit\n"
			<< "*******************************\nEnter: ";
		cin >> choice;

		switch(choice){
			case 1:
				create_choices();
				break;
			case 2:
				remove_choices();
				break;
			case 3:
				sort_by_atomic();
				break;
			case 4:
				cout << "Element Z: ";
				cin >> z;
				if(is_exist(z)){
					show_element(search_element(z));
				}else
					cout << "there is no element with given z.";
				break;
			case 5:
				cout << "Z: ";
				cin >> z;
				if(is_exist(z) and update_element(search_element(z)))
					cout << "Element were updated successfully.\n";
				else
					cout << "SOMTHING WENT WRONG!\n";
				break;
			case 6:
				print_all();
				break;
			case 7:
				sort_by_discovery();
				break;
			case 8:
				sort_by_remainings();
				break;
			case 9:
				print_stability();
				break;
			case 10:
				cout << "\n1. Group\n2. Period\n";
				cin >> z;
				if(z==1)
					show_by_group();
				else if(z==2)
					show_by_period();
				else
					cout << "choose a valid option please!\n";
				break;
			case 11:
				cout << "Name: " << endl;
				cin.ignore();
				getline(cin, name);
				if(is_compound_exist(name)){
					show_compound(compounds[search_compound(name)]);
				}else
					cout << "There is no compound with the given name in lablatuary\n";
				break;
			case 12:
				save();
				break;
			case 13:
				load();
				break;
			case 14:
				break;
		}
	}
	cout << "\nDONE.";
	return 0;
}