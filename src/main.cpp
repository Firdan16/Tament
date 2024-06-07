#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <algorithm>

using namespace std;

void banner()
{
    cout << "\n";
    cout << "████████╗ █████╗ ███╗   ███╗███████╗███╗   ██╗████████╗\n";
    cout << "╚══██╔══╝██╔══██╗████╗ ████║██╔════╝████╗  ██║╚══██╔══╝\n";
    cout << "   ██║   ███████║██╔████╔██║█████╗  ██╔██╗ ██║   ██║   \n";
    cout << "   ██║   ██╔══██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   \n";
    cout << "   ██║   ██║  ██║██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   \n";
    cout << "   ╚═╝   ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   \n";
    cout << "\n";
}

enum Status
{
    TODO,
    DONE
};

enum Kategori
{
    WORK,
    PERSONAL,
    OTHER
};

struct Task
{
    string nama;
    string deskripsi;
    Status status;
    Kategori kategori;

    Task(string n, string d, Status s, Kategori k) : nama(n), deskripsi(d), status(s), kategori(k) {}
};

string statusToString(Status s)
{
    switch (s)
    {
    case TODO:
        return "TODO";
    case DONE:
        return "DONE";
    default:
        return "UNKNOWN";
    }
}

string kategoriToString(Kategori k)
{
    switch (k)
    {
    case WORK:
        return "WORK";
    case PERSONAL:
        return "PERSONAL";
    case OTHER:
        return "OTHER";
    default:
        return "UNKNOWN";
    }
}

Kategori inputKategori()
{
    int pilihan;
    cout << "Pilih Kategori (0: WORK, 1: PERSONAL, 2: OTHER): ";
    cin >> pilihan;
    cin.ignore();
    switch (pilihan)
    {
    case 1:
        return PERSONAL;
    case 2:
        return OTHER;
    default:
        return WORK;
    }
}

Task *createTask()
{
    string nama, deskripsi;

    cout << "\n========  MEMBUAT TASK  ========\n";
    cout << "Masukkan Nama Task: ";
    getline(cin, nama);
    cout << "Masukkan Deskripsi Task: ";
    getline(cin, deskripsi);

    Status status = TODO;
    Kategori kategori = inputKategori();

    cout << "Task berhasil dibuat!\n";
    return new Task(nama, deskripsi, status, kategori);
}

void displayTasks(const vector<Task *> &tasks)
{
    cout << "\n";
    cout << left << setw(5) << "No"
         << left << setw(20) << "Nama"
         << left << setw(30) << "Deskripsi"
         << left << setw(10) << "Status"
         << left << setw(15) << "Kategori" << endl;

    cout << string(85, '=') << endl;

    int index = 1;
    for (const Task *task : tasks)
    {
        cout << left << setw(5) << index++
             << left << setw(20) << task->nama
             << left << setw(30) << task->deskripsi
             << left << setw(10) << statusToString(task->status)
             << left << setw(15) << kategoriToString(task->kategori) << endl;
    }
}


void displayTodoTasks(const vector<Task *> &tasks)
{
    cout << "\n";
    cout << left << setw(5) << "No"
         << left << setw(20) << "Nama"
         << left << setw(30) << "Deskripsi"
         << left << setw(15) << "Kategori" << endl;

    cout << string(85, '=') << endl;

    int index = 1;

    for (const Task *task : tasks)
    {
        if(task->status==TODO){
            cout << left << setw(5) << index
                << left << setw(20) << task->nama
                << left << setw(30) << task->deskripsi
                << left << setw(15) << kategoriToString(task->kategori) << endl;
            index++;
        }
    }
}

void displayTasksByCategory(const vector<Task *> &tasks, Kategori category)
{
    vector<Task *> filteredTasks;
    for (const Task *task : tasks)
    {
        if (task->kategori == category)
        {
            filteredTasks.push_back(const_cast<Task *>(task));
        }
    }
    displayTasks(filteredTasks);
}

void displayAllTasks(const vector<Task *> &tasks)
{
    displayTasks(tasks);
}

