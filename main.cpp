#include "mainwindow.h"
#include <QApplication>
#include "parser.h"
#include <QString>


QColor get_random_color () {

    // 0xCC in order to not allow anything too bright (i.e. white)
    uint8_t r = rand() % 0xCC;
    uint8_t g = rand() % 0xCC;
    uint8_t b = rand() % 0xCC;
    return QColor{r,g,b};
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Parser *p = nullptr;
    char const *filename = nullptr;
    QVector<int> task_identifiers{};
    QHash<QString, int> task_hash_map;
    w.show();

    // Open an empty display if no data was specified
    if (argc <= 1) {
        return a.exec();
    } else {
        qDebug() << "Running with input file: " << argv[1] << "\n";
        p = new Parser{(filename = argv[1])};
    }

    // Clean up and present an empty display if the file couldn't be found or not parsed
    if (p->status() != STATUS_OK) {
        qDebug() << "Parser reported code " << p->status() << "\n";
        delete p;
        return a.exec();
    }

    // Otherwise the data could be parsed, so begin loading it in
    double x_last_offset = 0.0;
    for (status_t s : *(p->status_array())) {

        // The offset is the timestamp
        double x_offset = static_cast<double>(s.milliseconds);

        // Perform a step-size in line with the offset
        w.step(x_offset - x_last_offset);

        // Update the last offset
        x_last_offset = x_offset;

        // Enable or disable tasks at this timestamp
        for (task_state_t task : *(s.task_states_p)) {
            int task_id = -1;

            // Create a QString with the name
            QString task_name = QString::fromStdString(task.task_name);

            // If the task exists, obtain its numerical id
            if (task_hash_map.contains(task_name)) {
                task_id = task_hash_map.value(task_name);
            } else {
                task_id = w.addTask(task_name, get_random_color(), x_offset);
                task_hash_map[task_name] = task_id;
            }

            // Active the task if it is going active, otherwise deactive it
            if (task.active) {
                w.startTask(task_id);
            } else {
                w.stopTask(task_id);
            }
        }

    }

    qDebug() << "Done - all data should be displayed!";

//    // Global x offset
//    double x_offset = 0.0;

//    // Task storage
//    QVector<int> task_ids{};

//    // Create a few tasks
//    task_ids.append(w.addTask("A", QColor{255, 0, 0}, x_offset));
//    task_ids.append(w.addTask("B", QColor{0, 255, 0}, x_offset));
//    task_ids.append(w.addTask("C", QColor{0, 0, 255}, x_offset));
//    task_ids.append(w.addTask("D", QColor{120, 50, 155}, x_offset));
//    task_ids.append(w.addTask("E", QColor{200, 60, 35}, x_offset));

//    // Start and stop the tasks arbitrarily
//    for (int i = 0; i < 500; ++i) {
//        int task_number = rand() % task_ids.length();

//        if ((rand() % 2) == 0) {
//            w.startTask(task_number);
//        } else {
//            w.stopTask(task_number);
//        }

//        w.step(1.0);
//    }

    // Deallocate the parser
    delete p;

    return a.exec();
}
