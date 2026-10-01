/*
Program Name: EECS 348 Assignment 3 - CEO Email Priority Queue
Brief Description: C++ object-oriented program that prioritizes a CEO's emails using a
                   custom MaxHeap and priority queue. Email priority is determined first
                   by sender category and then by newest date.
Inputs: A test file containing EMAIL, NEXT, READ, and COUNT commands.
Outputs: Terminal output showing the unread email count and the next email to read.
Collaborators: Google Gemini and Anthropic Claude were used as GenAI collaborators.
               The submitted program is an improved version based primarily on Claude's
               generated design, with modifications by the author.
Other Sources: EECS 348 Assignment 3 instructions and rubric. No external code websites used.
Author: Shashank Aitha
Creation Date: September 29, 2026
Revision Date: September 29, 2026
Revisions: Corrected output formatting, simplified priority comparison, improved parsing,
           added required prolog/source attribution, and added detailed comments.
*/

/*
Source attribution for this block:
The overall object-oriented organization and array/list-backed MaxHeap approach were based
on the Claude-generated program supplied for the required GenAI comparison. The author
modified the implementation to match the assignment's required output and behavior.
*/

#include <iostream>   // Provides std::cout and std::cerr for terminal output.
#include <fstream>    // Provides std::ifstream for reading the command file.
#include <string>     // Provides std::string for email fields and command text.
#include <vector>     // Provides the list-based storage used by the custom MaxHeap.
#include <stdexcept>  // Provides std::out_of_range for invalid heap access.
#include <cctype>     // Provides std::isspace for trimming command input.

/*
The Email class stores one email and encapsulates all priority comparison logic.
Priority rules come directly from the assignment:
1. Boss > Subordinate > Peer > ImportantPerson > OtherPerson.
2. For the same sender category, the newest date has higher priority.
*/
class Email {
private:
    std::string senderCategory; // Stores the sender category exactly as read from the file.
    std::string subject;        // Stores the email subject line.
    std::string dateText;       // Stores the original MM-DD-YYYY date for display.
    int categoryPriority;       // Stores a numeric rank for the sender category.
    long datePriority;          // Stores YYYYMMDD so newer dates compare as larger numbers.

    // Converts a valid sender category into its required numeric priority.
    static int getCategoryPriority(const std::string& category) {
        if (category == "Boss") return 5;            // Boss emails have the highest priority.
        if (category == "Subordinate") return 4;     // Subordinate emails are second.
        if (category == "Peer") return 3;            // Peer emails are third.
        if (category == "ImportantPerson") return 2; // ImportantPerson emails are fourth.
        if (category == "OtherPerson") return 1;     // OtherPerson emails are last.
        return 0;                                    // Defensive fallback for unexpected input.
    }

    // Converts MM-DD-YYYY into YYYYMMDD for efficient chronological comparison.
    static long getDatePriority(const std::string& date) {
        long month = std::stol(date.substr(0, 2));   // Extracts the two-digit month.
        long day = std::stol(date.substr(3, 2));     // Extracts the two-digit day.
        long year = std::stol(date.substr(6, 4));    // Extracts the four-digit year.
        return year * 10000 + month * 100 + day;     // Produces a sortable numeric date.
    }

public:
    // Constructs an Email object and precomputes both priority values once.
    Email(const std::string& category, const std::string& emailSubject,
          const std::string& date)
        : senderCategory(category),                  // Saves the sender category.
          subject(emailSubject),                     // Saves the subject.
          dateText(date),                            // Saves the original date string.
          categoryPriority(getCategoryPriority(category)), // Computes sender priority.
          datePriority(getDatePriority(date)) {}     // Computes chronological priority.

    // Returns true when this email must appear ahead of another email in the MaxHeap.
    bool hasHigherPriorityThan(const Email& other) const {
        if (categoryPriority != other.categoryPriority) {  // First compare sender categories.
            return categoryPriority > other.categoryPriority; // Higher category rank wins.
        }
        return datePriority > other.datePriority;     // If tied, the newer email wins.
    }

    // Displays one email in exactly the format shown by the assignment sample output.
    void display() const {
        std::cout << "Next email:" << std::endl;      // Prints the NEXT heading.
        std::cout << "Sender: " << senderCategory << std::endl; // Prints sender category.
        std::cout << "Subject: " << subject << std::endl;       // Prints subject line.
        std::cout << "Date: " << dateText << std::endl;         // Prints original date.
    }
};

