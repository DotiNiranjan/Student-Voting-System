#include <bits/stdc++.h>
using namespace std;

class Encryption {
    public:
    static void encode() {
        cout << "\x1b[33m"; // Foreground color
        cout << "\x1b[43m"; // Background color
    }
    static void decode() {
        cout << "\x1b[0m"; // Reset to default colors
    }
};

class Candidate {
    private:
        int cid;
        string cname;
        int votes;
    public:
        Candidate(int id, const string& cname) : cid(id), cname(cname), votes(0) {}
        
        void vote() {
            votes++;
        }
        int getVotes() const {
            return votes;
        }
        const string& getName() const {
            return cname;
        }
};



class Election {
    private:
        int year;
        string branch;
        int totalVoters;
        vector<Candidate> candidates; 
    public:
        Election() : year(0), totalVoters(0) {} // Ensure values are initialized
        
        void setElectionInfo(int year, const string& branch, int totalVoters) {
            this->year = year;
            this->branch = branch;
            this->totalVoters = totalVoters;
        }
        
        void addCandidate(const Candidate& candidate) {
            candidates.push_back(candidate);
        }
        
        void castVote(int candidateIndex) {
            if (candidateIndex >= 0 && candidateIndex < candidates.size()) {
                candidates[candidateIndex].vote();
            }
        }
        
        const vector<Candidate>& getCandidates() const {
            return candidates;
        }
        
        int getYear() const {
            return year;
        }
        
        const string& getBranch() const {
            return branch;
        }
        
        int getTotalVoters() const {
            return totalVoters;
        }
        
        int getWinner() {
            int maxVotes = -1;
            int winnerIndex = -1;
            bool tie = false;

            for (size_t i = 0; i < candidates.size(); ++i) { 
                if (candidates[i].getVotes() > maxVotes) {
                    maxVotes = candidates[i].getVotes();
                    winnerIndex = i;
                    tie = false;
                } else if (candidates[i].getVotes() == maxVotes) {
                    tie = true;
                }
            }

            return (tie ? -1 : winnerIndex);
        }
};

class Student {
    public:
    static bool isValid(const string& userID, const Election& election) {
        if(userID.length() != 14) {
            return false;
        }
        int inputedYear = stoi(userID.substr(0, 4)); // 2024
        string branchCode = userID.substr(4, 5); // btcse
        int inputedRollNo = stoi(userID.substr(9, 5));  // 233
        
        return (inputedYear == election.getYear() &&
                inputedRollNo <= election.getTotalVoters() &&
                branchCode == election.getBranch());
    }

    static bool hasVoted(const string& userID, const vector<char>& studentVoters) {
        int location = stoi(userID.substr(9, 5)) - 1;
        return studentVoters[location] != '0';
    }

    static void vote(const string& userID, vector<char>& studentVotes, Election& election) {
        int candidateIndex;
        cout << "Candidates for election:" << endl;
        const vector<Candidate>& candidates = election.getCandidates();
        for (size_t i = 0; i < candidates.size(); ++i) {
            cout << i + 1 << ". " << candidates[i].getName() << endl;
        }

        do {
            cout << "Enter your vote (1-" << candidates.size() << "): ";
            cin >> candidateIndex;
        } while (candidateIndex < 1 || candidateIndex > static_cast<int>(candidates.size()));

        int location = stoi(userID.substr(9, 5)) - 1;
        studentVotes[location] = '1'; 
        election.castVote(candidateIndex - 1);
        cout << "Vote cast successfully!" << endl;
    }
};

class Admin {
  public:
    static bool authenticate() {
        string username, password;
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        Encryption::encode();
        cin >> password;
        Encryption::decode();
        return (username == "Admin" && password == "Admin");
    }
    
    static void initiateNewElection(Election& election) {
        int year, totalVoters, numOfCandidates;
        string branch;
        
        cout << "Enter Election Year: ";
        cin >> year;
        
        cout << "Enter Total Voters: ";
        cin >> totalVoters;
        
        cout << "Enter Election Branch: ";
        cin >> branch;
        
        election.setElectionInfo(year, branch, totalVoters);
        
        // Adding predefined candidates
        vector<string> predefinedCandidates = {"Alice", "Bob", "Charlie", "David", "Eve"};
        for (size_t i = 0; i < predefinedCandidates.size(); ++i) {
            Candidate candidate(i + 1, predefinedCandidates[i]);
            election.addCandidate(candidate);
        }
        
        cout << "Predefined candidates have been added." << endl;
    }
    
