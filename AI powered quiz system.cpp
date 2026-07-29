#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <iomanip>
using namespace std;
// Constants for difficulties
const int EASY = 1;
const int MEDIUM = 2;
const int HARD = 3;
// Number of total questions in bank and questions per quiz
const int BANK_SIZE = 12;   // at least 10 required; we use 12 for variety
const int QUIZ_SIZE = 10;   // present 10 questions to the user
// Structure to hold a question
struct Question {
    const char* text;       // question text (C-style string for simplicity)
    const char* opts[4];    // four options A-D
    char answer;            // correct answer 'A'..'D'
    int difficulty;         // EASY, MEDIUM, HARD
};
// Structure to hold student info
struct Student {
    string name;
    string roll;
    int age;
};
// Function prototypes
void loadQuestionBank(Question* bank, int size);
void showWelcome();
Student registerStudent();
void clearInputLine();
void toUpperStr(string &s);
void shuffleArray(int* arr, int n);
int selectQuestions(const Question* bank, int bankSize, int diff, int* selected, int k);
int conductQuiz(const Question* bank, const int* selected, int k, int &correctEasy, int &correctMed, int &correctHard, int &askedEasy, int &askedMed, int &askedHard);
char calculateGrade(double percent);
int recommendDifficulty(int currentDiff, int correct, int total, int correctEasy, int correctMed, int correctHard);
void showResultReport(const Student &s, int correct, int wrong, double percent, char grade, int recommendedDiff);
const char* diffToString(int d);

// ASCII art array used in showWelcome
const char* welcomeArt[7] = {
    "================================================================================",
    "                       _                               ",
    "                      / \\    _   _ _ __ __ _          ",
    "                     / _ \\  | | | | '__/ _` |         ",
    "                    / ___ \\ | |_| | | | (_| |         ",
    "                   /_/   \\_\\ \\__,_|_|  \\__,_||       ",
    "                AI Aura - Intelligent Adaptive Quiz  "
};