/*
The MaxHeap class is a custom list-based binary max-heap.
The vector is only the underlying list storage; no pre-existing heap module or heap
algorithm is used. All heap behavior is implemented below with index arithmetic.
For index i: parent=(i-1)/2, left=2i+1, right=2i+2.
*/
class MaxHeap {
private:
    std::vector<Email> items; // Stores heap nodes in level-order as a list.

    // Returns the parent index for a non-root heap node.
    static size_t parent(size_t index) {
        return (index - 1) / 2; // Standard zero-based binary-heap parent formula.
    }

    // Returns the left-child index.
    static size_t leftChild(size_t index) {
        return 2 * index + 1; // Standard zero-based left-child formula.
    }

    // Returns the right-child index.
    static size_t rightChild(size_t index) {
        return 2 * index + 2; // Standard zero-based right-child formula.
    }

    // Restores heap order after a new item is appended.
    void siftUp(size_t index) {
        while (index > 0) { // Continues until the item reaches the root or its correct place.
            size_t parentIndex = parent(index); // Finds the current node's parent.
            if (!items[index].hasHigherPriorityThan(items[parentIndex])) {
                break; // Stops when the parent already has equal or higher priority.
            }
            std::swap(items[index], items[parentIndex]); // Moves higher-priority email upward.
            index = parentIndex; // Continues checking from the email's new position.
        }
    }

    // Restores heap order after the root is replaced during removal.
    void siftDown(size_t index) {
        while (true) { // Repeats until the current item is in a valid heap position.
            size_t largest = index; // Assumes the current node is highest priority initially.
            size_t left = leftChild(index); // Calculates the left-child position.
            size_t right = rightChild(index); // Calculates the right-child position.

            if (left < items.size() &&
                items[left].hasHigherPriorityThan(items[largest])) {
                largest = left; // Selects the left child when it has higher priority.
            }

            if (right < items.size() &&
                items[right].hasHigherPriorityThan(items[largest])) {
                largest = right; // Selects the right child when it has the highest priority.
            }

            if (largest == index) {
                break; // Stops when neither child should move above the current item.
            }

            std::swap(items[index], items[largest]); // Moves the highest-priority child upward.
            index = largest; // Continues from the displaced item's new position.
        }
    }

public:
    // Returns true when the heap contains no emails.
    bool isEmpty() const {
        return items.empty(); // Delegates only storage checking to the vector.
    }

    // Returns the number of unread emails currently stored.
    size_t size() const {
        return items.size(); // Heap size equals the number of list elements.
    }

    // Inserts an email and restores MaxHeap order.
    void insert(const Email& email) {
        items.push_back(email); // Adds the new email at the end of the complete tree.
        siftUp(items.size() - 1); // Moves it upward until heap order is restored.
    }

    // Returns the highest-priority email without deleting it.
    const Email& getMax() const {
        if (isEmpty()) { // Protects against invalid access to element zero.
            throw std::out_of_range("Cannot get maximum from an empty heap.");
        }
        return items[0]; // In a MaxHeap, the root is always the maximum-priority item.
    }

    // Deletes the highest-priority email and restores MaxHeap order.
    void removeMax() {
        if (isEmpty()) { // READ on an empty inbox must not crash.
            return; // Quietly does nothing when no email exists.
        }

        if (items.size() == 1) { // Handles the one-email case without further heap work.
            items.pop_back(); // Removes the only email.
            return; // Heap is now empty.
        }

        items[0] = items.back(); // Moves the final complete-tree node to the root.
        items.pop_back(); // Removes the duplicate final list element.
        siftDown(0); // Pushes the replacement root down to restore MaxHeap order.
    }
};

/*
The CEOInbox class is the assignment's priority queue abstraction.
It uses MaxHeap for every prioritization operation instead of using a library queue.
*/
class CEOInbox {
private:
    MaxHeap heap; // Stores unread emails in priority order through the custom MaxHeap.

public:
    // Adds a newly received email to the priority queue.
    void addEmail(const std::string& sender, const std::string& subject,
                  const std::string& date) {
        Email email(sender, subject, date); // Builds an Email object from parsed fields.
        heap.insert(email); // Inserts the object into the custom MaxHeap.
    }