    static void saveElectionInfo(Election& election) {
    string filename = "ElectionInfo_" + election.getBranch() + ".txt";
    ofstream file(filename);

    if(!file.is_open()) {
        cout << "Error: Unable to create or open the Election Info file! Filename: " << filename << endl;
        return;
    }

    file << election.getTotalVoters() << endl;
    file << election.getCandidates().size() << endl;

    for(const Candidate& candidate : election.getCandidates()) {
        file << candidate.getVotes() << " " << candidate.getName() << endl;        
    }

    file << election.getBranch() << endl;
    file << election.getYear() << endl;

    file.close();
    cout << "Election information has been saved successfully!\n";
}

    
    static Election loadElectionInfo() {
        string stream;
        cout << "Enter the branch name (5 letters, lowercase) to load election info: ";
        cin >> stream;
        string filename = "ElectionInfo_" + stream + ".txt";
        Election election;
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Unable to load the election info, better initiate it again!\n";
            return election;
        }
        
        int totalVoters, totalCandidates;
        file >> totalVoters; file.ignore();
        file >> totalCandidates; file.ignore();
        
        for (int i = 1; i <= totalCandidates; i++) {
            int votes;
            string cname;
            file >> votes;
            file.ignore();
            getline(file, cname);
            
            Candidate candidate(i, cname);
            for (int j = 1; j <= votes; j++) {
                candidate.vote();
            }
            election.addCandidate(candidate);
        }
        
        string branch;
        getline(file, branch);
        int year;
        file >> year;
        election.setElectionInfo(year, branch, totalVoters);
        
        file.close();
        cout << "The Election Info has been loaded successfully!\n";
        return election;
    }
};

int main()
{
    Election election;
    vector<char> studentVotes(500, '0');
    bool electionInitialized = false; // To check if the election is initialized

    while (true) {
        cout << "\n\t\t\t1. Student panel\n\t\t\t2. Admin panel\n\t\t\t3. Exit\n\t\t\tOption: ";
        char option;
        cin >> option;

        switch (option) {
            case '1': {
                if (!electionInitialized) {
                    cout << "Election is not yet initialized. Please ask the admin to set up the election first." << endl;
                    continue;
                }

                string userID;
                cout << "Enter user ID (e.g., 2024btcse00001): ";
                cin >> userID;

                if (!Student::isValid(userID, election)) {
                    cout << "Invalid User ID. Please try again." << endl;
                    continue;
                }

                if (Student::hasVoted(userID, studentVotes)) {
                    cout << "You have already voted." << endl;
                    continue;
                }

                Student::vote(userID, studentVotes, election);
                Admin::saveElectionInfo(election);
                break;
            }
            case '2': {
                if (!Admin::authenticate()) {
                    cout << "Invalid credentials. Access denied." << endl;
                    continue;
                }
                cout << "";
                while (true) {
                    cout << "\n\t\t\t1. New Election\n\t\t\t2. Save Election Info\n\t\t\t3. Load Election Info\n\t\t\t4. View Election Results\n\t\t\t5. Logout\n\t\t\tOption: ";
                    cin >> option;

                    switch (option) {
                        case '1':
                            Admin::initiateNewElection(election);
                            electionInitialized = true;  // Set election as initialized
                            break;
                        case '2':
                            Admin::saveElectionInfo(election);
                            break;
                        case '3':
                            election = Admin::loadElectionInfo();
                            electionInitialized = true;  // Mark as initialized when loaded
                            break;
                        case '4': {
                            const vector<Candidate>& candidates = election.getCandidates();
                            int winnerIndex = election.getWinner();
                            if (winnerIndex != -1) {
                                cout << "\nWinner: " << candidates[winnerIndex].getName() << " with " << candidates[winnerIndex].getVotes() << " votes." << endl;
                            } else {
                                cout << "\nIt's a tie." << endl;
                            }
                            cout << "\nElection Results:" << endl;
                            for (size_t i = 0; i < candidates.size(); ++i) {
                                cout << candidates[i].getName() << ": " << candidates[i].getVotes() << " votes." << endl;
                            }
                            break;
                        }
                        case '5':
                            break;
                        default:
                            cout << "Invalid option. Please try again." << endl;
                            continue;
                    }

                    if (option == '5') {
                        break;
                    }
                }

                break;
            }
            case '3':
                cout << "Exiting program..." << endl;
                return 0;
            default:
                cout << "Invalid option. Please try again." << endl;
                continue;
        }
    }

    return 0;
}
