#include "parser.h"


/*
 *******************************************************************************
 *                        Parsing Function Definitions                         *
 *******************************************************************************
*/


// Returns positive number of bytes parsed on success, else zero
off_t Parser::accept_timestamp (char const *line, unsigned long long *timestamp_p) {
    off_t offset = 0;
    unsigned long long timestamp;

    // Drop leading whitespace
    while (isspace(line[offset])) {
        offset++;
    }

    // Must accept a least one digit
    if (!isdigit(line[offset])) {
        return 0;
    } else {
        timestamp = static_cast<unsigned long long>(line[offset++] - '0');
    }

    // As long as more digits exist
    while (isdigit(line[offset])) {
        timestamp = timestamp * 10 + static_cast<unsigned long long>((line[offset] - '0'));
        offset++;
    }

    // Assign the timestamp pointer
    if (timestamp_p != nullptr) {
        *timestamp_p = timestamp;
    }

    return offset;
}


// Returns positive number of bytes parsed on success, else zero
off_t Parser::accept_symbol (char sym, char const *line) {
    off_t offset = 0;

    // Drop leading whitespace
    while (isspace(line[offset])) {
        offset++;
    }

    // Must accept exactly one delimiter
    if (line[offset] != sym) {
        return 0;
    }

    return (offset + 1);
}
// {timestamp, [(taskname,0),(taskname,1),(taskname,0)]}
off_t Parser::accept_task_state (task_state_t *state_p, char const *line) {
    off_t offset = 0, temp = 0, id_size = 0;
    char id[MAX_IDENTIFIER_SIZE] = {0};
    bool active = false;

    // Drop leading whitespace
    while (isspace(line[offset])) {
        offset++;
    }

    // Expect a parentheses
    if ((temp = accept_symbol(OPEN_PAR, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << "(";

    // Drop more leading whitespace
    while (isspace(line[offset])) {
        offset++;
    }

    // Expect at least one letter or number
    if (isalnum(line[offset]) == 0) {
        return 0;
    } else {
        id[id_size] = line[offset];
        offset++;
        id_size++;
    }

    // While there remains space for the identifier and am still accepting alnum
    while (id_size < MAX_IDENTIFIER_SIZE && isalnum(line[offset])) {
        id[id_size] = line[offset];
        offset++;
        id_size++;
    }

    qDebug() << id;

    // Expect a delimiter
    if ((temp = accept_symbol(DELIMITER, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << ",";

    // Drop whitespace
    while (isspace(line[offset])) {
        offset++;
    }

    // Expect a binary zero or one
    if (!(line[offset] == '1' || line[offset] == '0')) {
        return 0;
    } else {
        active = (line[offset] == '1');
        offset++;
    }

    qDebug() << active;

    // Expect closing parentheses
    if ((temp = accept_symbol(CLOSE_PAR, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << ")";

    // At this point we save to the pointer, but make sure that
    // we create a string object for the C-string so that it is
    // not lost
    std::string s = id;

    // Save to the pointer
    if (state_p != nullptr) {
        state_p->task_name = s;
        state_p->active = active;
    }

    return offset;
}


off_t Parser::accept_status (char const *line, status_t *status_p) {
    off_t offset = 0, temp = 0;
    unsigned long long timestamp;
    task_state_t task_state;
    QVector<task_state_t> *task_states;

    // Drop leading whitespace
    while (isspace(line[offset])) {
        offset++;
    }

    // Accept opening brace
    if (line[offset++] != OPEN_BRACE) {
        return 0;
    }

    qDebug() << "{";

    // Accept timestamp
    if ((temp = accept_timestamp(line + offset, &timestamp)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << timestamp;

    // Accept a delimiter
    if ((temp = accept_symbol(DELIMITER, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << ",";

    // Accept an opening bracket
    if ((temp = accept_symbol(OPEN_BRACKET, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << "[";

    // Accept any number of task status (at least one)
    if ((temp = accept_task_state(&task_state, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;

        // Initialize the status array
        task_states = new QVector<task_state_t>{};

        // Add this entry into it
        task_states->append(task_state_t{task_state.task_name, task_state.active});
    }

    // If you accept a comma, then you must accept another task state entry
    do {

        if ((temp = accept_symbol(DELIMITER, line + offset)) == 0) {
            break;
        } else {
            offset += temp;
        }

        qDebug() << ",";

        if ((temp = accept_task_state(&task_state, line + offset)) == 0) {
            return 0;
        } else {
            offset += temp;

            // Add entry into it
            task_states->append(task_state_t{task_state.task_name, task_state.active});
        }
    } while (1);

    // Accept a closing bracket
    if ((temp = accept_symbol(CLOSE_BRACKET, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << "]";

    // Accept a closing brace
    if ((temp = accept_symbol(CLOSE_BRACE, line + offset)) == 0) {
        return 0;
    } else {
        offset += temp;
    }

    qDebug() << "}\n";

    // Configure the given pointer
    *status_p = status_t{timestamp, task_states};

    return 1;
}


/*
 *******************************************************************************
 *                      General Class Method Definitions                       *
 *******************************************************************************
*/


Parser::Parser(char const *filename):
    d_filename(filename),
    d_file(nullptr),
    d_status(STATUS_OK),
    d_status_array(nullptr)
{

    // Open the file
    FILE *file = fopen(filename, "r");

    qDebug() << "Opening file " << filename << " ... \n";

    // Exit on failure
    if (file == nullptr) {
        d_status = STATUS_FILE_NOT_FOUND;
        qDebug() << "Couldn't actually find any file called " << filename << "\n";
        return;
    } else {
        qDebug() << "Found " << filename << "!\n";
    }

    // Save file pointer
    d_file = file;

    // Initialize the status array
    d_status_array = new QVector<status_t>{};

    qDebug() << "Okay the file is open, now invoking the parse method()!\n";

    // Call the parser
    this->parse();

    // Close the file
    fclose(file);

}

Parser::~Parser() {

    // Destroy elements of the status array
    if (d_status_array != nullptr) {
        for (status_t s : *d_status_array) {
            if (s.task_states_p != nullptr) {
                   delete s.task_states_p;
            }
        }
    }

    // Destroy the status array itself
    delete d_status_array;
}

void Parser::parse () {
    char line_buffer[MAX_LINE_LENGTH];
    status_t status;
    off_t line_count = 0;

    // While can read lines, ...
    while (fgets(line_buffer, MAX_LINE_LENGTH, d_file) != nullptr) {
        line_count++;
        if (accept_status(line_buffer, &status) == 0) {
            d_status = STATUS_PARSE_ERROR;
            qDebug() << "Parse error on line " << line_count << "\n";
            break;
        } else {
            d_status_array->append(status_t{status.milliseconds, status.task_states_p});
        }
    }
}