// Main function
int main() {
    srand((unsigned int)time(NULL)); // seed random number generator
    Question bank[BANK_SIZE];
    loadQuestionBank(bank, BANK_SIZE);
    showWelcome();
    Student student;
    bool registered = false;
    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Register Student\n";
        cout << "2. Take Quiz\n";
        cout << "3. Exit\n";
        cout << "Enter choice (1-3): ";
        string choice;
        getline(cin, choice);
        if (choice.empty()) continue;
        switch (choice[0]) {
            case '1':
                student = registerStudent();
                registered = true;
                break;
            case '2':
                if (!registered) {
                    cout << "Please register first (option 1).\n";
                    break;
                }
                {
                    // Ask for difficulty
                    cout << "Choose difficulty (1=Easy, 2=Medium, 3=Hard): ";
                    string dstr;
                    getline(cin, dstr);
                    int diff = EASY;
                    if (!dstr.empty()) {
                        char ch = dstr[0];
                        if (ch == '2') diff = MEDIUM;
                        else if (ch == '3') diff = HARD;
                        else diff = EASY;
                    }
                    // Select QUIZ_SIZE questions based on difficulty
                    int selected[QUIZ_SIZE];
                    int got = selectQuestions(bank, BANK_SIZE, diff, selected, QUIZ_SIZE);
                    if (got < QUIZ_SIZE) {
                        cout << "Not enough questions in chosen difficulty; using available questions.\n";
                    }
                    // Track correct answers by difficulty and counts asked per difficulty
                    int correctEasy = 0, correctMed = 0, correctHard = 0;
                    int askedEasy = 0, askedMed = 0, askedHard = 0;

                    int correct = conductQuiz(bank, selected, got, correctEasy, correctMed, correctHard, askedEasy, askedMed, askedHard);
                    int total = got;
                    int wrong = total - correct;
                    double percent = 0.0;
                    if (total > 0) percent = (double(correct) / double(total)) * 100.0;
                    char grade = calculateGrade(percent);
                    // Simple AI recommendation based on performance
                    int recommended = recommendDifficulty(diff, correct, total, correctEasy, correctMed, correctHard);
                    // Show result
                    showResultReport(student, correct, wrong, percent, grade, recommended);
                }
                break;
            case '3':
                cout << "Thank you for using the AI Powered Quiz System. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option. Please enter 1, 2, or 3.\n";
                break;
        }
    }
    return 0;
}
// Load the question bank with BANK_SIZE questions
void loadQuestionBank(Question* bank, int size) {
    // Assign fields individually to ensure compatibility with older compilers.
    bank[0].text = "What is the output of 1 + 1 in most languages?";
    bank[0].opts[0] = "A) 1"; bank[0].opts[1] = "B) 2"; bank[0].opts[2] = "C) 11"; bank[0].opts[3] = "D) Error";
    bank[0].answer = 'B'; bank[0].difficulty = EASY;

    bank[1].text = "Which keyword declares a variable in C++?";
    bank[1].opts[0] = "A) var"; bank[1].opts[1] = "B) let"; bank[1].opts[2] = "C) int"; bank[1].opts[3] = "D) define";
    bank[1].answer = 'C'; bank[1].difficulty = EASY;

    bank[2].text = "Which symbol ends a statement in C++?";
    bank[2].opts[0] = "A) ."; bank[2].opts[1] = "B) ;"; bank[2].opts[2] = "C) :"; bank[2].opts[3] = "D) ,";
    bank[2].answer = 'B'; bank[2].difficulty = EASY;

    bank[3].text = "Which function is the program entry point in C++?";
    bank[3].opts[0] = "A) start()"; bank[3].opts[1] = "B) run()"; bank[3].opts[2] = "C) main()"; bank[3].opts[3] = "D) begin()";
    bank[3].answer = 'C'; bank[3].difficulty = EASY;

    bank[4].text = "What does 'cout' do?";
    bank[4].opts[0] = "A) Input text"; bank[4].opts[1] = "B) Output text"; bank[4].opts[2] = "C) Compile code"; bank[4].opts[3] = "D) None";
    bank[4].answer = 'B'; bank[4].difficulty = EASY;

    // Medium questions (4)
    bank[5].text = "Which loop runs until a condition is false and checks at the end?";
    bank[5].opts[0] = "A) for"; bank[5].opts[1] = "B) while"; bank[5].opts[2] = "C) do-while"; bank[5].opts[3] = "D) switch";
    bank[5].answer = 'C'; bank[5].difficulty = MEDIUM;

    bank[6].text = "What container stores elements in contiguous memory?";
    bank[6].opts[0] = "A) list"; bank[6].opts[1] = "B) vector"; bank[6].opts[2] = "C) map"; bank[6].opts[3] = "D) set";
    bank[6].answer = 'B'; bank[6].difficulty = MEDIUM;

    bank[7].text = "Which header is needed for string class?";
    bank[7].opts[0] = "A) <cstring>"; bank[7].opts[1] = "B) <string>"; bank[7].opts[2] = "C) <str>"; bank[7].opts[3] = "D) <s>";
    bank[7].answer = 'B'; bank[7].difficulty = MEDIUM;

    bank[8].text = "What is the index of first element in arrays?";
    bank[8].opts[0] = "A) 0"; bank[8].opts[1] = "B) 1"; bank[8].opts[2] = "C) -1"; bank[8].opts[3] = "D) Depends";
    bank[8].answer = 'A'; bank[8].difficulty = MEDIUM;

    // Hard questions (3)
    bank[9].text  = "Which operation has higher precedence?";
    bank[9].opts[0] = "A) +"; bank[9].opts[1] = "B) -"; bank[9].opts[2] = "C) *"; bank[9].opts[3] = "D) ==";
    bank[9].answer = 'C'; bank[9].difficulty = HARD;

    bank[10].text = "What does '++i' do compared to 'i++' in expressions?";
    bank[10].opts[0] = "A) same"; bank[10].opts[1] = "B) increments after"; bank[10].opts[2] = "C) increments before"; bank[10].opts[3] = "D) error";
    bank[10].answer = 'C'; bank[10].difficulty = HARD;

    bank[11].text = "Which of the following is NOT an OOP concept?";
    bank[11].opts[0] = "A) Inheritance"; bank[11].opts[1] = "B) Polymorphism"; bank[11].opts[2] = "C) Encapsulation"; bank[11].opts[3] = "D) Compilation";
    bank[11].answer = 'D'; bank[11].difficulty = HARD;

    (void)size; // avoid unused parameter warning
}
// Display a simple welcome message
void showWelcome() {
    cout << "========================================\n";
    cout << "   Welcome to AI Powered Quiz System\n";
    cout << "   Programming Fundamentals Project\n";
    cout << "========================================\n";
    cout << "This quiz uses simple AI to recommend the next difficulty.\n";
    // Optionally show small ASCII art lines
    for (int i = 0; i < 7; ++i) {
        cout << welcomeArt[i] << "\n";
    }
}
// Register student: reads name, roll, age and returns a Student struct
Student registerStudent() {
    Student s;
    cout << "Enter your name: ";
    getline(cin, s.name);
    while (s.name.empty()) {
        cout << "Name cannot be empty. Enter your name: ";
        getline(cin, s.name);
    }
    cout << "Enter your roll number: ";
    getline(cin, s.roll);
    while (s.roll.empty()) {
        cout << "Roll cannot be empty. Enter your roll number: ";
        getline(cin, s.roll);
    }
    cout << "Enter your age: ";
    string ages;
    getline(cin, ages);
    s.age = atoi(ages.c_str()); // convert to integer; simple validation
    if (s.age <= 0) s.age = 18; // default if invalid
    cout << "Registration complete. Welcome, " << s.name << "!\n";
    return s;
}
// Clear leftover input (unused in this simple program but available)
void clearInputLine() {
    // Not used actively but provided as utility.
    // In many console environments getline handles line endings well.
}
// Convert string to uppercase
void toUpperStr(string &s) {
    for (size_t i = 0; i < s.length(); ++i) s[i] = toupper(static_cast<unsigned char>(s[i]));
}
// Simple Fisher-Yates shuffle for integer array
void shuffleArray(int* arr, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}
// Select k questions indices from the bank according to the requested difficulty.
// Returns the number of selected questions (ideally k).
// We fill selected[] with indices into bank[].
int selectQuestions(const Question* bank, int bankSize, int diff, int* selected, int k) {
    // Make a pool of indices that match the difficulty preference.
    int pool[BANK_SIZE];
    int poolCount = 0;
    // Prefer questions of the chosen difficulty; if not enough, include neighboring difficulties.
    for (int i = 0; i < bankSize; ++i) {
        if (bank[i].difficulty == diff) {
            pool[poolCount++] = i;
        }
    }
    // If pool not enough, include other questions until we have enough
    if (poolCount < k) {
        for (int i = 0; i < bankSize && poolCount < k; ++i) {
            bool already = false;
            for (int j = 0; j < poolCount; ++j) if (pool[j] == i) { already = true; break; }
            if (!already) pool[poolCount++] = i;
        }
    }
    // If still not enough (very small bank), cap at poolCount.
    int take = (poolCount < k) ? poolCount : k;

    // Shuffle pool indices and pick first 'take'
    shuffleArray(pool, poolCount);
    for (int i = 0; i < take; ++i) selected[i] = pool[i];

    return take;
}
int conductQuiz(const Question* bank, const int* selected, int k, int &correctEasy, int &correctMed, int &correctHard, int &askedEasy, int &askedMed, int &askedHard) {
    int correct = 0;
    for (int i = 0; i < k; ++i) {
        int idx = selected[i];
        const Question* qptr = &bank[idx]; // pointer usage: point to current question
        // Display question and options
        cout << "\nQuestion " << (i + 1) << ": " << qptr->text << "\n";
        cout << "A. " << qptr->opts[0] << "\n";
        cout << "B. " << qptr->opts[1] << "\n";
        cout << "C. " << qptr->opts[2] << "\n";
        cout << "D. " << qptr->opts[3] << "\n";
        // Read answer
        cout << "Your answer (A/B/C/D): ";
        string ans;
        getline(cin, ans);
        while (ans.empty()) {
            cout << "Please enter A, B, C, or D: ";
            getline(cin, ans);
        }
        toUpperStr(ans);
        char ch = ans[0];
        if (ch < 'A' || ch > 'D') {
            cout << "Invalid input; counted as wrong.\n";
            ch = 'X'; // invalid
        }
        // Check and update counts by difficulty
        if (qptr->difficulty == EASY) askedEasy++;
        else if (qptr->difficulty == MEDIUM) askedMed++;
        else if (qptr->difficulty == HARD) askedHard++;
        if (ch == qptr->answer) {
            cout << "Correct!\n";
            correct++;
            if (qptr->difficulty == EASY) correctEasy++;
            else if (qptr->difficulty == MEDIUM) correctMed++;
            else if (qptr->difficulty == HARD) correctHard++;
        } else {
            cout << "Wrong. Correct answer was: " << qptr->answer << "\n";
        }
    }
    return correct;
}
// Simple grade calculation
char calculateGrade(double percent) {
    if (percent >= 90.0) return 'A';
    else if (percent >= 80.0) return 'B';
    else if (percent >= 70.0) return 'C';
    else if (percent >= 60.0) return 'D';
    else return 'F';
}
// Simple AI recommendation for next difficulty
// Logic:
// - If score >= 80%: recommend harder (unless already HARD)
// - If score <= 50%: recommend easier (unless already EASY)
// - Else recommend same or slightly higher depending on performance in specific difficulties
int recommendDifficulty(int currentDiff, int correct, int total, int correctEasy, int correctMed, int correctHard) {
    if (total == 0) return currentDiff; // no data
    double percent = (double(correct) / double(total)) * 100.0;
    // Basic global rule
    if (percent >= 80.0) {
        if (currentDiff == EASY) return MEDIUM;
        if (currentDiff == MEDIUM) return HARD;
        return HARD;
    } else if (percent <= 50.0) {
        if (currentDiff == HARD) return MEDIUM;
        if (currentDiff == MEDIUM) return EASY;
        return EASY;
    }

    if (correctHard > 0 && correctMed > 0) {
        // did some hard questions correctly -> try to keep or increase
        if (currentDiff == EASY) return MEDIUM;
        return currentDiff;
    }
    // Default: keep same difficulty
    return currentDiff;
}
// Display the result report
void showResultReport(const Student &s, int correct, int wrong, double percent, char grade, int recommendedDiff) {
    cout << "\n========================================\n";
    cout << "Result for: " << s.name << " (Roll: " << s.roll << ")\n";
    cout << "Correct Answers: " << correct << "\n";
    cout << "Wrong Answers:   " << wrong << "\n";
    cout << "Score Percent:   " << fixed << setprecision(2) << percent << "%\n";
    cout << "Grade:           " << grade << "\n";
    cout << "AI Recommendation: Try " << diffToString(recommendedDiff) << " next time.\n";
    cout << "========================================\n";
}
// Helper to convert difficulty int to string
const char* diffToString(int d) {
    switch (d) {
        case EASY: return "Easy";
        case MEDIUM: return "Medium";
        case HARD: return "Hard";
        default: return "Unknown";
    }
}