    // Implements NEXT without removing the email.
    void showNext() const {
        if (heap.isEmpty()) { // Checks the edge case required by the grader.
            return; // Produces no output because the assignment specifies no empty-NEXT text.
        }
        heap.getMax().display(); // Displays the same root on repeated NEXT commands.
    }

    // Implements READ by deleting the current highest-priority email.
    void readEmail() {
        heap.removeMax(); // Also safely handles repeated READ commands on an empty heap.
    }

    // Implements COUNT using the current heap size.
    void showCount() const {
        std::cout << "There are " << heap.size()
                  << " emails to read." << std::endl; // Matches the sample output wording.
    }
};

/*
The CommandProcessor class owns file parsing and command dispatch.
EMAIL lines are split on commas; the assignment guarantees that subjects contain no commas.
*/
class CommandProcessor {
private:
    CEOInbox inbox; // Owns the CEO's priority queue.

    // Removes leading/trailing whitespace, including Windows carriage returns.
    static std::string trim(const std::string& text) {
        size_t start = 0; // Begins scanning at the first character.
        size_t end = text.size(); // Begins the ending scan one past the final character.

        while (start < end &&
               std::isspace(static_cast<unsigned char>(text[start]))) {
            ++start; // Skips whitespace at the beginning.
        }

        while (end > start &&
               std::isspace(static_cast<unsigned char>(text[end - 1]))) {
            --end; // Skips whitespace at the end.
        }

        return text.substr(start, end - start); // Returns only meaningful characters.
    }

    // Parses the three comma-delimited fields following an EMAIL command.
    void processEmail(const std::string& fields) {
        size_t firstComma = fields.find(','); // Finds the sender/subject delimiter.
        size_t lastComma = fields.rfind(','); // Finds the subject/date delimiter.

        if (firstComma == std::string::npos ||
            lastComma == std::string::npos ||
            firstComma == lastComma) {
            return; // Defensive check; the assignment says grader input is properly formatted.
        }

        std::string sender = trim(fields.substr(0, firstComma)); // Extracts sender category.
        std::string subject = trim(
            fields.substr(firstComma + 1, lastComma - firstComma - 1)); // Extracts subject.
        std::string date = trim(fields.substr(lastComma + 1)); // Extracts date.

        inbox.addEmail(sender, subject, date); // Adds the parsed email to the priority queue.
    }

    // Executes one command line from the test file.
    void processLine(const std::string& rawLine) {
        std::string line = trim(rawLine); // Normalizes line endings and outer whitespace.

        if (line.empty()) { // Ignores blank lines if any are present.
            return; // No command exists on a blank line.
        }

        if (line.rfind("EMAIL ", 0) == 0) { // Detects EMAIL followed by the required space.
            processEmail(line.substr(6)); // Sends only the comma-delimited fields to the parser.
        } else if (line == "NEXT") { // Detects a NEXT command.
            inbox.showNext(); // Displays but does not remove the highest-priority email.
        } else if (line == "READ") { // Detects a READ command.
            inbox.readEmail(); // Removes the highest-priority unread email.
        } else if (line == "COUNT") { // Detects a COUNT command.
            inbox.showCount(); // Displays the number of unread emails.
        }
    }

public:
    // Opens and processes every command in the specified test file.
    bool processFile(const std::string& fileName) {
        std::ifstream inputFile(fileName); // Attempts to open the grader/sample test file.

        if (!inputFile.is_open()) { // Detects a missing or inaccessible file.
            std::cerr << "Error opening file: " << fileName << std::endl; // Reports the error.
            return false; // Signals failure to main.
        }

        std::string line; // Reuses one string for each line read from the file.

        while (std::getline(inputFile, line)) { // Reads commands until end-of-file.
            processLine(line); // Dispatches the current command through the object.
        }

        inputFile.close(); // Explicitly closes the input file.
        return true; // Signals successful processing.
    }
};

/*
main only creates the top-level object and starts processing.
The assignment's behavior is implemented by objects and their methods.
*/
int main(int argc, char* argv[]) {
    std::string fileName = "input.txt"; // Uses input.txt when no command-line file is supplied.

    if (argc > 1) { // Checks whether the user supplied a test-file path.
        fileName = argv[1]; // Uses the first command-line argument as the input file.
    }

    CommandProcessor processor; // Creates the object that owns and runs the program.
    bool success = processor.processFile(fileName); // Processes the complete command file.

    return success ? 0 : 1; // Returns zero on success and nonzero if the file could not open.
}