int todoSize(const vector<Task *> &tasks)
{
    int index = 0;

    for (const Task *task : tasks)
    {
        if(task->status==TODO){
            index++;
        }
    }
    return index;
}

void markAsDone(vector<Task *> &tasks)
{
    int todoTaskSize = todoSize(tasks);

    if(todoTaskSize == 0){
        cout << "Tidak ada Todo task saat ini";
        return;
    }

    displayTodoTasks(tasks);
    cout << "Pilih nomor task yang ingin ditandai sebagai selesai: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > todoTaskSize)
    {
        cout << "Nomor task tidak valid." << endl;
        return;
    }

    tasks[nomorTask - 1]->status = DONE;
    cout << "Task '" << tasks[nomorTask - 1]->nama << "' berhasil ditandai sebagai selesai." << endl;
}

void deleteTask(vector<Task *> &tasks)
{
    int deleteTask = todoSize(tasks);

    if (deleteTask == 0){
        cout << "Tidak ada task yang dapat dihapus" << endl;
        return;
    }

    displayAllTasks(tasks);

    cout << "Pilih nomor task yang ingin dihapus: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > deleteTask)
    {
        cout << "Nomor task tidak Seelsai" << endl;
        return;
    }

    delete tasks[nomorTask - 1];
    tasks.erase(tasks.begin() + (nomorTask - 1));
    cout << "Task '" << tasks[nomorTask - 1]->nama << "' berhasil ditandai sebagai selesai" << endl;
}

int main()
{
    vector<Task *> tasks;
    banner();
    while (true)
    {
        cout << "\n========  MENU UTAMA  ========\n";
        cout << "1. Tambah Task" << endl;
        cout << "2. Tampilkan Task Berdasarkan Kategori" << endl;
        cout << "3. Tampilkan Semua Task" << endl;
        cout << "4. Tandai Task Selesai" << endl;
        cout << "5. Tampilkan Todo Task" << endl;
        cout << "6. Hapus Task" << endl;
        cout << "7. Keluar" << endl;
        cout << "Pilih opsi: ";

        int opsi;
        cin >> opsi;
        cin.ignore();

        switch (opsi)
        {
        case 1:
        {
            Task *newTask = createTask();
            tasks.push_back(newTask);
            break;
        }
        case 2:
        {
            cout << "Pilih Kategori untuk Ditampilkan (0: WORK, 1: PERSONAL, 2: OTHER): ";
            int kategoriPilihan;
            cin >> kategoriPilihan;
            cin.ignore();
            Kategori kategori = static_cast<Kategori>(kategoriPilihan);
            displayTasksByCategory(tasks, kategori);
            break;
        }
        case 3:
        {
            displayAllTasks(tasks);
            break;
        }
        case 4:
        {
            markAsDone(tasks);
            break;
        }
        case 5:
        {
            displayTodoTasks(tasks);
            break;
        }
        case 6:
        {
            deleteTask(tasks);
            break;
        }
        case 7:
        {
            cout << "Keluar program" << endl;
            exit(EXIT_SUCCESS);
        }
        default:
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opsi tidak valid" << endl;
            break;
        }
        }
    }

    for (Task *task : tasks)
    {
        delete task;
    }

    return 0;
}

struct Task {
    std::string name;
    int priority;
};

bool comparePriority(const Task &task1, const Task &task2) {
    return task1.priority < task2.priority;
}

int main() {
    std::vector<Task> taskList = {{"Tugas 1", 2}, {"Tugas 2", 1}, {"Tugas 3", 3}};

    std::cout << "Sebelum diurutkan berdasarkan prioritas:" << std::endl;
    for (const auto &task : taskList) {
        std::cout << task.name << " - Prioritas: " << task.priority << std::endl;
    }

    std::sort(taskList.begin(), taskList.end(), comparePriority);

    std::cout << "\nSetelah diurutkan berdasarkan prioritas:" << std::endl;
    for (const auto &task : taskList) {
        std::cout << task.name << " - Prioritas: " << task.priority << std::endl;
    }

    return 0;
}