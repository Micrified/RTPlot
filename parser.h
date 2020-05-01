#ifndef PARSER_H
#define PARSER_H

#include <QVector>
#include <QDebug>


/*
 *******************************************************************************
 *                             Symbolic Constants                              *
 *******************************************************************************
*/


#define OPEN_BRACE            '{'
#define CLOSE_BRACE           '}'
#define OPEN_BRACKET          '['
#define CLOSE_BRACKET         ']'
#define OPEN_PAR              '('
#define CLOSE_PAR             ')'
#define DELIMITER             ','

#define MAX_IDENTIFIER_SIZE    32
#define MAX_LINE_LENGTH        512


/*
 *******************************************************************************
 *                              Type Definitions                               *
 *******************************************************************************
*/


// Enumeration of parser status
typedef enum {
    STATUS_OK,
    STATUS_FILE_NOT_FOUND,
    STATUS_PARSE_ERROR
} parser_status_t;


// Structure describing a change in task state
typedef struct {
    std::string task_name;
    bool active;
} task_state_t;


// Structure describing a change in status
typedef struct {
    unsigned long long milliseconds;
    QVector<task_state_t> *task_states_p;
} status_t;


/*
 *******************************************************************************
 *                           Parser Class Definition                           *
 *******************************************************************************
*/


class Parser
{
public:
    Parser(char const *filename);
    ~Parser();

    // Returns the parser status
    parser_status_t status () {
        return d_status;
    }

    // Returns the parsed status data
    QVector<status_t> *status_array() {
        return d_status_array;
    }

private:

    // The associated filename
    char const *d_filename;

    // The file object
    FILE *d_file;

    // The parser internal status
    parser_status_t d_status;

    // The parsed status data
    QVector<status_t> *d_status_array;

    // Parsing procedures
    off_t accept_timestamp (char const *, unsigned long long *);
    off_t accept_symbol (char, char const *);
    off_t accept_task_state (task_state_t *, char const *);
    off_t accept_status (char const *, status_t *);

    // The parse procedure
    void parse ();
};

#endif // PARSER_H
